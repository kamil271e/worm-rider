#include "../lib/coin.h"
#include "../lib/shaderprogram.h"
#include "../lib/object.h"


Coin::Coin(float x, float z, float rotation, GLuint tex){
    this->x = x;
    this->z = z;
    this->rotation = rotation;
    this->tex = tex;
}

Coin::~Coin(){
    std::cout << "Coin destroyed" << std::endl;
}

bool Coin::drawCoin(glm::vec3 eye, glm::vec3 center, glm::vec3 up){
    static Object c("obj/coin.obj", tex);
    float distance_to_coin = glm::sqrt(glm::pow(x-eye.x,2) + glm::pow(z-eye.z,2));   
    if ( distance_to_coin > 7.5){

       	c.drawObject(eye, center, up, glm::vec3(x,1.0f,z), glm::vec3(1.5f, 0, rotation), glm::vec3(0.6f));

        return true;
    }
    else return false; // usuwanie coina
}
