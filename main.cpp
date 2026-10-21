#include "camera.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"

int main() {
    HittableList world;
    world.Add(std::make_shared<Sphere>(Point(0, 0, -1), 0.5));
    world.Add(std::make_shared<Sphere>(Point(0, -100.5, -1), 100));

    Camera cam(16.0 / 9.0, 1000, 50, 20);
    cam.Render(world);
}
