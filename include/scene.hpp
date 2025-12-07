#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <memory>
#include "camera.hpp"
#include "light.hpp"
#include "pigment.hpp"
#include "finish.hpp"
#include "object.hpp"

class Scene {
public:
    Camera camera;
    std::vector<Light> lights;
    std::vector<Pigment> pigments;
    std::vector<Finish> finishes;
    
    // Usamos shared_ptr pois Object é polimórfico (Sphere ou Polyhedron)
    std::vector<std::shared_ptr<Object>> objects;

    Scene() {}
};

#endif