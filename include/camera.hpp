#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "vec3.hpp"

struct Camera {
    vec3 eye;       // Posição do olho
    vec3 at;        // Para onde olha
    vec3 up;        // Vetor UP
    double fovy;    // Abertura vertical
    
    // Dados derivados para gerar raios (calculados depois)
    vec3 u, v, w;
    double viewport_height;
    double viewport_width;
    vec3 horizontal;
    vec3 vertical;
    vec3 lower_left_corner;
};

#endif