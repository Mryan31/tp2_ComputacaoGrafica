#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "vec3.hpp"

struct Light {
    vec3 position;
    vec3 color; // Intensidade RGB (0 a 1)
    
    // Atenuação
    double att_const;     // Constante
    double att_linear;    // Linear
    double att_quad;      // Quadrática
};

#endif