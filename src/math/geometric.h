#pragma once

#include "vec2.h"

namespace bacchus {

// p0-p1, p1-p2
inline float direction(const vec2f& p0, const vec2f& p1, const vec2f& p2) {
    return cross(p2 - p0, p1 - p0);
}

inline bool onsegment(const vec2f& p0, const vec2f& p1, const vec2f& p) {
    vec2f minv = min(p0, p1);
    vec2f maxv = max(p0, p1);
    return (p.x >= minv.x && p.x <= maxv.x &&
            p.y >= minv.y && p.y <= maxv.y );
}

// p1-p2, p3-p4
inline bool segments_intersect(const vec2f& p1, const vec2f& p2,
                               const vec2f& p3, const vec2f& p4 )
{
    float d1 = direction(p3,p4,p1);
    float d2 = direction(p3,p4,p2);
    float d3 = direction(p1,p2,p3);
    float d4 = direction(p1,p2,p4);

    if (   (d1 > 0.f && d2 < 0.f)
        || (d1 < 0.f && d2 > 0.f)
        || (d3 > 0.f && d4 < 0.f)
        || (d3 < 0.f && d4 > 0.f) )
    {
        return true;
    }
    else if (d1==0.f && onsegment(p3,p4,p1)) return true;
    else if (d2==0.f && onsegment(p3,p4,p2)) return true;
    else if (d3==0.f && onsegment(p1,p2,p3)) return true;
    else if (d4==0.f && onsegment(p1,p2,p4)) return true;
    else return false;
}

} // namespace bacchus
