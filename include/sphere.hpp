#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "object.hpp"

class Sphere : public Object {
public:
    vec3 center;
    double radius;

    Sphere(int p, int f, vec3 c, double r) : Object(p, f), center(c), radius(r) {}

    virtual bool intersect(const ray& r, double t_min, double t_max, HitRecord& rec) const override;
    // A implementação vai no .cpp
};

#endif