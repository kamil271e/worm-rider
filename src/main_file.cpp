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
#include "../lib/OBJ_loader.h"
#include "../lib/movement.h"
#include "coin.cpp"
#include "worm.cpp"
#include "object.cpp"
#include "skull.cpp"
#include "spaceship.cpp"

glm::vec3 eye;
glm::vec3 center;
glm::vec3 up = glm::vec3(glm::vec3(0.0f, 5.0f, 0.0f));

GLuint sandTex, goldTex, skyTex, skinTex, furTex, boneTex, shipTex;
std::vector<Coin> coinVector;
std::vector<Skull> skullVector;
std::vector<Spaceship> spaceshipVector;
Worm worm;
objl::Loader loader;
float desert_size = 15.0f;

const int nr_of_spaceships = 10;
const int nr_of_skulls = 1;
const int nr_of_coins = 3;
float enemy_max_x=15, enemy_min_z=100, enemy_max_z=300;
float enemy_speed = 8.0f;

//Error processing callback procedure
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void initCoins(){
	float* init_x = new float[nr_of_coins] {5.0f,-3.0f,0.0f};
	float* init_z = new float[nr_of_coins] {20.0f, 30.0f, 40.0f};
	for (int i = 0; i < nr_of_coins; i++){
		Coin coin(init_x[i], init_z[i], 0.0f, goldTex);
		coinVector.push_back(coin);
	}
	delete[] init_x;
	delete[] init_z;
}

void initSkulls(){
	float init_x = -5.0f;
	float init_z = 20.0f;
	Skull skull(init_x, init_z, boneTex);
	skullVector.push_back(skull);
}

void initSpaceships(){
	for (int i=0; i<nr_of_spaceships; i++){
		float init_x = randomNum(eye.x-enemy_max_x, eye.x+enemy_max_x);
		float init_z = randomNum(eye.z+enemy_min_z, eye.z+enemy_max_z);
		Spaceship tempSpaceship(init_x, init_z, shipTex);
		spaceshipVector.push_back(tempSpaceship);
	}
}

//Initialization code procedure
void initOpenGLProgram(GLFWwindow* window) {
	goldTex = readTexture("tex/gold.png");
	sandTex = readTexture("tex/pink_sand.png"); // lepiej cos ciemniejszego
	skyTex = readTexture("tex/red_sky.png");
	skinTex = readTexture("tex/worm_skin.png");
	furTex = readTexture("tex/fur.png");
	boneTex = readTexture("tex/bone.png");
	shipTex = readTexture("tex/spaceship.png");
	
	initShaders();
	initCoins();
	initSkulls();
	initSpaceships();
	glEnable(GL_DEPTH_TEST);
	
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

//Release resources allocated by the program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
	glDeleteTextures(1,&goldTex);
	glDeleteTextures(1,&sandTex);
	glDeleteTextures(1,&skyTex);
	//************Place any code here that needs to be executed once, after the main loop ends************
}

void updateCamera(){
	eye = glm::vec3(x_change, 3.0f, z_change);
	center = glm::vec3(x_change - (float)(x_cursor)/x_window*20, 0.0f, z_change+10);
}

void drawCoins(float coin_rotation){
	for (int i = 0; i < 3; i++){ // 3 coiny jednoczesnie na scenie
		coinVector[i].rotation = coin_rotation;
		if (!coinVector[i].drawCoin(eye, center, up, spPhong)){
			// domyslnie nowo pojawiajace sie monety bede mialy wspolrzedne x:[E-6f,E+6f), z:[N+10f,N+20f),
			// gdzie N to pozycja ostatniej monety w wektorze (najdalszej od obserwatora)
			// E pozycja oka
			remove(coinVector, i); 
			float temp_x = randomNum(eye.x-6.0f, eye.x+6.0f);
			float temp_z = randomNum(coinVector.back().z+10.0f, coinVector.back().z+20.f);
			Coin temp_coin(temp_x, temp_z, coin_rotation, goldTex, skyTex);
			coinVector.push_back(temp_coin);
		}
	}	
}

void drawSkulls(){
	//static Object skull2("obj/skull.obj", boneTex);
	//skull2.drawObject(eye, center, up, glm::vec3(0.0f,-0.5f,20.0f), glm::vec3(5.0f,0.0f,10.0f), glm::vec3(0.1f), spLambertTextured);

	if(!skullVector[0].drawSkull(eye, center, up, spLambertTextured)){
		remove(skullVector, 0);
		float temp_x = randomNum(eye.x-8.0f, eye.x+8.0f);
		float temp_z = eye.z+70.0f;
		Skull temp_skull(temp_x, temp_z, boneTex);
		skullVector.push_back(temp_skull);
	}
}

void drawSpaceships(){
	for (int i=0; i<nr_of_spaceships; i++){
		if(!spaceshipVector[i].drawSpaceship(eye, center, up, spaceshipVector[i].z_change, spLambertTextured)){
			remove(spaceshipVector, i);
			float temp_x = randomNum(eye.x-enemy_max_x, eye.x+enemy_max_x);
			float temp_z = randomNum(eye.z+enemy_min_z, eye.z+enemy_max_z);
			Spaceship tempSpaceship(temp_x, temp_z, boneTex);
			spaceshipVector.push_back(tempSpaceship);
		}
	}
}

void drawDesert(){
	static Object desert("obj/desert.obj", sandTex);
	for (int i = 0; i < 6; i++){
		for (int j = 0; j < 8; j++){
			glm::vec3 pos = glm::vec3((j-3)*desert_size+(int)(eye.x/desert_size)*desert_size, 0, desert_size*i+(int)(eye.z/desert_size)*desert_size);
			desert.drawObject(eye, center, up, pos, glm::vec3(0.0f), glm::vec3(1.0f), spLambertTextured);
		}
	}
	
}

void drawSkyBox(){
	static Object skybox("obj/dome.obj", skyTex, 0, true);
	skybox.drawObject(eye, center, up, glm::vec3(eye.x,-5.0f,eye.z+20.0f), glm::vec3(0.0f), glm::vec3(1.6f), spTextured);
}

//Drawing procedure
void drawScene(GLFWwindow* window, float coin_rotation, std::vector<float> worm_rotation) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0.15, 1.0f);

	worm.drawWorm(eye, center, up, worm_rotation, spLambert, skinTex, furTex);
	drawCoins(coin_rotation);
	drawSkulls();
	drawSpaceships();
	drawDesert();
	drawSkyBox();
	glfwGetCursorPos(window, &x_cursor, &y_cursor);
	
	//std::cout << "X:" << x_cursor << std::endl;
	//std::cout << "(X:" << eye.x << ", Y:" << eye.y << ", Z:" << eye.z << ")" << std::endl;

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

	window = glfwCreateWindow(x_window, y_window, "Worm Rider", NULL, NULL);  //Create a window 500pxx500px titled "OpenGL" and an OpenGL context associated with it. 


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
		for (int i=0; i<nr_of_spaceships; i++) spaceshipVector[i].z_change -= enemy_speed * glfwGetTime();
		
		// rotacja monet
		coin_rotate_angle += coin_rotate_speed * glfwGetTime();

		// rotacja segmentów czerwia
		if (abs(x_speed) > 0.1 || z_speed > 0.1){
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
		}

		glfwSetTime(0);
		updateCamera();
		updateMovement();
		drawScene(window, coin_rotate_angle, worm_rotation);
		glfwPollEvents(); //Process callback procedures corresponding to the events that took place up to now
	}
	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Delete OpenGL context and the window.
	glfwTerminate(); //Free GLFW resources
	exit(EXIT_SUCCESS);
}