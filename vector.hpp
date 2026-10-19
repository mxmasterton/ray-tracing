#pragma once

#include <cmath>
#include <iostream>

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
