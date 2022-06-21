#ifndef COIN_H
#define COIN_H

#include "model.h"
#include <glm/glm.hpp>

class Coin{
    public:
        float x, z, rotation;
        GLuint tex, tex_reflect;
        Coin(float = 0.0f, float = 0.0f, float = 0.0f, GLuint = 0, GLuint = 0);
        ~Coin();
        bool drawCoin(glm::vec3, glm::vec3, glm::vec3, ShaderProgram*);
};

#endif