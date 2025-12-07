#ifndef PIGMENT_HPP
#define PIGMENT_HPP

#include "vec3.hpp"
#include <string>
#include <vector>

enum PigmentType { SOLID, CHECKER, TEXMAP };

struct Pigment {
    PigmentType type;

    // Solid
    vec3 color;

    // Checker
    vec3 color1, color2;
    double cube_size;

    // Texmap
    std::string texture_file; // Nome do arquivo PPM
    int tex_width, tex_height;
    std::vector<vec3> texture_data; // Pixels da imagem carregada
    
    // Vetores de projeção para textura (s = P0*PC, r = P1*PC)
    // Usamos vec3 aqui, assumindo coordenada homogênea implícita ou calculada no shader
    // O PDF diz que são vetores de 4 elementos. Vamos guardar 4 doubles para garantir.
    double p0[4]; 
    double p1[4];
};

#endif