#ifndef CUBE_H
#define CUBE_H

#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include "triangle.h"

class cube : public hittable {
    public:
        cube(point3 center, double length, std::shared_ptr<material> m) {
            double half_length = length / 2;
            point3 p1 = center + point3(-half_length,  half_length, -half_length);
            point3 p2 = center + point3(-half_length,  half_length,  half_length);
            point3 p3 = center + point3( half_length,  half_length,  half_length);
            point3 p4 = center + point3( half_length,  half_length, -half_length);
            point3 p5 = center + point3(-half_length, -half_length, -half_length);
            point3 p6 = center + point3(-half_length, -half_length,  half_length);
            point3 p7 = center + point3( half_length, -half_length,  half_length);
            point3 p8 = center + point3( half_length, -half_length, -half_length);
            
            // top (+y)
            triangles.add(make_shared<triangle>(p1, p2, p3, m));
            triangles.add(make_shared<triangle>(p3, p4, p1, m));
            // bottom (-y)
            triangles.add(make_shared<triangle>(p7, p6, p5, m));
            triangles.add(make_shared<triangle>(p5, p8, p7, m));

            // left (+x)
            triangles.add(make_shared<triangle>(p3, p7, p8, m));
            triangles.add(make_shared<triangle>(p8, p4, p3, m));
            // right (-x)
            triangles.add(make_shared<triangle>(p6, p2, p1, m));
            triangles.add(make_shared<triangle>(p1, p5, p6, m));

            // forward (+z)
            triangles.add(make_shared<triangle>(p7, p3, p2, m));
            triangles.add(make_shared<triangle>(p2, p6, p7, m));
            // back (-z)
            triangles.add(make_shared<triangle>(p5, p1, p4, m));
            triangles.add(make_shared<triangle>(p4, p8, p5, m));
        }
        virtual ~cube() {}

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const{
            return triangles.hit(r, t_min, t_max, rec);
        }
    
    private:
        hittable_list triangles;
};

#endif