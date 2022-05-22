#include "../lib/coin.h"
#include "../lib/shaderprogram.h"


Coin::Coin(float x, float z, float rotation){
    this->x = x;
    this->z = z;
    this->rotation = rotation;
}

Coin::~Coin(){}

bool Coin::drawCoin(float worm_x, float worm_z, GLuint &tex){
    float distance_to_coin = glm::sqrt(glm::pow(x-worm_x,2) + glm::pow(z-worm_z,2));
	if ( distance_to_coin > 5){
        // COIN JAKO ŻÓŁTY TORUS:
		/*glm::mat4 M = glm::mat4(1.0);
		M = glm::translate(M, glm::vec3(x, 0.5f, z));
		M = glm::rotate(M, rotation, glm::vec3(0,1,0));
		glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M)); 
		glUniform4f(spLambert->u("color"), 1, 1, 0, 1);
		Models::torus.drawSolid();
        return true;*/

        // COIN JAKO OTEKSTUROWANY SZEŚCIAN:
        glm::mat4 M = glm::mat4(1.0);
		M = glm::translate(M, glm::vec3(x, 0.5f, z));
		M = glm::rotate(M, rotation, glm::vec3(0,1,0));
        glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M));


        glEnableVertexAttribArray(spLambertTextured->a("vertex"));
        glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, myCubeVertices);

        glEnableVertexAttribArray(spLambertTextured->a("normal"));
        glVertexAttribPointer(spLambertTextured->a("normal"),4,GL_FLOAT,false,0,myCubeNormals);

        glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
        glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, myCubeTexCoords);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
        glUniform1i(spLambertTextured->u("tex"), 0);

        glDrawArrays(GL_TRIANGLES, 0, myCubeVertexCount);

        glDisableVertexAttribArray(spLambertTextured->a("vertex"));
        glDisableVertexAttribArray(spLambertTextured->a("normal"));
        glDisableVertexAttribArray(spLambertTextured->a("color"));

        return true;
    }
    else return false;
}
