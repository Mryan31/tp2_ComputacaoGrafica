#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
    vec3 orig;
    vec3 dir;

    ray() {}
    ray(const vec3& origin, const vec3& direction)
        : orig(origin), dir(direction) {}

    vec3 origin() const  { return orig; }
    vec3 direction() const { return dir; }

    vec3 at(double t) const {
        // P(t) = A + t*B
        // Retorna o ponto no raio no parametro t
        // Voce precisa implementar a sobrecarga de operadores no vec3.h para isso funcionar:
        // return orig + dir * t;
        return orig; // Placeholder
    }
};
#endif
