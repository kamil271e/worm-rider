#ifndef SPACESHIP_H
#define SPACESHIP_H


#include "model.h"
#include <glm/glm.hpp>

class Spaceship{
    public:
        float x, z;
        float z_change=0;
        GLuint tex;
        Spaceship(float = 0.0f, float = 0.0f, GLuint = 0);
        ~Spaceship();
        bool drawSpaceship(glm::vec3, glm::vec3, glm::vec3, float, ShaderProgram*);
};

#endif