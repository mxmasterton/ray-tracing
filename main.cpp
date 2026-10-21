#include <memory>

#include "camera.hpp"
#include "colour.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "sphere.hpp"

using std::make_shared;

int main() {
    HittableList world;

    auto material_ground = make_shared<Lambertian>(Colour(0.8, 0.8, 0.0));
    auto material_centre = make_shared<Lambertian>(Colour(0.1, 0.2, 0.5));
    auto material_left = make_shared<Metal>(Colour(0.8, 0.8, 0.8));
    auto material_right = make_shared<Metal>(Colour(0.8, 0.6, 0.2));

    world.Add(make_shared<Sphere>(Point(0.0, -100.5, -1.0), 100.0, material_ground));
    world.Add(make_shared<Sphere>(Point(0.0, 0.0, -1.2), 0.5, material_centre));
    world.Add(make_shared<Sphere>(Point(-1.0, 0.0, -1.0), 0.5, material_left));
    world.Add(make_shared<Sphere>(Point(1.0, 0.0, -1.0), 0.5, material_right));

    Camera cam(16.0 / 9.0, 1000, 50, 10);
    cam.Render(world);
}
