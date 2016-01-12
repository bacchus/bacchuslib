#pragma once

#include "math/matmxn.h"

namespace bacchus {

void stftAnald(const matxd& x, const matxd& w, uint N, uint H, matxd& xmX, matxd& xpX);
matxd stftSynthd(const matxd& mY, const matxd& pY, uint M, uint H);

} // namespace bacchus
