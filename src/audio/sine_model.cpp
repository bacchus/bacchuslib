#include "sine_model.h"
#include <vector>
#include "math/sort.h"

namespace bacchus {

void sineTracking(
        const matxd& pfreq, const matxd& pmag, const matxd& pphase // frequencies and magnitude of current frame
        , const matxd& tfreq // frequencies of incoming tracks from previous frame
        , double freqDevOffset // = 20 // slope increase of minimum frequency deviation
        , double freqDevSlope // = 0.01 // slope increase of minimum frequency deviation
        , matxd& tfreqn, matxd& tmagn, matxd& tphasen// returns: frequency, magnitude and phase of tracks
        )
{
    uint nt = tfreq.size();
    tfreqn.resize(nt); // initialize array for output frequencies
    tmagn.resize(nt); // initialize array for output magnitudes
    tphasen.resize(nt); // initialize array for output phases

    matxd pmagnz;
    pmagnz.reserve(pmag.size());
    std::vector<int> pindexes; // indexes of current peaks
    uint np = pfreq.size();
    pindexes.reserve(np);
    for (uint i = 0; i < np; ++i) {
        if (!eq(pfreq[i],0.0)) {
            pindexes.push_back(i);
            pmagnz.push_back(pmag[i]);
        }
    }

    std::vector<int> incomingTracks; // indexes of incoming tracks
    incomingTracks.reserve(nt);
    for (uint i = 0; i < nt; ++i) {
        if (!eq(tfreq[i],0.0))
            incomingTracks.push_back(i);
    }

    std::vector<int> newTracks(nt, -1); // initialize to -1 new tracks
    matxd pfreqt = pfreq; // copy current peaks to temporary array
    matxd pmagt = pmag; // copy current peaks to temporary array
    matxd pphaset = pphase; // copy current peaks to temporary array
    std::vector<int> magOrder = argsort(-pmagnz); // order current peaks by magnitude

    // continue incoming tracks
    if (incomingTracks.size() > 0) { // if incoming tracks exist
        for (int i: magOrder) { // iterate over current peaks
            if (incomingTracks.size() == 0) // break when no more incoming tracks
                break;
            matxd tfreqinc;
            tfreqinc.reserve(incomingTracks.size());
            for (int k: incomingTracks)
                tfreqinc.push_back(k);
            matxd adif = abs(matxd::fill(pfreqt[i], tfreqinc.size()) - tfreqinc);// closest incoming track to peak
            int track = std::distance(adif.begin(), std::min_element(adif.begin(), adif.end()));
            int freqDistance = std::abs(pfreq[i] - tfreq[incomingTracks[track]]); // measure freq distance
            if (freqDistance < (freqDevOffset + freqDevSlope * pfreq[i])) { // choose track if distance is small
                newTracks[incomingTracks[track]] = i; // assign peak index to track index
                incomingTracks.erase(incomingTracks.begin() + track); // delete index of track in incomming tracks
            }
        }
    }

    std::vector<int> indext; // indexes of assigned tracks
    indext.reserve(newTracks.size());
    for (uint i = 0; i < newTracks.size(); ++i) {
        if (newTracks[i] != -1)
            indext.push_back(i);
    }

    if (indext.size() > 0) {
        std::vector<int> indexp;
        for (int i: indext)
            indexp.push_back(newTracks[i]);// indexes of assigned peaks

        for (uint i = 0; i < indext.size(); ++i) {
            tfreqn[indext[i]] = pfreqt[indexp[i]]; // output freq tracks
            tmagn[indext[i]] = pmagt[indexp[i]]; // output mag tracks
            tphasen[indext[i]] = pphaset[indexp[i]]; // output phase tracks
            pfreqt.data.erase(pfreqt.data.begin() + indexp[i]); // delete used peaks
            pmagt.data.erase(pmagt.data.begin() + indexp[i]); // delete used peaks
            pphaset.data.erase(pphaset.data.begin() + indexp[i]); // delete used peaks
        }
    }

    // create new tracks from non used peaks
    std::vector<int> emptyt;// indexes of empty incoming tracks
    for (uint i = 0; i < tfreq.size(); ++i) {
        if (eq(tfreq[i], 0.0))
            emptyt.push_back(i);
    }
    std::vector<int> peaksleft = argsort(-pmagt); // sort left peaks by magnitude
    if (((peaksleft.size() > 0) && (emptyt.size() >= peaksleft.size()))) { // fill empty tracks
        for (uint i = 0; i < peaksleft.size(); ++i) {
            tfreqn[emptyt[i]] = pfreqt[peaksleft[i]];
            tmagn[emptyt[i]] = pmagt[peaksleft[i]];
            tphasen[emptyt[i]] = pphaset[peaksleft[i]];
        }
    } else if (((peaksleft.size() > 0) & (emptyt.size() < peaksleft.size()))) { // add more tracks if necessary
        for (uint i = 0; i < emptyt.size(); ++i) {
            tfreqn[emptyt[i]] = pfreqt[peaksleft[i]];
            tmagn[emptyt[i]] = pmagt[peaksleft[i]];
            tphasen[emptyt[i]] = pphaset[peaksleft[i]];
        }
        for (uint i = emptyt.size(); i < peaksleft.size(); ++i) {
            tfreqn.push_back(pfreqt[peaksleft[i]]);
            tmagn.push_back(pmagt[peaksleft[i]]);
            tphasen.push_back(pphaset[peaksleft[i]]);
        }
    }
}

void cleaningSineTracks(matxd& tfreq, uint minTrackLength = 3) {
    if (tfreq.n == 0)
        return;
    uint nFrames = tfreq.m; // number of frames
    uint nTracks = tfreq.n; // number of tracks in a frame
    for (uint t = 0; t < nTracks; ++t) {
        std::vector<int> trackBegs, trackEnds;
        if (tfreq(0,t) > BCC_M_DOUBLE_EPSILON)
            trackBegs.push_back(0);
        for (uint i = 1; i < nFrames-1; ++i) {
            if (tfreq(i-1,t) <= BCC_M_DOUBLE_EPSILON && tfreq(i,t) > BCC_M_DOUBLE_EPSILON)
                trackBegs.push_back(i);
            if (tfreq(i-1,t) > BCC_M_DOUBLE_EPSILON && tfreq(i,t) <= BCC_M_DOUBLE_EPSILON)
                trackEnds.push_back(i);
        }
        if (tfreq(nFrames-1,t) > BCC_M_DOUBLE_EPSILON)
            trackEnds.push_back(nFrames-1);

        for (uint i = 0; i < trackBegs.size(); ++i) {
            uint trackLen = 1 + trackEnds[i] - trackBegs[i];
            if (trackLen <= minTrackLength) {
                for (uint j = i; j < i + trackLen; ++j) {
                    tfreq(j,t) = 0;
                }
            }
        }
    }
}

} // namespace bacchus
