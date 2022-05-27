#include "../lib/worm.h"
#include "../lib/shaderprogram.h"

Worm::Worm(){}

Worm::~Worm(){}

void Worm::drawWorm(glm::vec3 eye, glm::vec3 center, glm::vec3 up, std::vector<float> segm_rotations){
	glm::mat4 V = glm::lookAt(eye, center, up);
    glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f);
	spLambert->use();
	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V));
    
    glm::mat4 M;
	float heights[2] = {1.0f, -1.0f};

	for (int i = 0; i < 2; i++){
		M = glm::mat4(1.0f);
		M = glm::translate(M, glm::vec3(0.0f+eye.x, 0.0f, 7.5f+eye.z)); // ustawienie węża na scenie
		M = glm::rotate(M, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f)); // rotacja "do tyłu"
		M = glm::rotate(M, segm_rotations[i], glm::vec3(0.0f, 0.0f, 1.0f)); // "wicie się" segmentu węża
		M = glm::translate(M, glm::vec3(0.0f, heights[i], 2.0f)); // pozycja
		M = glm::scale(M, glm::vec3(0.3f, 1.0f, 0.3f)); // skalowanie
		glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M));
		Models::cube.drawSolid();
	}
}