#ifndef COIN_H
#define COIN_H

#include "model.h"
#include "cube.h"
#include <glm/glm.hpp>

class Coin{
    public:
        float x, z, rotation;
        GLuint tex;
        Coin(float = 0.0f, float = 0.0f, float = 0.0f, GLuint = 0);
        ~Coin();
        bool drawCoin(glm::vec3, glm::vec3, glm::vec3);
};

#endif