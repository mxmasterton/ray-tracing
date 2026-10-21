#pragma once

#include <cmath>
#include <memory>

#include "hittable.hpp"

class Sphere : public Hittable {
public:
    Sphere(const Point& centre, double radius, std::shared_ptr<Material> mat) :
        centre_(centre), radius_(std::fmax(0, radius)), mat_(mat) {}

    bool Hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
        Vector oc = centre_ - r.origin();
        auto a = r.direction().LengthSquared();
        auto h = Dot(r.direction(), oc);
        auto c = oc.LengthSquared() - radius_*radius_;

        auto discriminant = h*h - a*c;
        if (discriminant < 0) {
            return false;
        }

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range
        auto root = (h - sqrtd) / a;
        if (!ray_t.Surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!ray_t.Surrounds(root)) {
                return false;
            }
        }

        rec.t = root;
        rec.p = r.At(rec.t);
        Vector outward_normal = (rec.p - centre_) / radius_;
        rec.SetFaceNormal(r, outward_normal);
        rec.mat = mat_;

        return true;
    }

private:
    Point centre_;
    double radius_;
    std::shared_ptr<Material> mat_;
};
