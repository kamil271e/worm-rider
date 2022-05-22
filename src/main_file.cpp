#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "../lib/constants.h"
#include "../lib/allmodels.h"
#include "../lib/lodepng.h"
#include "../lib/shaderprogram.h"
#include "coin.cpp"

float x_speed = 0.0f; // [radians/s]
float z_speed = 0.0f;

double x_cursor = 0.0f;
double y_cursor = 0.0f;
float x_window = 1000.0f;
float y_window = 1000.0f;

float x_change = 0;
float z_change = 0;

glm::vec3 eye;
glm::vec3 center;
glm::vec3 up = glm::vec3(glm::vec3(0.0f, 5.0f, 0.0f));

std::vector<Coin> CoinVector;

//Error processing callback procedure
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_A) x_speed = 1.5*PI;
		if (key == GLFW_KEY_D) x_speed = -1.5*PI;
		if (key == GLFW_KEY_W) z_speed = 1.5*PI;
		if (key == GLFW_KEY_S) z_speed = -1.5*PI;
		if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_A || key == GLFW_KEY_D) x_speed = 0;
		if (key == GLFW_KEY_W || key == GLFW_KEY_S) z_speed = 0;
	}
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset){
	if (yOffset < 0) z_speed = -1.5*PI;
	else z_speed = 1.5*PI;
}

void initCoins(){
	float init_x[3] = {5.0f,-3.0f,0.0f};
	float init_z[3] = {20.0f, 30.0f, 40.0f};
	for (int i = 0; i <3; i++){
		Coin coin(init_x[i], init_z[i]);
		CoinVector.push_back(coin);
	}
}

//Initialization code procedure
void initOpenGLProgram(GLFWwindow* window) {
	initShaders();
	initCoins();
	glEnable(GL_DEPTH_TEST);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	//************Place any code here that needs to be executed once, at the program start************
}

//Release resources allocated by the program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
	//************Place any code here that needs to be executed once, after the main loop ends************
}

void updateCamera(){
	eye = glm::vec3(x_change, 3.0f, z_change);
	center = glm::vec3(x_change - (float)(x_cursor)/x_window*20, 0.0f, z_change+10);
}

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


void drawCoins(glm::mat4 &P, glm::mat4 &V, float coin_rotation){
	spLambert->use();
	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V));

	for (int i = 0; i < 3; i++){
		CoinVector[i].rotation = coin_rotation;
		if (!CoinVector[i].drawCoin(eye.x, eye.z)){
			// domyslnie nowo pojawiajace sie monety bede mialy wspolrzedne x:[N-8f,N+8f), z:[N,N+20f),
			// gdzie N to pozycja ostatniej monety w wektorze (najdalszej od obserwatora)
			remove(CoinVector, i); 
			float temp_x = randomNum(CoinVector.back().x-8.0f, CoinVector.back().x+8.0f);
			float temp_z = randomNum(CoinVector.back().z, CoinVector.back().z+20.f);
			Coin temp_coin(temp_x, temp_z, coin_rotation);
			CoinVector.push_back(temp_coin);
		}
	}	
}

// First Person Perspecitve
void FPP(GLFWwindow* window, float coin_rotation){
	updateCamera();
	glm::mat4 V = glm::lookAt(eye, center, up);
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f);
	drawCoins(P,V,coin_rotation);
	glfwGetCursorPos(window, &x_cursor, &y_cursor);
	// std::cout << "X:" << x_cursor << std::endl;
	std::cout << "(X:" << eye.x << ", Y:" << eye.y << ", Z:" << eye.z << ")" << std::endl;
}

//Drawing procedure
void drawScene(GLFWwindow* window, float coin_rotation) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0.15, 1.0f);

	FPP(window, coin_rotation);

	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSwapBuffers(window);	
}

int main(void)
{
	GLFWwindow* window; //Pointer to object that represents the application window

	glfwSetErrorCallback(error_callback);//Register error processing callback procedure

	if (!glfwInit()) { //Initialize GLFW library
		fprintf(stderr, "Can't initialize GLFW.\n");
		exit(EXIT_FAILURE); 
	}

	window = glfwCreateWindow(x_window, y_window, "OpenGL", NULL, NULL);  //Create a window 500pxx500px titled "OpenGL" and an OpenGL context associated with it. 

	if (!window) //If no window is opened then close the program
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Since this moment OpenGL context corresponding to the window is active and all OpenGL calls will refer to this context.
	glfwSwapInterval(1); //During vsync wait for the first refresh

	GLenum err;
	if ((err=glewInit()) != GLEW_OK) { //Initialize GLEW library
		fprintf(stderr, "Can't initialize GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}
	initOpenGLProgram(window); //Call initialization procedure

	//Main application loop
	float coin_rotate_angle = 0;
	float coin_rotate_speed = 1.0f;
	glfwSetTime(0);
	while (!glfwWindowShouldClose(window)) //As long as the window shouldnt be closed yet...
	{		
		x_change += x_speed * glfwGetTime();
		z_change += z_speed * glfwGetTime();
		coin_rotate_angle += coin_rotate_speed * glfwGetTime();

		glfwSetTime(0);
		drawScene(window, coin_rotate_angle); //Execute drawing procedure
		glfwPollEvents(); //Process callback procedures corresponding to the events that took place up to now
	}
	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Delete OpenGL context and the window.
	glfwTerminate(); //Free GLFW resources
	exit(EXIT_SUCCESS);
}