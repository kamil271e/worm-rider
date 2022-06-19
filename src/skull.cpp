#include "../lib/skull.h"
#include "../lib/shaderprogram.h"
#include "../lib/object.h"

Skull::Skull(float x, float z, GLuint tex){
    this->x = x;
    this->z = z;
    this->tex = tex;
}

Skull::~Skull(){}

bool Skull::drawSkull(glm::vec3 eye, glm::vec3 center, glm::vec3 up, ShaderProgram* sp){
    static Object skull("obj/skull.obj", tex);
    float distance_to_skull = glm::sqrt(glm::pow(x-eye.x,2) + glm::pow(z-eye.z,2)); 
    if (eye.z - z < -10.0f){
        skull.drawObject(eye, center, up, glm::vec3(x,-0.5f,z), glm::vec3(5.0f,0.0f,10.0f), glm::vec3(0.1f), sp);
        return true;
    }
    return false;
}