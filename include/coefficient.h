#ifndef LANE_FINDING_ADVANCED_CPP_COEFFICIENT_H
#define LANE_FINDING_ADVANCED_CPP_COEFFICIENT_H

/// Represents coefficients of second order polynomial
struct Coefficient {
    float a, b, c;  // coefficients
    explicit Coefficient() : a(0), b(0), c(0) {}
    explicit constexpr Coefficient(float a_, float b_, float c_)
        : a{a_}, b{b_}, c{c_} {}
};

#endif //LANE_FINDING_ADVANCED_CPP_COEFFICIENT_H
