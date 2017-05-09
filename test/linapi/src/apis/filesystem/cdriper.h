#pragma once

#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <linux/cdrom.h>

const int BUF_SIZE = 75*2352;

namespace cdriper {

struct wav_header {

    wav_header(int size) {
        memcpy(riff , (const void *) "RIFF", 4);
        memcpy(rifftype, (const void *) "WAVE", 4);
        memcpy(chunk_id1, (const void *) "fmt ", 4);
        chunksize1 = 16;
        wFormatTag = 1;  // WAVE_FORMAT_PCM;
        memcpy(chunk_id2, (const void *) "data", 4);
        nChannels = 2;
        nSamplesPerSec = 44100;
        nBlockAlign = 4;
        nAvgBytesPerSec = 44100 * nBlockAlign;
        wBitsPerSample = 16;
        chunksize2 = size*2352;
        filesize = chunksize2 + 44;
    }

    char riff[4];
    long filesize;
    char rifftype[4];
    char chunk_id1[4];
    long chunksize1;
    short wFormatTag;
    short nChannels;
    long nSamplesPerSec;
    long nAvgBytesPerSec;
    short nBlockAlign;
    short wBitsPerSample;
    char chunk_id2[4];
    long chunksize2;
};

int msf_to_frames(struct cdrom_msf0 * msf) {
    return (msf->minute * 60 + msf->second) * 75 + msf->frame;
}

void frames_to_msf(int frames, struct cdrom_msf0 * msf) {
    msf->frame = frames % 75;
    msf->second = (frames / 75) % 60;
    msf->minute = (frames / 75) / 60;
}

int cd_eject() {
    int cdd = open("/dev/cdrom", O_RDONLY);
    int drive_stat = ioctl(cdd, CDROM_DRIVE_STATUS, CDSL_CURRENT);

    if ((drive_stat != CDS_DISC_OK)
            && (drive_stat != CDS_NO_INFO)
            && (drive_stat != -1))
    {
        close(cdd);
        printf("device not ready\n");
        return 1;
    }

    ioctl(cdd, CDROMEJECT, 0);

    close(cdd);
    return 0;
}

int cd_read(int argc, char** argv) {

    if (argc != 3) {
        printf("usage: %s <track no> <audiofile>\n", argv[0]);
        return 1;
    }

    int track = atoi(argv[1]);
    char* audiofile = argv[2];

    /// opens cdrom for read
    int cdd = open("/dev/cdrom", O_RDONLY);
    int disc_stat = ioctl(cdd, CDROM_DRIVE_STATUS, CDSL_CURRENT);

    if ((disc_stat != CDS_DISC_OK)
            && (disc_stat != CDS_NO_INFO)
            && (disc_stat != -1))
    {
        close(cdd);
        printf("device not ready\n");
        return 1;
    }

    struct cdrom_tochdr toc;
    ioctl(cdd, CDROMREADTOCHDR, &toc);

    if ((track < toc.cdth_trk0)
            || (track > toc.cdth_trk1))
    {
        close(cdd);
        printf("wrong track no\n");
        return 1;
    }

    struct cdrom_tocentry entry;
    entry.cdte_format = CDROM_MSF;
    entry.cdte_track = track;
    ioctl(cdd, CDROMREADTOCENTRY, &entry);

    if ((entry.cdte_ctrl & CDROM_DATA_TRACK) != 0)
    {
        close(cdd);
        printf("not an audio track\n");
        return 1;
    }

    int start = msf_to_frames(&entry.cdte_addr.msf);
    entry.cdte_track = (track < toc.cdth_trk1) ? track + 1 : CDROM_LEADOUT;
    ioctl(cdd, CDROMREADTOCENTRY, &entry);
    int stop = msf_to_frames(&entry.cdte_addr.msf);

    wav_header hdr(stop-start);
    int fd = open(audiofile, O_WRONLY|O_CREAT, 0777);
    write(fd, &hdr, sizeof(hdr));

    char buf[BUF_SIZE];
    struct cdrom_read_audio rdaudio;
    rdaudio.addr_format = CDROM_MSF;
    rdaudio.buf = (__u8*)buf;
    while (start < stop) {
        frames_to_msf(start, &rdaudio.addr.msf);
        start += (rdaudio.nframes = (stop - start) > 75 ? 75 : stop - start);
        ioctl(cdd, CDROMREADAUDIO, &rdaudio);
        write(fd, buf, rdaudio.nframes*2352);
    }

    close(cdd);
    close(fd);
    return 0;
}

} // namespace cdriper
