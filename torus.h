#ifndef TORUS_H
#define TORUS_H

#include "rtweekend.h"
#include "hittable.h"

class torus : public hittable {
    public:
        torus() {}
        torus(point3 cen, vec3 uv, double r1, double r2, shared_ptr<material> m)
            : center(cen), upvector(uv), radius1(r1), radius2(r2), mat_ptr(m) {};
        virtual ~torus() {}

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
            if (r.dir.near_zero()) {
                return false;
            }
            double maxT = std::min(t_max, ((r.orig - center).length() + radius1) / r.dir.length());
            double minDist = 0.0001;
            double currentT = t_min;
            while (currentT <= maxT) {
                point3 origin = r.at(currentT);
                vec3 centerToRayOrig = origin - center;
                double distToPlane = dot(centerToRayOrig, upvector);
                point3 closestOnPlane = origin - (distToPlane * upvector);
                vec3 dirCenterToClosestOnPlane = unit_vector(closestOnPlane - center);
                point3 closestOnMedial = (radius1 * dirCenterToClosestOnPlane) + center;
                vec3 rayOrigToMedial = closestOnMedial - origin;
                double dist = std::abs(rayOrigToMedial.length() - radius2);
                currentT += dist / r.dir.length();
                if (dist < minDist) {
                    rec.t = currentT;
                    rec.p = r.at(rec.t);
                    vec3 outward_normal = unit_vector(rec.p - closestOnMedial);
                    rec.set_face_normal(r, outward_normal);
                    rec.mat_ptr = mat_ptr;
                    return true;
                }
            }
            return false;
        }

    public:
        point3 center;
        vec3 upvector;
        double radius1;
        double radius2;
        shared_ptr<material> mat_ptr;
};

#endif