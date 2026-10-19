#pragma once

#include "interval.hpp"
#include "ray.hpp"
#include "vector.hpp"

struct HitRecord {
    Point p;
    Vector normal;
    double t;
    bool front_face;

    void SetFaceNormal(const Ray& r, const Vector& outward_normal) {
        // Sets the HitRecord normal vector
        // Note: the parameter 'outward_normal' is assumed to have unit length
        front_face = Dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool Hit(const Ray& r, Interval ray_t, HitRecord& rec) const = 0;
};
