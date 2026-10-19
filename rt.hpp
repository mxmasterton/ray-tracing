#pragma once

#include <cmath> // IWYU pragma: export
#include <iostream> // IWYU pragma: export
#include <limits>
#include <random>
#include <memory> // IWYU pragma: export

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min, max)
    return min + (max - min) * random_double();
}

// Common headers

#include "colour.hpp" // IWYU pragma: export
#include "interval.hpp" // IWYU pragma: export
#include "ray.hpp" // IWYU pragma: export
#include "vec3.hpp" // IWYU pragma: export
