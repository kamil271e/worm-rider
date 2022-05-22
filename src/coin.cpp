#include "../lib/coin.h"

Coin::Coin(float x, float z, float rotation){
    this->x = x;
    this->z = z;
    this->rotation = rotation;
}

Coin::~Coin(){}

bool Coin::drawCoin(float worm_x, float worm_z){
    float distance_to_coin = glm::sqrt(glm::pow(x-worm_x,2) + glm::pow(z-worm_z,2));
	if ( distance_to_coin > 5){
		glm::mat4 M = glm::mat4(1.0);
		M = glm::translate(M, glm::vec3(x, 0.5f, z));
		M = glm::rotate(M, rotation, glm::vec3(0,1,0));
		glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M)); 
		glUniform4f(spLambert->u("color"), 1, 1, 0, 1);
		Models::torus.drawSolid();
        return true;
    }
    else return false;
}
