#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "scene.hpp"
#include <vector>

class Renderer {
public:
    // Renderiza a cena e preenche o buffer de imagem
    // image_buffer armazena RGB sequencialmente: r,g,b, r,g,b...
    void render(const Scene& scene, int width, int height, std::vector<unsigned char>& image_buffer);

private:
    // Função recursiva principal
    vec3 trace(const ray& r, const Scene& scene, int depth);
    
    // Calcula a cor base do objeto (Pigmento)
    vec3 get_pigment_color(const Pigment& pig, const vec3& p);
    
    // Calcula a iluminação local (Phong) + Sombras
    vec3 shade(const Scene& scene, const HitRecord& rec, const vec3& view_dir, int depth);

    // Máximo de rebatimentos (reflexão/refração)
    const int MAX_DEPTH = 5;
    const vec3 BACKGROUND_COLOR = vec3(0.1, 0.1, 0.1); // Cinza escuro
};

#endif