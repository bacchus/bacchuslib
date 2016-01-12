#pragma once

#include "math/matmxn.h"

namespace bacchus {

void dftAnald(const matxd& x, const matxd& w, uint N, matxd& mX, matxd& pX);
matxd dftSynthd(const matxd& mX, const matxd& pX, uint M);

} // namespace bacchus
