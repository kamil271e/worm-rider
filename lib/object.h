#ifndef OBJECT_H
#define OBJECT_H
#include <glm/glm.hpp>
#include <string>

class Object{
    public:
        float *vertices, *normals, *texCoords;
        int vertexCount;
        GLuint tex, tex_reflect;
        Object(std::string, GLuint=0, GLuint=0, bool=false);
        ~Object();
        void drawObject(glm::vec3, glm::vec3, glm::vec3, glm::vec3, glm::vec3, glm::vec3, ShaderProgram*, bool=false);
};

#endif