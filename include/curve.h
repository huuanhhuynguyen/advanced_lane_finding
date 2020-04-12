#ifndef LANE_FINDING_ADVANCED_CPP_CURVE_H
#define LANE_FINDING_ADVANCED_CPP_CURVE_H

struct SecondPolynomial {
    float a, b, c;  // coefficients
    explicit SecondPolynomial() : a(0), b(0), c(0) {}
    explicit constexpr SecondPolynomial(float a_, float b_, float c_)
        : a{a_}, b{b_}, c{c_} {}
};

#endif //LANE_FINDING_ADVANCED_CPP_CURVE_H
