#pragma once

#include "vector.hpp"

class Ray {
public:
    Ray(const Point& origin, const Vector& direction) : origin_(origin), direction_(direction) {}

    const Point& origin() const { return origin_; }
    const Vector& direction() const { return direction_; }

    Point At(double t) const {
        return origin_ + t * direction_;
    }

private:
    Point origin_;
    Vector direction_;
};
