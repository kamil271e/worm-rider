#ifndef COIN_H
#define COIN_H

#include "model.h"
#include "torus.h"
#include "../lib/shaderprogram.h"
#include <glm/glm.hpp>

class Coin{
    public:
        float x, z, rotation;
        Coin(float = 0.0f, float = 0.0f, float = 0.0f);
        ~Coin();
        bool drawCoin(float worm_x, float worm_z);
};

#endif