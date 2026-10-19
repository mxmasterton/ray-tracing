#pragma once

#include <cmath> // IWYU pragma: export
#include <iostream> // IWYU pragma: export
#include <limits> // IWYU pragma: export
#include <memory> // IWYU pragma: export

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility functions
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// Common headers

#include "colour.hpp" // IWYU pragma: export
#include "interval.hpp" // IWYU pragma: export
#include "ray.hpp" // IWYU pragma: export
#include "vec3.hpp" // IWYU pragma: export
