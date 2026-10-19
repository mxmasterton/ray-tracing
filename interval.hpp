#pragma once

#include "rt.hpp"

class Interval {
public:
    Interval() : min_(+kInfinity), max_(-kInfinity) {}
    Interval(double min, double max) : min_(min), max_(max) {}

    double min() const { return min_; }
    double max() const { return max_; }

    double Size() const {
        return max_ - min_;
    }

    bool Contains(double x) const {
        return min_ <= x && x <= max_;
    }

    bool Surrounds(double x) const{
        return min_ < x && x < max_;
    }

    double Clamp(double x) const {
        if (x < min_) return min_;
        if (x > max_) return max_;
        return x;
    }

    static const Interval kEmpty;
    static const Interval kUniverse;

private:
    double min_;
    double max_;
};

inline const Interval Interval::kEmpty(+kInfinity, -kInfinity);
inline const Interval Interval::kUniverse(-kInfinity, +kInfinity);
