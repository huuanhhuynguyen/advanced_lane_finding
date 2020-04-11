#ifndef LANE_FINDING_ADVANCED_CPP_OFFSET_H
#define LANE_FINDING_ADVANCED_CPP_OFFSET_H

#include <vector>

float calculate_offset_from_center(int vehicle_y,
                                   const std::vector<float>& coeff_left,
                                   const std::vector<float>& coeff_right)
{
    int bottom_left_x = coeff_left[0];  // y = 0
    int bottom_right_x = coeff_right[0];

    // pixel position of lane center
    int bottom_mid_x = int((bottom_left_x + bottom_right_x) / 2);

    int offset_pixel = vehicle_y - bottom_mid_x;

    constexpr float kX = 3.7 / 700.0;  // meters per pixel in x dimension
    return kX * float(offset_pixel);
}

#endif //LANE_FINDING_ADVANCED_CPP_OFFSET_H
