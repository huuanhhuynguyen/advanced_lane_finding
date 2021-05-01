#ifndef BASIC_LANE_FINDING_CPP_SMOOTHEN_H
#define BASIC_LANE_FINDING_CPP_SMOOTHEN_H

#include <deque>
#include <numeric>
#include "coefficient.h"

/// Calculates the moving average value.
/// Example:
///     MovAvg<float> mov_avg{3}; // window size = 3
///     mov_avg.update(3);  // returns 3
///     mov_avg.update(5);  // returns (3+5)/2 = 4
///     mov_avg.update(10); // returns (3+5+10)/3 = 6
///     mov_avg.update(-2); // returns (5+10-2)/3 = 4.333
template <typename T>
class MovAvg {
public:
    explicit constexpr MovAvg(unsigned int window_ = 5) : window{window_} {}

    /// update history and return the moving-averaged value
    constexpr T update(const T& number) const noexcept
    {
        if (prevs.size() >= window)
        {
            prevs.pop_front();
        }
        prevs.push_back(number);
        const auto sum = std::accumulate(prevs.begin(), prevs.end(), 0.0);
        const auto mean = sum / prevs.size();
        return static_cast<T>(mean);
    }

private:
    mutable std::deque<T> prevs;
    const unsigned int window;
};

/// Calculates moving-averaged line (i.e. moving averaged slope and bias).
/// Example:
///    CoeffMoveAvg mov_avg{2};  // window size = 2
///    Coefficient coeff(0.5, 2, 1);      // a = 0.5, b = 2, c = 1
///    mov_avg.update(coeff);   // returns a coefficient instance with a = 0.5, b = 2.0, c = 1.0
///    mov_avg.update(Coefficient(-0.5, 3, 1)); // returns a = 0.0, b = 2.5, c = 1.0
///    mov_avg.update(Coefficient(2.5, -5, 3)); // returns a = 1.0, b = -1.0, c = 2.0
class CoeffMoveAvg {
public:
    explicit constexpr CoeffMoveAvg(unsigned int window_ = 5) :
            first{window_}, second{window_}, third{window_}, window{window_} {}

    /// update history and return the moving-averaged line
    Coefficient update(const Coefficient& coeff) const noexcept
    {
        const auto a = first.update(coeff.a);
        const auto b = second.update(coeff.b);
        const auto c = third.update(coeff.c);
        return Coefficient(a, b, c);
    }

private:
    MovAvg<float> first;
    MovAvg<float> second;
    MovAvg<float> third;
    const unsigned int window;
};

#endif //BASIC_LANE_FINDING_CPP_SMOOTHEN_H