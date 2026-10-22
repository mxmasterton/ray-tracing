#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>

#include "rt.hpp"

class Vector {
public:
    Vector() : coords_{0, 0, 0} {}
    Vector(double x, double y, double z) : coords_{x, y, z} {}

    double x() const { return coords_[0]; }
    double y() const { return coords_[1]; }
    double z() const { return coords_[2]; }

    Vector operator-() const {
        return Vector(-coords_[0], -coords_[1], -coords_[2]);
    }

    double operator[](int i) const {
        return coords_[i];
    }

    double& operator[](int i) {
        return coords_[i];
    }

    Vector& operator+=(const Vector& v) {
        coords_[0] += v.coords_[0];
        coords_[1] += v.coords_[1];
        coords_[2] += v.coords_[2];
        return *this;
    }

    Vector& operator*=(double t) {
        coords_[0] *= t;
        coords_[1] *= t;
        coords_[2] *= t;
        return *this;
    }

    Vector& operator/=(double t) {
        return *this *= 1/t;
    }

    double Length() const {
        return std::sqrt(LengthSquared());
    }

    double LengthSquared() const {
        return coords_[0] * coords_[0] + coords_[1] * coords_[1] + coords_[2] * coords_[2];
    }

    static Vector Random() {
        return Vector(RandomDouble(), RandomDouble(), RandomDouble());
    }

    static Vector Random(double min, double max) {
        return Vector(RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max));
    }

    bool NearZero() const {
        auto s = 1e-8;
        return (std::fabs(coords_[0]) < s) && (std::fabs(coords_[1]) < s)
            && (std::fabs(coords_[2]) < s);
    }

private:
    double coords_[3];
};

using Point = Vector;

inline std::ostream& operator<<(std::ostream& out, const Vector& v) {
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline Vector operator+(const Vector& u, const Vector& v) {
    return Vector(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

inline Vector operator-(const Vector& u, const Vector& v) {
    return Vector(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

inline Vector operator*(const Vector& u, const Vector& v) {
    return Vector(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

inline Vector operator*(double t, const Vector& v) {
    return Vector(t * v.x(), t * v.y(), t * v.z());
}

inline Vector operator*(const Vector& v, double t) {
    return t * v;
}

inline Vector operator/(const Vector& v, double t) {
    return (1.0/t) * v;
}

inline double Dot(const Vector& u, const Vector& v) {
    return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

inline Vector Cross(const Vector& u, const Vector& v) {
    return Vector(u.y() * v.z() - u.z() * v.y(),
        u.z() * v.x() - u.x() * v.z(),
        u.x() * v.y() - u.y() * v.x());
}

inline Vector UnitVector(const Vector& v) {
    return v / v.Length();
}

inline Vector RandomUnitVector() {
    // Why can't we just take Vector::Random(-1,1) and scale even if its too big?
    // Because we would be projecting a uniform distribution from a cube onto a square, and a
    // disproportionately large number of random vectors would get pulled towards the diagonal
    // corners.
    while (true) {
        auto p = Vector::Random(-1, 1);
        auto lensq = p.LengthSquared();
        if (1e-160 <= lensq && lensq <= 1) {
            return p / std::sqrt(lensq);
        }
    }
}

inline Vector RandomOnHemisphere(const Vector& normal) {
    Vector on_unit_sphere = RandomUnitVector();
    if (Dot(on_unit_sphere, normal) > 0.0) { // In the same hemisphere as normal
        return on_unit_sphere;
    } else {
        return -on_unit_sphere;
    }
}

inline Vector Reflect(const Vector& v, const Vector& n) {
    return v - 2*Dot(v, n) * n;
}

inline Vector Refract(const Vector& uv, const Vector& n, double etai_over_etat) {
    auto cos_theta = std::min(Dot(-uv, n), 1.0);
    Vector r_out_perp = etai_over_etat * (uv + cos_theta*n);
    Vector r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.LengthSquared())) * n;
    return r_out_perp + r_out_parallel;
}
