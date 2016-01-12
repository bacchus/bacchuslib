#pragma once

#include <vector>

#include "matmxn.h"
#include "mdefs.h"

namespace bacchus {

static const int dbg = 0;

void print_equatoins(
        const std::vector<int>& N
        , const std::vector<int>& B
        , const matxf& A
        , const std::vector<float>& b
        , const std::vector<float>& c
        , float v
        );

void print_equatoins(
        const matxf& A
        , const std::vector<float>& b
        , const std::vector<float>& c
        );

int find_min(const std::vector<float>& vec);

int positive_index(const std::vector<float>& c);

int exist_in(int x, const std::vector<int>& vec);

void pivot(
        std::vector<int>& N
        , std::vector<int>& B
        , matxf& A
        , std::vector<float>& b
        , std::vector<float>& c
        , float& v
        , int l, int e // l - base; e - nonbase
        );

int init_simplex(
        std::vector<int>& N
        , std::vector<int>& B
        , matxf& A
        , std::vector<float>& b
        , std::vector<float>& c
        , float& v
        );

int simplex(
        const matxf& inA
        , const std::vector<float>& inb
        , const std::vector<float>& inc
        , std::vector<float>& outx);

} // namespace bacchus
