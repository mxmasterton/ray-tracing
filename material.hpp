#pragma once

#include "colour.hpp"
#include "hittable.hpp"
#include "ray.hpp"
#include "vector.hpp"

class Material {
public:
    virtual ~Material() = default;

    virtual bool Scatter(
        const Ray& r, const HitRecord& rec, Colour& attenuation, Ray& scattered
    ) const {
        return false;
    }
};

class Lambertian : public Material {
public:
    Lambertian(const Colour& albedo) : albedo_(albedo) {}

    bool Scatter(
        const Ray& r, const HitRecord& rec, Colour& attenuation, Ray& scattered
    ) const override {
        auto scatter_direction = rec.normal + RandomUnitVector();

        // Catch degenerate scatter direciton
        if (scatter_direction.NearZero()) {
            scatter_direction = rec.normal;
        }

        scattered = Ray(rec.p, scatter_direction);
        attenuation = albedo_;
        return true;
    }

private:
    Colour albedo_;
};

class Metal : public Material {
public:
    Metal(const Colour& albedo, double fuzz) : albedo_(albedo), fuzz_(fuzz < 1 ? fuzz : 1) {}

    bool Scatter(
        const Ray& r_in, const HitRecord& rec, Colour& attenuation, Ray& scattered
    ) const override {
        Vector reflected = Reflect(r_in.direction(), rec.normal);
        attenuation = albedo_;
        while (true) {
            reflected = UnitVector(reflected) + (fuzz_ * RandomUnitVector());
            scattered = Ray(rec.p, reflected);
            if (Dot(scattered.direction(), rec.normal) > 0) {
                return true;
            }
        }
    }

private:
    Colour albedo_;
    double fuzz_;
};
