#ifndef SKULL_H
#define SKULL_H


#include "model.h"
#include <glm/glm.hpp>

class Skull{
    public:
        float x, z;
        GLuint tex;
        Skull(float = 0.0f, float = 0.0f, GLuint = 0);
        ~Skull();
        bool drawSkull(glm::vec3, glm::vec3, glm::vec3, ShaderProgram*);
};

#endif