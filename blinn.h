#ifndef BLINN_H
#define BLINN_H

#include "rtweekend.h"
#include "hittable.h"
#include "light.h"

class blinn {
    public:
        blinn(color ac, color lc, double ka, double kd, double ks, double sh) :
        ambientcolor(ac), lightcolor(lc), kambient(ka), kdiffuse(kd), kspecular(ks), shininess(sh) {}
        virtual ~blinn() {}

        color shade(const color& materialcolor, const point3& viewpos, const light& lights, const hittable& world, const hit_record& rec) const {
            if (!rec.front_face) {
                return materialcolor;
            }
            color ambient = materialcolor * ambientcolor;
            if (!lights.inlight(rec.p, world)) {
                return kambient * ambient;
            }
            vec3 viewvector = unit_vector(viewpos - rec.p);
            vec3 lightvector = lights.lightvector(rec.p);
            double nlAngle = std::max(0.0, dot(rec.normal, lightvector));
            color diffuse = materialcolor * lightcolor * nlAngle;

            vec3 halfvector = unit_vector(lightvector + viewvector);
            float specamount = std::pow(std::max(0.0, dot(rec.normal, halfvector)), shininess);
            color specular = specamount * lightcolor;

            return (kambient * ambient) + (kdiffuse * diffuse) + (kspecular * specular);
        }

    public:
        color ambientcolor;
        color lightcolor;
        double kambient;
        double kdiffuse;
        double kspecular;
        double shininess;
};

#endif