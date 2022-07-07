#include "../lib/coin.h"
#include "../lib/shaderprogram.h"
#include "../lib/object.h"


Coin::Coin(float x, float z, float rotation, GLuint tex, GLuint tex_reflect){
    this->x = x;
    this->z = z;
    this->rotation = rotation;
    this->tex = tex;
    this->tex_reflect = tex;
}

Coin::~Coin(){}

bool Coin::drawCoin(glm::vec3 eye, glm::vec3 center, glm::vec3 up, ShaderProgram* sp){
    static Object c("obj/coin.obj", tex, tex_reflect);
    float distance_to_coin = glm::sqrt(glm::pow(x-eye.x,2) + glm::pow(z-eye.z,2)); 
    if (eye.z - z > 7.5){ //Coin has not been collected - draw new one
        c.drawObject(eye, center, up, glm::vec3(x,1.0f,z), glm::vec3(1.5f, 0, rotation), glm::vec3(0.6f), sp);
        return false;
    }
    else if ( distance_to_coin > 3){ //Coin is far
       	c.drawObject(eye, center, up, glm::vec3(x,1.0f,z), glm::vec3(1.5f, 0, rotation), glm::vec3(0.6f), sp);
        return true;
    }
    else{   //Collect and deleting coin
        coin_collecting();
        return false;
    }
}


