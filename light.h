#ifndef LIGHT_H
#define LIGHT_H

#include "rtweekend.h"
#include "hittable.h"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class light {
    public:
        light() {}
        virtual bool inlight(const point3& p, const hittable& world) const {
            hit_record lightblockrec;
            if (world.hit(ray(p, lightvector(p)), 0.001, infinity, lightblockrec)) {
                return false;
            }
            return true;
        }
        virtual vec3 lightvector(const point3& p) const = 0;
};

class point_light : public light {
    public:
        point_light(point3 position, double s) : pos(position), size(s) {}
        virtual vec3 lightvector(const point3& p) const {
            if (size == 0) {
                return unit_vector(pos - p);
            }
            return unit_vector(pos + (size * random_in_unit_sphere()) - p);
        }
    private:
        point3 pos;
        double size; // length of the radius of the point light sphere for soft shadows
};

class dir_light : public light {
    public:
        dir_light(vec3 direction, double s) : dir(direction), soft(s) {}
        virtual vec3 lightvector(const point3& p) const {
            vec3 newdir = unit_vector(dir + soft * random_in_unit_sphere());
            if (newdir.near_zero()) {
                return unit_vector(-dir);
            }
            return -unit_vector(newdir);
        }
    private:
        vec3 dir;
        double soft;
};

#endif