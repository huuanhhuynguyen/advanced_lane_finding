#ifndef LANE_FINDING_ADVANCED_CPP_OFFSET_H
#define LANE_FINDING_ADVANCED_CPP_OFFSET_H

#include <vector>
#include "globals.h"
#include "birdeye.h"

constexpr float calculate_offset_in_meter(int pixelA, int pixelB)
{
    return float(pixelA - pixelB) * kx;
}

#endif //LANE_FINDING_ADVANCED_CPP_OFFSET_H
