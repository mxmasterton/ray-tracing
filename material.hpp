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

class Dielectric : public Material {
public:
    Dielectric(double refractive_index) : refractive_index_(refractive_index) {}

    bool Scatter(
        const Ray& r_in, const HitRecord& rec, Colour& attenuation, Ray& scattered
    ) const override {
        attenuation = Colour(1.0, 1.0, 1.0);
        double ri = rec.front_face ? (1.0 / refractive_index_) : refractive_index_;

        Vector unit_direction = UnitVector(r_in.direction());
        double cos_theta = std::fmin(Dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = ri * sin_theta > 1.0;
        Vector direction;

        if (cannot_refract || Reflectance(cos_theta, ri) > RandomDouble()) {
            direction = Reflect(unit_direction, rec.normal);
        } else {
            direction = Refract(unit_direction, rec.normal, ri);
        }

        scattered = Ray(rec.p, direction);
        return true;
    }

private:
    double refractive_index_;

    static double Reflectance(double cosine, double refractive_index) {
        // Schlick's approximation:
        // https://en.wikipedia.org/wiki/Schlick%27s_approximation
        auto r0 = (1 - refractive_index) / (1 + refractive_index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};
