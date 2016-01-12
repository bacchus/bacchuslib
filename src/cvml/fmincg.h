#pragma once
#include <functional>
#include "math/matmxn.h"

namespace bacchus {

//options = optimset('GradObj', 'on', 'MaxIter', 50);
struct fmincg_opt_t {
    int MaxIter = 100;
    int red = 1;

    fmincg_opt_t(int MaxIter = 100, int red = 1)
        : MaxIter(MaxIter), red(red)
    {}
};

//function [X, fX, i] =
struct fmincg_res_t {
    matxf X;
    std::vector<float> fX;
    int i;

    //fmincg_res_t(uint n): X(n), i(0) {}
};

//[f1 df1] = eval(argstr);
struct fmincg_funcres_t {
    float f1;
    matxf df1;
};

//typedef float(*fmincg_func_t)(const matxf&, matxf&);
typedef std::function<float(const matxf&, matxf&)> fmincg_func_t;

fmincg_res_t fmincg(fmincg_func_t func, const matxf& inX, fmincg_opt_t options/*, P1, P2, P3, P4, P5*/);

} // namespace bacchus
