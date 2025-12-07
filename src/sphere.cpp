#include "../include/sphere.hpp"
#include <cmath>

bool Sphere::intersect(const ray& r, double t_min, double t_max, HitRecord& rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = std::sqrt(discriminant);

    // Encontrar a raiz mais próxima que esteja no intervalo aceitável (t_min, t_max)
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || root > t_max) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || root > t_max)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    // Normal da esfera: (Ponto - Centro) / Raio
    vec3 outward_normal = (rec.p - center) / radius;
    rec.normal = outward_normal; // Assumimos normal sempre apontando para fora
    
    rec.pigment_index = pigment_idx;
    rec.finish_index = finish_idx;

    return true;
}