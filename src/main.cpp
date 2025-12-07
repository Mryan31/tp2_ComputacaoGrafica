#include <iostream>
#include <string>
#include "../include/vec3.h"
#include "../include/ray.h"

// Estrutura para configuracoes da renderizacao
struct RenderConfig {
    std::string inputFile;
    std::string outputFile;
    int width = 800;  // Padrao definido na especificacao
    int height = 600; // Padrao definido na especificacao
};

int main(int argc, char* argv[]) {
    // Leitura de argumentos conforme especificacao 
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <input_scene.txt> <output_image.ppm> [width] [height]" << std::endl;
        return 1;
    }

    RenderConfig config;
    config.inputFile = argv[1];
    config.outputFile = argv[2];

    if (argc >= 5) {
        config.width = std::stoi(argv[3]);
        config.height = std::stoi(argv[4]);
    }

    std::cout << "Renderizando cena: " << config.inputFile << std::endl;
    std::cout << "Saida: " << config.outputFile << " (" << config.width << "x" << config.height << ")" << std::endl;

    // TODO: Chamar o Parser aqui para ler a cena
    
    // Exemplo de geracao de PPM simples (Degrade de teste)
    // Redirecionar stdout para arquivo ou usar fstream
    FILE *f = fopen(config.outputFile.c_str(), "w");
    fprintf(f, "P3\n%d %d\n255\n", config.width, config.height);

    for (int j = config.height - 1; j >= 0; --j) {
        for (int i = 0; i < config.width; ++i) {
            auto r = double(i) / (config.width - 1);
            auto g = double(j) / (config.height - 1);
            auto b = 0.25;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            fprintf(f, "%d %d %d\n", ir, ig, ib);
        }
    }
    fclose(f);

    std::cout << "Concluido." << std::endl;
    return 0;
}
