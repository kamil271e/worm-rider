#ifndef WORM_H
#define WORM_H

#include "model.h"
#include "cube.h"
#include <glm/glm.hpp>
#include <vector>

class Worm{
    public:
        Worm();
        ~Worm();
        void drawWorm(glm::vec3, glm::vec3, glm::vec3, std::vector<float>, ShaderProgram*, GLuint skinTex, GLuint furTex);
};

#endif