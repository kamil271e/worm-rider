#include "../lib/worm.h"
#include "../lib/shaderprogram.h"

Worm::Worm(){}

Worm::~Worm(){}

void Worm::drawWorm(glm::vec3 eye, glm::vec3 center, glm::vec3 up, std::vector<float> segm_rotations,
					ShaderProgram* sp, GLuint skinTex, GLuint furTex){
	glm::mat4 V = glm::lookAt(eye, center, up);
    glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f);
	sp->use();
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));

	// Fur
	float* vertices = myCubeVertices;
	float* normals = myCubeNormals;
	float* texCoords = myCubeTexCoords;
	float* colors = myCubeColors;
	int vertexCount = myCubeVertexCount;

	glEnableVertexAttribArray(sp->a("vertex"));  //Enable sending data to the attribute vertex
    glVertexAttribPointer(sp->a("vertex"),4,GL_FLOAT,false,0,vertices); //Specify source of the data for the attribute vertex

	glEnableVertexAttribArray(sp->a("color")); 
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colors);

	glEnableVertexAttribArray(sp->a("normal"));  
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals); 

	glEnableVertexAttribArray(sp->a("texCoord0"));  
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords); 

	float maxFurLength = 0.1f;
	float maxLayer = 100;
	glUniform1f(sp->u("maxFurLength"),maxFurLength);
	glUniform1f(sp->u("maxLayer"),maxLayer);
	glUniform1i(sp->u("textureMapSkin"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, skinTex);
	glUniform1i(sp->u("textureMapFur"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, furTex);

    
    glm::mat4 M;
	float heights[2] = {2.0f, -2.0f};

	for (int i = 0; i < 2; i++){
		M = glm::mat4(1.0f);
		M = glm::translate(M, glm::vec3(0.0f+eye.x, 0.0f, 7.5f+eye.z)); // Positioning worm
		M = glm::rotate(M, 1.2f, glm::vec3(1.0f, 0.0f, 0.0f)); // "Backward" rotation 
		M = glm::rotate(M, segm_rotations[i], glm::vec3(0.0f, 0.0f, 1.0f)); // Worm segment "wriggling"
		M = glm::translate(M, glm::vec3(0.0f, heights[i], 2.0f)); // Position
		M = glm::scale(M, glm::vec3(0.3f, 1.0f, 0.3f)); // Scalling
		M = glm::scale(M, glm::vec3(2.0f, 2.0f, 2.0f)); 
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
		glDrawArraysInstanced(GL_TRIANGLES,0,vertexCount, maxLayer); // Drwa the object
	}

	glDisableVertexAttribArray(sp->a("vertex"));   //Disable sending data to the attribute vertex
	glDisableVertexAttribArray(sp->a("color"));  
	glDisableVertexAttribArray(sp->a("normal"));  
	glDisableVertexAttribArray(sp->a("texCoord0")); 
}