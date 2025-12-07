#include "../include/renderer.hpp"
#include <cmath>
#include <limits>
#include <algorithm>
#include <iostream>

// Definição de PI para garantir compatibilidade
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// --- Funções Auxiliares ---

double clamp(double x) { return x < 0 ? 0 : (x > 1 ? 1 : x); }

unsigned char to_byte(double x) { return static_cast<unsigned char>(255.99 * clamp(x)); }

vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
}

// --- Implementação da Classe Renderer ---

void Renderer::render(const Scene& scene, int width, int height, std::vector<unsigned char>& buffer) {
    buffer.resize(width * height * 3);

    // Configuração da Câmera
    vec3 w = unit_vector(scene.camera.eye - scene.camera.at); // Z
    vec3 u = unit_vector(cross(scene.camera.up, w));          // X
    vec3 v = cross(w, u);                                     // Y

    double theta = scene.camera.fovy * M_PI / 180.0;
    double h_height = tan(theta / 2.0);
    double viewport_height = 2.0 * h_height;
    double viewport_width = viewport_height * (double(width) / height);

    vec3 horizontal = viewport_width * u;
    vec3 vertical = viewport_height * v;
    vec3 lower_left_corner = scene.camera.eye - horizontal/2 - vertical/2 - w;

    // Loop principal
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            double u_coord = double(i) / (width - 1);
            double v_coord = double(height - 1 - j) / (height - 1);

            vec3 dir = lower_left_corner + u_coord*horizontal + v_coord*vertical - scene.camera.eye;
            ray r(scene.camera.eye, unit_vector(dir));

            vec3 pixel_color = trace(r, scene, 0);

            int index = (j * width + i) * 3;
            buffer[index] = to_byte(pixel_color.x());
            buffer[index+1] = to_byte(pixel_color.y());
            buffer[index+2] = to_byte(pixel_color.z());
        }
    }
}

vec3 Renderer::trace(const ray& r, const Scene& scene, int depth) {
    HitRecord rec;
    bool hit_anything = false;
    double closest_so_far = std::numeric_limits<double>::max();
    HitRecord temp_rec;

    for (const auto& object : scene.objects) {
        if (object->intersect(r, 0.001, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    if (!hit_anything) {
        return BACKGROUND_COLOR;
    }

    return shade(scene, rec, -r.direction(), depth);
}

vec3 Renderer::get_pigment_color(const Pigment& pig, const vec3& p) {
    if (pig.type == SOLID) {
        return pig.color;
    } 
    else if (pig.type == CHECKER) {
        double size = pig.cube_size;
        int cx = static_cast<int>(std::floor(p.x() / size));
        int cy = static_cast<int>(std::floor(p.y() / size));
        int cz = static_cast<int>(std::floor(p.z() / size));

        if ((cx + cy + cz) % 2 == 0) return pig.color1;
        else return pig.color2;
    }
    else if (pig.type == TEXMAP) {
        // Mapeamento Planar simples baseado nos vetores P0 e P1
        double s = pig.p0[0] * p.x() + pig.p0[1] * p.y() + pig.p0[2] * p.z() + pig.p0[3];
        double r = pig.p1[0] * p.x() + pig.p1[1] * p.y() + pig.p1[2] * p.z() + pig.p1[3];
        
        s = s - std::floor(s);
        r = r - std::floor(r);

        return vec3(s, r, 0.0); // Placeholder visual para textura
    }
    return vec3(1, 0, 1);
}

vec3 Renderer::shade(const Scene& scene, const HitRecord& rec, const vec3& view_dir, int depth) {
    const Finish& finish = scene.finishes[rec.finish_index];
    const Pigment& pigment = scene.pigments[rec.pigment_index];

    vec3 base_color = get_pigment_color(pigment, rec.p);
    vec3 final_color = finish.ka * base_color; 

    for (const auto& light : scene.lights) {
        vec3 light_dir = light.position - rec.p;
        double dist = light_dir.length();
        light_dir = unit_vector(light_dir);

        // Shadow Ray
        ray shadow_ray(rec.p, light_dir);
        bool in_shadow = false;
        HitRecord shadow_rec;
        for (const auto& obj : scene.objects) {
            if (obj->intersect(shadow_ray, 0.001, dist, shadow_rec)) {
                in_shadow = true;
                break;
            }
        }

        if (!in_shadow) {
            double attenuation = 1.0 / (light.att_const + light.att_linear * dist + light.att_quad * dist * dist);
            
            double diff = std::max(0.0, dot(rec.normal, light_dir));
            vec3 diffuse = finish.kd * diff * base_color * light.color;

            vec3 reflect_dir_light = reflect(-light_dir, rec.normal);
            double spec = std::pow(std::max(0.0, dot(reflect_dir_light, view_dir)), finish.alpha);
            vec3 specular = finish.ks * spec * light.color; 

            final_color += attenuation * (diffuse + specular);
        }
    }

    if (depth < MAX_DEPTH) {
        // Reflexão
        if (finish.kr > 0) {
            vec3 incident = -view_dir;
            vec3 r_out = reflect(incident, rec.normal);
            ray reflected_ray(rec.p, unit_vector(r_out));
            
            final_color += finish.kr * trace(reflected_ray, scene, depth + 1);
        }

        // Refração
        if (finish.kt > 0) {
            vec3 incident = -view_dir;
            double dt = dot(incident, rec.normal);
            double ni_over_nt;
            vec3 normal_vector;

            if (dt > 0) { 
                normal_vector = -rec.normal;
                ni_over_nt = 1.0 / finish.ior; 
            } else { 
                normal_vector = rec.normal;
                ni_over_nt = finish.ior;
                dt = -dt;
            }

            vec3 unit_in = unit_vector(incident);
            double discriminant = 1.0 - ni_over_nt * ni_over_nt * (1.0 - dt * dt);
            
            if (discriminant > 0) {
                vec3 refracted = ni_over_nt * (unit_in + normal_vector * dt) - normal_vector * std::sqrt(discriminant);
                ray refracted_ray(rec.p, refracted);
                final_color += finish.kt * trace(refracted_ray, scene, depth + 1);
            } 
        }
    }

    return final_color;
}