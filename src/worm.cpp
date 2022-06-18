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

	glEnableVertexAttribArray(sp->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
    glVertexAttribPointer(sp->a("vertex"),4,GL_FLOAT,false,0,vertices); //Wskaż tablicę z danymi dla atrybutu vertex

	glEnableVertexAttribArray(sp->a("color"));  //Włącz przesyłanie danych do atrybutu color
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colors); //Wskaż tablicę z danymi dla atrybutu color

	glEnableVertexAttribArray(sp->a("normal"));  //Włącz przesyłanie danych do atrybutu normal
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals); //Wskaż tablicę z danymi dla atrybutu normal

	glEnableVertexAttribArray(sp->a("texCoord0"));  //Włącz przesyłanie danych do atrybutu texCoord
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords); //Wskaż tablicę z danymi dla atrybutu texCoord

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
		M = glm::translate(M, glm::vec3(0.0f+eye.x, 0.0f, 7.5f+eye.z)); // ustawienie węża na scenie
		M = glm::rotate(M, 1.2f, glm::vec3(1.0f, 0.0f, 0.0f)); // rotacja "do tyłu"
		M = glm::rotate(M, segm_rotations[i], glm::vec3(0.0f, 0.0f, 1.0f)); // "wicie się" segmentu węża
		M = glm::translate(M, glm::vec3(0.0f, heights[i], 2.0f)); // pozycja
		M = glm::scale(M, glm::vec3(0.3f, 1.0f, 0.3f)); // skalowanie
		M = glm::scale(M, glm::vec3(2.0f, 2.0f, 2.0f)); // powiększenie
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
		// Models::cube.drawSolid();
		glDrawArraysInstanced(GL_TRIANGLES,0,vertexCount, maxLayer); //Narysuj obiekt
	}

	glDisableVertexAttribArray(sp->a("vertex"));  //Wyłącz przesyłanie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("color"));  //Wyłącz przesyłanie danych do atrybutu color
	glDisableVertexAttribArray(sp->a("normal"));  //Wyłącz przesyłanie danych do atrybutu normal
	glDisableVertexAttribArray(sp->a("texCoord0"));  //Wyłącz przesyłanie danych do atrybutu texCoord0
}