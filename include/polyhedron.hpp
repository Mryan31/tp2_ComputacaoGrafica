#ifndef POLYHEDRON_HPP
#define POLYHEDRON_HPP

#include "object.hpp"
#include <vector>

// Representa um plano: Ax + By + Cz + D = 0
struct Plane {
    double a, b, c, d;
    vec3 normal() const { return vec3(a, b, c); }
};

class Polyhedron : public Object {
public:
    std::vector<Plane> faces;

    Polyhedron(int p, int f) : Object(p, f) {}

    void add_face(double a, double b, double c, double d) {
        faces.push_back({a, b, c, d});
    }

    virtual bool intersect(const ray& r, double t_min, double t_max, HitRecord& rec) const override;
};

#endif