#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "../include/parser.hpp"
#include "../include/renderer.hpp"

void save_ppm(const std::string& filename, int width, int height, const std::vector<unsigned char>& data) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao salvar arquivo " << filename << std::endl;
        return;
    }

    file << "P3\n" << width << " " << height << "\n255\n";
    for (size_t i = 0; i < data.size(); i += 3) {
        file << (int)data[i] << " " << (int)data[i+1] << " " << (int)data[i+2] << "\n";
    }
    file.close();
}

int main(int argc, char* argv[]) {
    // [cite: 46-48] Parâmetros de linha de comando
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <input_scene.txt> <output_image.ppm> [width] [height]" << std::endl;
        return 1;
    }

    std::string input_file = argv[1];
    std::string output_file = argv[2];
    int width = 800;
    int height = 600;

    if (argc >= 5) {
        width = std::stoi(argv[3]);
        height = std::stoi(argv[4]);
    }

    // 1. Carregar Cena
    Scene scene;
    std::cout << "Lendo cena: " << input_file << "..." << std::endl;
    if (!load_scene(input_file, scene)) {
        return 1; // Erro ao ler
    }

    // 2. Renderizar
    std::cout << "Renderizando imagem " << width << "x" << height << "..." << std::endl;
    Renderer renderer;
    std::vector<unsigned char> image_data;
    renderer.render(scene, width, height, image_data);

    // 3. Salvar
    std::cout << "Salvando em: " << output_file << "..." << std::endl;
    save_ppm(output_file, width, height, image_data);

    std::cout << "Concluido!" << std::endl;
    return 0;
}