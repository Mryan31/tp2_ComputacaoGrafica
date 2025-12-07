#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include "scene.hpp"

// A assinatura deve ser identica a implementacao no .cpp
bool load_scene(const std::string& filename, Scene& scene);

#endif