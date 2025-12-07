#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "ray.hpp"
#include "pigment.hpp"
#include "finish.hpp"

struct HitRecord {
    double t;
    vec3 p;
    vec3 normal;
    int pigment_index; // Índice no vetor global de pigmentos
    int finish_index;  // Índice no vetor global de acabamentos
};

class Object {
public:
    int pigment_idx;
    int finish_idx;

    Object(int p_idx, int f_idx) : pigment_idx(p_idx), finish_idx(f_idx) {}
    virtual ~Object() {}

    // Retorna true se houver interseção válida entre t_min e t_max
    virtual bool intersect(const ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
};

#endif