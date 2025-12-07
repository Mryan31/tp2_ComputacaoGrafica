#include "../include/parser.hpp"
#include "../include/sphere.hpp"
#include "../include/polyhedron.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

// Funções auxiliares para leitura
template <typename T>
void read_val(std::ifstream& file, T& val) {
    file >> val;
}

void read_vec3(std::ifstream& file, vec3& v) {
    double x, y, z;
    file >> x >> y >> z;
    v = vec3(x, y, z);
}

// Implementação da função load_scene
bool load_scene(const std::string& filename, Scene& scene) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir " << filename << std::endl;
        return false;
    }

    //[cite_start]// [cite: 11] 1. Câmera
    read_vec3(file, scene.camera.eye);
    read_vec3(file, scene.camera.at);
    read_vec3(file, scene.camera.up);
    read_val(file, scene.camera.fovy);

    //[cite_start]// [cite: 14] 2. Luzes
    int num_lights;
    read_val(file, num_lights);
    for (int i = 0; i < num_lights; ++i) {
        Light l;
        read_vec3(file, l.position);
        read_vec3(file, l.color);
        read_val(file, l.att_const);
        read_val(file, l.att_linear);
        read_val(file, l.att_quad);
        scene.lights.push_back(l);
    }

    //[cite_start]// [cite: 19] 3. Pigmentos
    int num_pigments;
    read_val(file, num_pigments);
    for (int i = 0; i < num_pigments; ++i) {
        Pigment p;
        std::string type;
        read_val(file, type);

        if (type == "solid") {
            p.type = SOLID;
            read_vec3(file, p.color);
        } else if (type == "checker") {
            p.type = CHECKER;
            read_vec3(file, p.color1);
            read_vec3(file, p.color2);
            read_val(file, p.cube_size);
        } else if (type == "texmap") {
            p.type = TEXMAP;
            read_val(file, p.texture_file);
            for(int k=0; k<4; k++) file >> p.p0[k];
            for(int k=0; k<4; k++) file >> p.p1[k];
        }
        scene.pigments.push_back(p);
    }

    //[cite_start]// [cite: 30] 4. Acabamentos
    int num_finishes;
    read_val(file, num_finishes);
    for (int i = 0; i < num_finishes; ++i) {
        Finish f;
        read_val(file, f.ka);
        read_val(file, f.kd);
        read_val(file, f.ks);
        read_val(file, f.alpha);
        read_val(file, f.kr);
        read_val(file, f.kt);
        read_val(file, f.ior);
        scene.finishes.push_back(f);
    }

    //[cite_start]// [cite: 39] 5. Objetos
    int num_objects;
    read_val(file, num_objects);
    for (int i = 0; i < num_objects; ++i) {
        int pig_idx, fin_idx;
        std::string obj_type;
        read_val(file, pig_idx);
        read_val(file, fin_idx);
        read_val(file, obj_type);

        if (obj_type == "sphere") {
            vec3 c;
            double r;
            read_vec3(file, c);
            read_val(file, r);
            scene.objects.push_back(std::make_shared<Sphere>(pig_idx, fin_idx, c, r));
        } 
        else if (obj_type == "polyhedron") {
            int num_faces;
            read_val(file, num_faces);
            auto poly = std::make_shared<Polyhedron>(pig_idx, fin_idx);
            for (int k = 0; k < num_faces; ++k) {
                double a, b, c, d;
                file >> a >> b >> c >> d;
                poly->add_face(a, b, c, d);
            }
            scene.objects.push_back(poly);
        }
    }

    return true;
}