#ifndef LANE_FINDING_ADVANCED_CPP_COEFFICIENT_H
#define LANE_FINDING_ADVANCED_CPP_COEFFICIENT_H

#include "globals.h"

/// Represents coefficients of second order polynomial
struct Coefficient
{
    float a, b, c;  // coefficients

    explicit Coefficient() : a(0), b(0), c(0) {}

    explicit constexpr Coefficient(float a_, float b_, float c_)
        : a{a_}, b{b_}, c{c_} {}
};

Coefficient pixel_to_meter(Coefficient coeff)
{
    Coefficient result;
    result.a = coeff.a * kx / ky / ky;
    result.b = coeff.b * kx / ky;
    result.c = coeff.c * kx;
    return result;
}

#endif //LANE_FINDING_ADVANCED_CPP_COEFFICIENT_H
