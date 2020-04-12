#ifndef LANE_FINDING_ADVANCED_CPP_OFFSET_H
#define LANE_FINDING_ADVANCED_CPP_OFFSET_H

#include <vector>
#include "birdeye.h"

/// Calculate vehicle position offset from the lane center [in meters]
/// TODO document function arguments
float calculate_offset_from_center(int vehicle_y,
                                   int bottom_left_x, int bottom_right_x)
{
    // pixel position of lane center
    int bottom_mid_x = int((bottom_left_x + bottom_right_x) / 2);

    int offset_pixel = vehicle_y - bottom_mid_x;

    // meters per pixel in x dimension
    constexpr float xm_per_pix = 3.7 / 700;

    return xm_per_pix * float(offset_pixel);
}

#endif //LANE_FINDING_ADVANCED_CPP_OFFSET_H
