#include "../include/polyhedron.hpp"
#include <limits>
#include <cmath>

bool Polyhedron::intersect(const ray& r, double t_min, double t_max, HitRecord& rec) const {
    double t_enter = t_min;
    double t_exit = t_max;
    int hit_plane_index = -1;

    // Para um poliedro convexo, testamos o raio contra todos os planos das faces
    for (size_t i = 0; i < faces.size(); ++i) {
        const auto& plane = faces[i];
        vec3 n = plane.normal();
        
        // Denominador: N . D
        double denom = dot(n, r.direction());
        
        // Numerador: -(N . O + d)
        double dist = dot(n, r.origin()) + plane.d;

        // Raio paralelo ao plano
        if (std::abs(denom) < 1e-8) {
            // Se dist > 0, a origem está fora do semi-espaço (lado "positivo" do plano)
            if (dist > 0) return false; 
        } else {
            double t = -dist / denom;
            
            if (denom < 0) {
                // Entrando no semi-espaço
                if (t > t_enter) {
                    t_enter = t;
                    hit_plane_index = i;
                }
            } else {
                // Saindo do semi-espaço
                if (t < t_exit) {
                    t_exit = t;
                }
            }
        }
    }

    if (t_enter > t_exit) return false;
    if (t_enter < t_min || t_enter > t_max) return false;

    // Preenche o registro de hit
    rec.t = t_enter;
    rec.p = r.at(t_enter);
    if (hit_plane_index != -1) {
        rec.normal = unit_vector(faces[hit_plane_index].normal());
    } else {
        rec.normal = vec3(0,1,0); 
    }
    
    rec.pigment_index = pigment_idx;
    rec.finish_index = finish_idx;

    return true;
}