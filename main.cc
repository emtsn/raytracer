#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "triangle.h"
#include "cube.h"
#include "torus.h"
#include "camera.h"
#include "material.h"
#include "light.h"
#include "blinn.h"

#include <iostream>
#include <time.h>

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            auto choose_shape = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> shape_material;

                if (choose_mat < 0.80) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    shape_material = make_shared<lambertian>(albedo);
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    shape_material = make_shared<metal>(albedo, fuzz);
                } else {
                    // glass
                    shape_material = make_shared<dielectric>(1.5);
                }
                if (choose_shape < 0.6) {
                    world.add(make_shared<sphere>(center, 0.2, shape_material));
                } else if (choose_shape < 0.8) {
                    world.add(make_shared<cube>(center, 0.4, shape_material));
                } else {
                    world.add(make_shared<torus>(center, vec3(0, 1, 0), 0.15, 0.05, shape_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

color ray_color(const ray& r, const blinn& blinn, const hittable& world, const light& lights, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0) {
        return color(0, 0, 0);
    }

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;

        if (rec.mat_ptr->scatter(r, world, rec, attenuation, scattered)) {
            return blinn.shade(attenuation, r.orig, lights, world, rec) * ray_color(scattered, blinn, world, lights, depth-1);
        }
        return color(0, 0, 0);
    }
    vec3 unit_direction = unit_vector(r.direction()); // sets -1 <= y <= 1
    auto t = 0.5 * (unit_direction.y() + 1.0); // scales y so that 0 <= t <= 1
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0); // lerp between white (t=0) -> blue (t=1)
}

int main() {

    // // Image
    // const auto aspect_ratio = 16.0 / 9.0;
    // const int image_width = 400;
    // const int image_height = static_cast<int>(image_width / aspect_ratio);
    // const int samples_per_pixel = 50;
    // const int max_depth = 10;

    // // World
    // hittable_list world;
    // blinn blinn(color(0.2, 0.2, 0.2), color(1.0, 1.0, 1.0), 1.0, 1.0, 1.0, 40.0);

    // auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    // auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    // auto material_left   = make_shared<dielectric>(1.5);
    // auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);
    // auto normal_material = make_shared<normalmat>();
    // auto red_material = make_shared<lambertian>(color(0.5, 0.2, 0.1));
    // auto green_material = make_shared<lambertian>(color(0.2, 0.5, 0.1));

    // world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));

    // // world.add(make_shared<cube>(point3(0.0, 1.0, -1.0),   1, material_left));
    // // world.add(make_shared<triangle>(point3(0.0, 1.0, -1.0), point3(0.0, 0.0, 0.0), point3(0.0, 0.0, -2.0), material_left));
    // // world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_left));
    // // world.add(make_shared<sphere>(point3(-2.0,    1-0.25, -1.0),   0.25, normal_material));

    // world.add(make_shared<torus>(point3( 0.0,    0.0, 0.0), vec3(0, 1, 0), 1.0, 0.2, material_center));
    // // world.add(make_shared<cube>(point3( -1.0,    1.0, 1.0), 0.7, material_left));
    // // world.add(make_shared<sphere>(point3( 1.0,    0.0, 0.0), 0.5, green_material));
    // // world.add(make_shared<sphere>(point3( 0.0,    0.0, 0.0), 0.5, material_center));
    // // world.add(make_shared<sphere>(point3( -1.0,    0.0, 0.0), 0.5, red_material));
    // // world.add(make_shared<sphere>(point3( 0.0,    0.0, 1.0), 0.3, material_center));

    // // world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    // // world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    // // world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));
    // // auto light = point_light(point3(-3, 2, 0), 0);
    // auto light = dir_light(vec3(1.5, -1, 0), 0.2);
    // auto lightptr = &light;

    // // Camera
    // point3 lookfrom(0, 4, -8);
    // point3 lookat(0, 0, 0);
    // vec3 vup(0,1,0);
    // auto dist_to_focus = (lookfrom-lookat).length();
    // auto aperture = 0;

    // Image

    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 50;
    const int max_depth = 20;

    // World

    auto world = random_scene();
    blinn blinn(color(0.2, 0.2, 0.2), color(1.0, 1.0, 1.0), 1.0, 1.0, 1.0, 40.0);
    auto now = time(0);
    double choose_light = ((localtime(&now)->tm_sec) % 10) / 10.0;
    // std::cout << "choose_light: " << choose_light << std::endl;
    light* lightptr;
    auto dlight = dir_light(point3(-0.2, -1, -2.0), 0.2);
    auto plight = point_light(point3(-10, 50, 20), 4);
    if (choose_light < 0.5) {
        lightptr = &dlight;
    } else {
        lightptr = &plight;
    }

    // Camera

    point3 lookfrom(13,2,3);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, blinn, world, *lightptr, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}