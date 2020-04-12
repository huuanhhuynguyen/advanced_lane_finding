#ifndef LANE_FINDING_ADVANCED_CPP_RADIUS_H
#define LANE_FINDING_ADVANCED_CPP_RADIUS_H

#include <vector>
#include "coefficient.h"

float calculate_curve_radius(const Coefficient& coeff)
{
    Coefficient coeff_m = pixel_to_meter(coeff);

    float y_eval = 0;
    float dxdy = 2 * coeff_m.a * y_eval + coeff_m.b;  // 1st derivative
    float d2xdy2 = 2 * coeff_m.a;  // 2nd derivative

    // source of the formula: https://medium.com/@mithi/advanced-lane-finding-using-computer-vision-techniques-7f3230b6c6f2
    return std::pow(1 + dxdy * dxdy, 1.5) / std::abs(d2xdy2);
}

#endif //LANE_FINDING_ADVANCED_CPP_RADIUS_H
