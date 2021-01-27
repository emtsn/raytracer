#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "rtweekend.h"
#include "hittable.h"

class triangle : public hittable {
    public:
        triangle() {}
        triangle(point3 pa, point3 pb, point3 pc, shared_ptr<material> m) {
            a = pa;
            b = pb;
            c = pc;
            vec3 crossPd = cross(c - b, a - b);
            n = unit_vector(crossPd);
            double_area = crossPd.length();
            mat_ptr = m;
        }
        virtual ~triangle() {}

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
            vec3 w = a - r.orig;
            double distToPlane = dot(w, n);
            double perpDistOfRay = dot(r.dir, n);
            if (perpDistOfRay == 0) {
                return false;
            }
            double t = distToPlane / perpDistOfRay;
            if (t < t_min || t > t_max) {
                return false;
            }
            point3 p = r.at(t);

            vec3 pa = a - p;
            vec3 pb = b - p;
            vec3 pc = c - p;

            double alpha = cross(pb, pc).length();
            double beta = cross(pa, pc).length();
            double gamma = cross(pa, pb).length();
            
            if (abs(double_area - (alpha + beta + gamma)) > 0.0001) {
                return false;
            }
            rec.t = t;
            rec.p = p;
            vec3 outward_normal = vec3(n.x(), n.y(), n.z());
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }

    public:
        point3 a;
        point3 b;
        point3 c;
        vec3 n;
        double double_area;
        shared_ptr<material> mat_ptr;
};

#endif