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
    // Each rendering worker owns its PRNG, avoiding shared mutable state.
    thread_local std::mt19937 generator(std::random_device{}());
    thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}

inline double RandomDouble(double min, double max) {
    // Returns a random real in [min,max)
    return min + (max - min) * RandomDouble();
}
