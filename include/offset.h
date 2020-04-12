#ifndef LANE_FINDING_ADVANCED_CPP_OFFSET_H
#define LANE_FINDING_ADVANCED_CPP_OFFSET_H

#include <vector>
#include "birdeye.h"

constexpr float calculate_offset_in_meter(int pixelA, int pixelB)
{
    constexpr float xm_per_pix = 3.7 / 700; // meters per pixel in x dimension
    return (pixelA - pixelB) * xm_per_pix;
}

#endif //LANE_FINDING_ADVANCED_CPP_OFFSET_H
