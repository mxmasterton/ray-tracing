#pragma once

#include <cmath>
#include <iostream>

#include "interval.hpp"
#include "vector.hpp"

using Colour = Vector;

inline double LinearToGamma(double linear_component) {
    if (linear_component > 0) {
        return std::sqrt(linear_component);
    }

    return 0;
}

inline void WriteColour(std::ostream& out, const Colour& pixel_colour) {
    auto r = pixel_colour.x();
    auto g = pixel_colour.y();
    auto b = pixel_colour.z();

    // Apply a linear to gamma transform for gamma 2
    r = LinearToGamma(r);
    g = LinearToGamma(g);
    b = LinearToGamma(b);

    // Translate the [0,1] component values to the byte range [0,255]
    static const Interval intensity(0.000, 0.999);
    int rbyte = static_cast<int>(256 * intensity.Clamp(r));
    int gbyte = static_cast<int>(256 * intensity.Clamp(g));
    int bbyte = static_cast<int>(256 * intensity.Clamp(b));

    // Write out the pixel colour components
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
