#include "fmincg.h"
#include "math/matmxn.h"
#include "math/matalg.h"

namespace bacchus {

// Minimize a continuous differentialble multivariate function. Starting point
// is given by "X" (D by 1), and the function named in the string "f", must
// return a function value and a vector of partial derivatives. The Polack-
// Ribiere flavour of conjugate gradients is used to compute search directions,
// and a line search using quadratic and cubic polynomial approximations and the
// Wolfe-Powell stopping criteria is used together with the slope ratio method
// for guessing initial step sizes. Additionally a bunch of checks are made to
// make sure that exploration is taking place and that extrapolation will not
// be unboundedly large. The "length" gives the length of the run: if it is
// positive, it gives the maximum number of line searches, if negative its
// absolute gives the maximum allowed number of function evaluations. You can
// (optionally) give "length" a second component, which will indicate the
// reduction in function value to be expected in the first line-search (defaults
// to 1.0). The function returns when either its length is up, or if no further
// progress can be made (ie, we are at a minimum, or so close that due to
// numerical problems, we cannot get any closer). If the function terminates
// within a few iterations, it could be an indication that the function value
// and derivatives are not consistent (ie, there may be a bug in the
// implementation of your "f" function). The function returns the found
// solution "X", a vector of function values "fX" indicating the progress made
// and "i" the number of iterations (line searches or function evaluations,
// depending on the sign of "length") used.
//
// Usage: [X, fX, i] = fmincg(f, X, options, P1, P2, P3, P4, P5)
//
// See also: checkgrad
//
// Copyright (C) 2001 and 2002 by Carl Edward Rasmussen. Date 2002-02-13
//
// [ml-class] Changes Made:
// 1) Function name and argument specifications
// 2) Output display
//

fmincg_res_t fmincg(fmincg_func_t func, const matxf& inX, fmincg_opt_t options) {
    assert(inX.n == 1);

    matxf X = inX;
    //uint N = X.size();

    int length = options.MaxIter;
    int red = options.red;

    float RHO = 0.01;   // a bunch of constants for line searches
    float SIG = 0.5;    // RHO and SIG are the constants in the Wolfe-Powell conditions
    float INT = 0.1;    // don't reevaluate within 0.1 of the limit of the current bracket
    float EXT = 3.0;    // extrapolate maximum 3 times the current bracket
    float RATIO = 100;  // maximum allowed slope ratio
    int MAX = 20;       // max 20 function evaluations per line search

    //int di = length < 0 ? 1 : 0;
    int di = 1;

    int i = 0; // zero the run length counter
    int ls_failed = 0; // no previous line search has failed
    std::vector<float> fX;

    matxf df1;//(N);
    float f1 = func(X, df1); // get function value and gradient

    i += di;
    matxf s = -df1;             // search direction is steepest
    float d1 = -inprod(s,s);    // this is the slope -transpose(s)*s is 1x1
    float z1 = red/(1.f - d1);  // initial step is red/(|s|+1): |s|==-d1

    while (i < std::abs(length)) { // while not finished
        i += di;

        // make a copy of current values
        matxf X0 = X;
        float f0 = f1;
        matxf df0 = df1;

        X = X + z1*s; // begin line search

        matxf df2;//(N);
        float f2 = func(X, df2); // get function value and gradient

        i += di;
        float d2 = inprod(df2,s);

        // initialize point 3 equal to point 1
        float f3 = f1;
        float d3 = d1;
        float z3 = -z1;

        int M;
        if (length>0) M = MAX;
        else M = std::min(MAX, -length-i);

        // initialize quanteties
        int success = 0;
        int limit = -1;
        while (1) {
            while (((f2 > f1+z1*RHO*d1) || (d2 > -SIG*d1)) && (M > 0.f)) {
                limit = z1; // tighten the bracket
                float z2;
                if (f2 > f1) {
                    z2 = z3 - (0.5f*d3*z3*z3)/(d3*z3+f2-f3); // quadratic fit
                } else {
                    float A = 6.f*(f2-f3)/z3+3.f*(d2+d3); // cubic fit
                    float B = 3.f*(f3-f2)-z3*(d3+2.f*d2);
                    z2 = (std::sqrt(B*B-A*d2*z3*z3)-B)/A; // numerical error possible - ok!
                }
                if (std::isnan(z2) || std::isinf(z2)) {
                    z2 = z3/2; // if we had a numerical problem then bisect
                }
                z2 = std::max(std::min(z2, INT*z3), (1.f - INT)*z3); // don't accept too close to limits
                z1 = z1 + z2; // update the step
                X = X + z2*s;
                f2 = func(X, df2);
                M = M - 1;
                i += di;
                d2 = inprod(df2,s);
                z3 = z3-z2; // z3 is now relative to the location of z2
            }

            if (f2 > f1+z1*RHO*d1 || d2 > -SIG*d1) {
                break; // this is a failure
            } else if (d2 > SIG*d1) {
                success = 1;
                break; // success
            } else if (M == 0) {
                break; // failure
            }

            // make cubic extrapolation
            float A = 6.f*(f2 - f3)/z3 + 3.f*(d2 + d3);
            float B = 3.f*(f3 - f2) - z3*(d3 + 2.f*d2);
            float z2 = -d2*z3*z3/(B + std::sqrt(B*B - A*d2*z3*z3)); // num. error possible - ok!
            if (std::isnan(z2) || std::isinf(z2) || z2 < 0) { // num prob or wrong sign?
                if (limit < -0.5f) { // if we have no upper limit
                    z2 = z1 * (EXT-1.f); // the extrapolate the maximum amount
                } else {
                    z2 = (limit-z1)/2.f; // otherwise bisect
                }
            } else if ((limit > -0.5f) && (z2+z1 > limit)) { // extraplation beyond max?
                z2 = (limit-z1)/2; // bisect
            } else if ((limit < -0.5f) && (z2+z1 > z1*EXT)) { // extrapolation beyond limit
                z2 = z1*(EXT-1.0); // set to extrapolation limit
            } else if (z2 < -z3*INT) {
                z2 = -z3*INT;
            } else if ((limit > -0.5f) && (z2 < (limit-z1)*(1.f-INT))) { // too close to limit?
                z2 = (limit-z1)*(1.0-INT);
            }

            // set point 3 equal to point 2
            f3 = f2; d3 = d2; z3 = -z2;

            // update current estimates
            z1 = z1 + z2;
            X = X + z2*s;
            f2 = func(X, df2);
            M = M - 1;
            i += di;
            d2 = inprod(df2,s);
        } // end of line search

        if (success) { // if line search succeeded
            f1 = f2;
            fX.push_back(f1);

            printf("Iteration %4i | Cost: %4.6e\r", i, f1);

            // Polack-Ribiere direction
            s = (inprod(df2,df2) - inprod(df1,df2))/inprod(df1,df1)*s - df2;

            //tmp = df1; df1 = df2; df2 = tmp;
            std::swap(df1, df2); // swap derivatives
            d2 = inprod(df1,s);
            if (d2 > 0) { // new slope must be negative
                s = -df1; // otherwise use steepest direction
                d2 = -inprod(s,s);
            }
            z1 = z1 * std::min(RATIO, d1/(d2 - 1.0e-7f)); // slope ratio but max RATI
            d1 = d2;
            ls_failed = 0; // this line search did not fail
        } else {
            X = X0; f1 = f0; df1 = df0; // restore point from before failed line search
            if (ls_failed || i > std::abs(length)) { // line search failed twice in a row
                break; // or we ran out of time, so we give up
            }
            //tmp = df1; df1 = df2; df2 = tmp;
            std::swap(df1, df2); // swap derivatives
            s = -df1; // try steepest
            d1 = -inprod(s,s);
            z1 = 1.f/(1.f - d1);
            ls_failed = 1; // this line search failed
        }

        //fflush(std::cout);

    }
    std::cout << std::endl;

    return {X, fX, i};
}

} // namespace bacchus
