#pragma once

#include <array>
#include <cmath>
#include <cstdint>
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

inline std::array<std::uint8_t, 3> ToRGB(const Colour& pixel_colour) {
    // Apply a linear to gamma transform for gamma 2.
    const auto r = LinearToGamma(pixel_colour.x());
    const auto g = LinearToGamma(pixel_colour.y());
    const auto b = LinearToGamma(pixel_colour.z());

    // Translate the [0,1] component values to the byte range [0,255].
    static const Interval intensity(0.000, 0.999);
    return {
        static_cast<std::uint8_t>(256 * intensity.Clamp(r)),
        static_cast<std::uint8_t>(256 * intensity.Clamp(g)),
        static_cast<std::uint8_t>(256 * intensity.Clamp(b)),
    };
}

inline void WriteColour(std::ostream& out, const Colour& pixel_colour) {
    const auto rgb = ToRGB(pixel_colour);
    out << static_cast<int>(rgb[0]) << ' '
        << static_cast<int>(rgb[1]) << ' '
        << static_cast<int>(rgb[2]) << '\n';
}
