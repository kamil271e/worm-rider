#include <random>
#include <vector>
#include "coin.h"

// funkcja pomocnicza - usuwanie danego elementu w wektorze
void remove(std::vector<Coin>& vec, size_t pos)
{
    std::vector<Coin>::iterator it = vec.begin();
    std::advance(it, pos);
    vec.erase(it);
}

// funkcja pomocnicza - losowanie liczby z przedzialu [a,b)
float randomNum(float a, float b){
	return a + (rand() / (RAND_MAX / (b-a)));
}

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Read into computers memory
	std::vector<unsigned char> image;   //Allocate memory 
	unsigned width, height;   //Variables for image size
	//Read the image
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import to graphics card memory
	glGenTextures(1, &tex); //Initialize one handle
	glBindTexture(GL_TEXTURE_2D, tex); //Activate handle
	//Copy image to graphics cards memory reprezented by the active handle
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}