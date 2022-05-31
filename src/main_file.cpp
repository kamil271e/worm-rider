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
#include <fstream>
#include "../lib/constants.h"
#include "../lib/lodepng.h"
#include "../lib/shaderprogram.h"
#include "../lib/myCube.h"
#include "../lib/utils.h"
#include "coin.cpp"
#include "worm.cpp"
#include "../lib/OBJ_loader.h"
#include "object.cpp"

float x_speed = 0.0f; // [radians/s]
float z_speed = 0.0f;
float worm_wriggle_speed = 1.0f; 
float coin_rotate_speed = 1.0f;

double x_cursor = 0.0f;
double y_cursor = 0.0f;
float x_window = 1000.0f;
float y_window = 1000.0f;

float x_change = 0;
float z_change = 0;

glm::vec3 eye;
glm::vec3 center;
glm::vec3 up = glm::vec3(glm::vec3(0.0f, 5.0f, 0.0f));
ShaderProgram *sp;

GLuint tex, objTex;
std::vector<Coin> CoinVector;
Worm worm;
objl::Loader loader;
float desert_size = 15.0f;

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
		Coin coin(init_x[i], init_z[i], 0.0f, tex);
		CoinVector.push_back(coin);
	}
}

//Initialization code procedure
void initOpenGLProgram(GLFWwindow* window) {
	tex = readTexture("tex/gold.png");
	objTex = readTexture("tex/sand.png");
	initShaders();
	initCoins();
	glEnable(GL_DEPTH_TEST);
	sp=new ShaderProgram("v_lamberttextured.glsl",NULL,"f_lamberttextured.glsl");
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

//Release resources allocated by the program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
	glDeleteTextures(1,&tex);
	glDeleteTextures(1,&objTex);
	delete sp;
	//************Place any code here that needs to be executed once, after the main loop ends************
}

void updateCamera(){
	eye = glm::vec3(x_change, 3.0f, z_change);
	center = glm::vec3(x_change - (float)(x_cursor)/x_window*20, 0.0f, z_change+10);
}

void drawCoins(float coin_rotation){
	for (int i = 0; i < 3; i++){ // 3 coiny jednoczesnie na scenie
		CoinVector[i].rotation = coin_rotation;
		if (!CoinVector[i].drawCoin(eye, center, up)){
			// domyslnie nowo pojawiajace sie monety bede mialy wspolrzedne x:[N-8f,N+8f), z:[N,N+20f),
			// gdzie N to pozycja ostatniej monety w wektorze (najdalszej od obserwatora)
			remove(CoinVector, i); 
			float temp_x = randomNum(CoinVector.back().x-8.0f, CoinVector.back().x+8.0f);
			float temp_z = randomNum(CoinVector.back().z, CoinVector.back().z+20.f);
			Coin temp_coin(temp_x, temp_z, coin_rotation, tex);
			CoinVector.push_back(temp_coin);
		}
	}	
}

// First Person Perspecitve
void FPP(GLFWwindow* window, float coin_rotation, std::vector<float> worm_rotation){
	drawCoins(coin_rotation);
	worm.drawWorm(eye, center, up, worm_rotation);
	glfwGetCursorPos(window, &x_cursor, &y_cursor);
	//std::cout << "X:" << x_cursor << std::endl;
	std::cout << "(X:" << eye.x << ", Y:" << eye.y << ", Z:" << eye.z << ")" << std::endl;
}

void drawOBJ(){
	static Object desert("obj/desert.obj", objTex);
	for (int i = 0; i < 5; i++){
		for (int j = 0; j < 8; j++){
			desert.drawObject(eye, center, up, (j-3)*desert_size+(int)(eye.x/desert_size)*desert_size, 0, desert_size*i+(int)(eye.z/desert_size)*desert_size);
		}
	}
}

//Drawing procedure
void drawScene(GLFWwindow* window, float coin_rotation, std::vector<float> worm_rotation) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0.15, 1.0f);

	drawOBJ();
	FPP(window, coin_rotation, worm_rotation);

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
	std::vector <float> worm_rotation(2, 0.0f); // kąty rotacji segmentów robaka
	int flag = 1;
	glfwSetTime(0);
	while (!glfwWindowShouldClose(window))
	{		
		// ruch w osi x i z
		x_change += x_speed * glfwGetTime();
		z_change += z_speed * glfwGetTime();
		
		// rotacja monet
		coin_rotate_angle += coin_rotate_speed * glfwGetTime();

		// rotacja segmentów czerwia
		if (worm_rotation[0] >= PI/6) flag = 0;
		if (worm_rotation[0] <= -PI/6) flag = 1;
		if (flag){
			worm_rotation[0] += worm_wriggle_speed * glfwGetTime();
			worm_rotation[1] -= worm_wriggle_speed * glfwGetTime();
		}
		else{
			worm_rotation[0] -= worm_wriggle_speed * glfwGetTime();
			worm_rotation[1] += worm_wriggle_speed * glfwGetTime();
		}

		glfwSetTime(0);
		updateCamera();
		drawScene(window, coin_rotate_angle, worm_rotation);
		glfwPollEvents(); //Process callback procedures corresponding to the events that took place up to now
	}
	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Delete OpenGL context and the window.
	glfwTerminate(); //Free GLFW resources
	exit(EXIT_SUCCESS);
}