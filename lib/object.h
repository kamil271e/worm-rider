#ifndef OBJECT_H
#define OBJECT_H
#include <glm/glm.hpp>
#include <string>

class Object{
    public:
        float *vertices, *normals, *texCoords;
        int vertexCount;
        GLuint tex;
        Object(std::string, GLuint);
        ~Object();
        void drawObject(glm::vec3, glm::vec3, glm::vec3);
};

#endif