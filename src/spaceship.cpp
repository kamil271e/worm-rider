#include "../lib/spaceship.h"
#include "../lib/shaderprogram.h"
#include "../lib/object.h"
#include "../lib/coin.h"

Spaceship::Spaceship(float x, float z, GLuint tex){
    this->x = x;
    this->z = z;
    this->tex = tex;
}

Spaceship::~Spaceship(){}

bool Spaceship::drawSpaceship(glm::vec3 eye, glm::vec3 center, glm::vec3 up, float z_change, ShaderProgram* sp){
    static Object spaceship("obj/spaceship.obj", tex);
    float distance_to_spaceship = glm::sqrt(glm::pow(x-eye.x,2) + glm::pow(z+z_change-eye.z,2));
    if ( distance_to_spaceship < 3){
       	std::cout<<"\nSpaceship touched - game over"<<std::endl;
        std::cout<<"Collected coins: "<<coin_collecting(false)<<std::endl<<std::endl;
        exit(0);
    }
    if (eye.z - (z + z_change) < 30.0f){
        spaceship.drawObject(eye, center, up, glm::vec3(x, 1.5f,(z + z_change)), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f), sp, true);
        return true;
    }
    
    return false;
}