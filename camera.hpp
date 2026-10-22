#pragma once

#include <iostream>

#include "colour.hpp"
#include "hittable.hpp"
#include "material.hpp" // IWYU pragma: keep
#include "ray.hpp"
#include "utility.hpp"
#include "vector.hpp"

class Camera {
public:
    Camera(double aspect_ratio, int image_width, int samples_per_pixel, int max_depth) :
        aspect_ratio_(aspect_ratio), image_width_(image_width),
        samples_per_pixel_(samples_per_pixel), max_depth_(max_depth) {
        image_height_ = static_cast<int>(image_width / aspect_ratio);
        image_height_ = (image_height_ < 1) ? 1 : image_height_;

        pixel_samples_scale_ = 1.0 / samples_per_pixel;

        centre_ = Point(0, 0, 0);

        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * static_cast<double>(image_width) / image_height_;

        auto viewport_u = Vector(viewport_width, 0, 0);
        auto viewport_v = Vector(0, -viewport_height, 0);

        pixel_delta_u_ = viewport_u / image_width_;
        pixel_delta_v_ = viewport_v / image_height_;

        auto viewport_upper_left = centre_
            - Vector(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
        pixel00_loc_ = viewport_upper_left + 0.5 * (pixel_delta_u_ + pixel_delta_v_);
    }

    void Render(const Hittable& world) {
        std::cout << "P3\n" << image_width_ << ' ' << image_height_ << "\n255\n";

        for (int j = 0; j < image_height_; ++j) {
            std::clog << "Scanlines remaining: " << (image_height_ - j) << '\n';

            for (int i = 0; i < image_width_; ++i) {
                Colour pixel_colour(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel_; ++sample) {
                    Ray r = GetRay(i, j);
                    pixel_colour += RayColour(r, 1, world);
                }
                WriteColour(std::cout, pixel_colour * pixel_samples_scale_);
            }
        }

        std::clog << "Done!" << '\n';
    }

private:
    int image_width_;
    int image_height_;
    int samples_per_pixel_;
    int max_depth_;
    double aspect_ratio_;
    double pixel_samples_scale_;
    Point centre_;
    Point pixel00_loc_;
    Vector pixel_delta_u_;
    Vector pixel_delta_v_;

    Ray GetRay(int i, int j) const {
        // Construct a camera ray originating from the origin and directed at a random
        // poiint around the pixel location (i, j)
        auto offset = SampleSquare();
        auto pixel_sample = pixel00_loc_
            + ((i + offset.x()) * pixel_delta_u_)
            + ((j + offset.y()) * pixel_delta_v_);

        auto ray_origin = centre_;
        auto ray_direction = pixel_sample - ray_origin;

        return Ray(ray_origin, ray_direction);
    }

    Vector SampleSquare() const {
        // Returns the vector to a random point in the [-.5, -.5] - [+.5, +.5] unit square
        return Vector(RandomDouble() - 0.5, RandomDouble() - 0.5, 0.0);
    }

    Colour RayColour(const Ray& r, int depth, const Hittable& world) const {
        if (depth > max_depth_) {
            // If we've exceeded the ray bounce limit, no more light can be gathered
            return Colour(0, 0, 0);
        }

        HitRecord rec;

        if (world.Hit(r, Interval(0.001, kInfinity), rec)) {
            Ray scattered;
            Colour attenuation;
            if (rec.mat->Scatter(r, rec, attenuation, scattered)) {
                return attenuation * RayColour(scattered, depth + 1, world);
            } else {
                return Colour(0, 0, 0);
            }
        }

        // Background colouring
        Vector unit_direction = UnitVector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * Colour(1.0, 1.0, 1.0) + a * Colour(0.5, 0.7, 1.0);
    }
};
