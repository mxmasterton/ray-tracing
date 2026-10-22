#pragma once

#include <limits>
#include <random>

// Constants

const double kInfinity = std::numeric_limits<double>::infinity();
const double kPi = 3.1415926535897932385;

// Utility functions

inline double DegreesToRadians(double degrees) {
    return degrees * kPi / 180.0;
}

inline double RandomDouble() {
    // Returns a random real in [0,1)
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double RandomDouble(double min, double max) {
    // Returns a random real in [min,max)
    return min + (max - min) * RandomDouble();
}
