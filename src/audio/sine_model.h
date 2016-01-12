#pragma once

#include "math/matmxn.h"

namespace bacchus {

void sineAnald(
        const matxd& x, double fs, const matxd& w, uint N, uint H, double t
        , uint maxnSines // = 100
        , double minSineDur // = 0.01
        , double freqDevOffset // = 20
        , double freqDevSlope // = 0.01
        , matxd& xtfreq, matxd& xtmag, matxd& xtphase);

matxd sineSynthd(
        const matxd& xtfreq, const matxd& xtmag, const matxd& xtphase
        , uint N, uint H, double fs);

} // namespace bacchus
