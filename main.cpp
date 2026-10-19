#include "colour.hpp"
#include "vec3.hpp"

int main() {
    // Image
    constexpr int image_width = 256;
    constexpr int image_height = 256;

    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << " " << std::flush;

        for (int i = 0; i < image_width; ++i) {
            auto pixel_colour = colour(
                static_cast<double>(i) / (image_width - 1),
                static_cast<double>(j) / (image_height - 1),
                0.0
            );
            write_colour(std::cout, pixel_colour);
        }
    }

    std::clog << "\r\033[KDone!\n";
}
