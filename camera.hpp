#pragma once

#include <algorithm>
#include <atomic>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>

#include "stb_image_write.h"

#include "colour.hpp"
#include "hittable.hpp"
#include "material.hpp" // IWYU pragma: keep
#include "ray.hpp"
#include "utility.hpp"
#include "vector.hpp"

class Camera {
public:
    Camera(
        double aspect_ratio, int image_width, int samples_per_pixel, int max_depth, double fov,
        Point look_from, Point look_to, Vector up
    ) : aspect_ratio_(aspect_ratio), image_width_(image_width),
        samples_per_pixel_(samples_per_pixel), max_depth_(max_depth), fov_(fov),
        look_from_(look_from), look_to_(look_to), up_(up) {
        image_height_ = static_cast<int>(image_width / aspect_ratio);
        image_height_ = (image_height_ < 1) ? 1 : image_height_;

        pixel_samples_scale_ = 1.0 / samples_per_pixel;

        centre_ = look_from_;

        // Determine viewport directions
        auto focal_length = (look_from_ - look_to_).Length();
        auto theta = DegreesToRadians(fov_);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2 * h * focal_length;
        auto viewport_width = viewport_height * static_cast<double>(image_width) / image_height_;

        // Calculate the u, v, w unit basis vectors for the camera coordiante frame
        w_ = UnitVector(look_from_ - look_to_);
        u_ = UnitVector(Cross(up_, w_));
        v_ = Cross(w_, u_);

        // Calculate the vectors across the horizontal and down the vertical viewport edges
        Vector viewport_u = viewport_width * u_;
        Vector viewport_v = viewport_height * -v_;

        // Calculate the horizontal and vertical delta vectors from pixel to pixel
        pixel_delta_u_ = viewport_u / image_width_;
        pixel_delta_v_ = viewport_v / image_height_;

        // Calculate the location of the upper left pixel
        auto viewport_upper_left = centre_
            - (focal_length * w_) - viewport_u/2 - viewport_v/2;
        pixel00_loc_ = viewport_upper_left + 0.5 * (pixel_delta_u_ + pixel_delta_v_);
    }

    void Render(const Hittable& world) {
        const auto pixel_count = static_cast<std::size_t>(image_width_) * image_height_;
        std::vector<Colour> pixels(pixel_count);
        std::atomic<std::size_t> next_pixel = 0;

        const auto worker_count = std::max(1u, std::thread::hardware_concurrency());
        std::vector<std::thread> workers;
        workers.reserve(worker_count);

        for (unsigned int worker = 0; worker < worker_count; ++worker) {
            workers.emplace_back([this, &world, &pixels, &next_pixel, pixel_count] {
                while (true) {
                    const auto pixel_index = next_pixel.fetch_add(1);
                    if (pixel_index >= pixel_count) {
                        break;
                    }

                    const int i = static_cast<int>(pixel_index % image_width_);
                    const int j = static_cast<int>(pixel_index / image_width_);

                    Colour pixel_colour(0, 0, 0);
                    for (int sample = 0; sample < samples_per_pixel_; ++sample) {
                        pixel_colour += RayColour(GetRay(i, j), 1, world);
                    }

                    // Every worker receives a unique index, so no pixel is written concurrently.
                    pixels[pixel_index] = pixel_colour * pixel_samples_scale_;
                }
            });
        }

        for (auto& worker : workers) {
            worker.join();
        }

        std::vector<unsigned char> image_data(pixel_count * 3);
        for (std::size_t pixel_index = 0; pixel_index < pixel_count; ++pixel_index) {
            const auto rgb = ToRGB(pixels[pixel_index]);
            const auto byte_index = pixel_index * 3;
            image_data[byte_index] = rgb[0];
            image_data[byte_index + 1] = rgb[1];
            image_data[byte_index + 2] = rgb[2];
        }

        if (!stbi_write_png(
                "image.png", image_width_, image_height_, 3, image_data.data(), image_width_ * 3
            )) {
            throw std::runtime_error("Failed to write image.png");
        }

        std::clog << "Wrote image.png" << '\n';
    }

private:
    int image_width_;
    int samples_per_pixel_;
    int max_depth_;
    double aspect_ratio_;
    double fov_;
    Point look_from_ = Point(0, 0, 0);
    Point look_to_ = Point(0, 0, -1);
    Vector up_ = Vector(0, 1, 0);

    int image_height_;
    double pixel_samples_scale_;
    Point centre_;
    Point pixel00_loc_;
    Vector pixel_delta_u_;
    Vector pixel_delta_v_;
    Vector u_, v_, w_;

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
