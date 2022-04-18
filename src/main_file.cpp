#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "../lib/constants.h"
#include "../lib/allmodels.h"
#include "../lib/lodepng.h"
#include "../lib/shaderprogram.h"

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
glm::vec3 up = glm::vec3(glm::vec3(0.0f, 2.0f, 0.0f));

//Error processing callback procedure
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Initialization code procedure
void initOpenGLProgram(GLFWwindow* window) {
	initShaders();
	glEnable(GL_DEPTH_TEST);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	//************Place any code here that needs to be executed once, at the program start************
}

//Release resources allocated by the program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
	//************Place any code here that needs to be executed once, after the main loop ends************
}

void initCamera(){
	eye = glm::vec3(x_change, 0.0f, z_change-10.0f);
	center = glm::vec3(x_change - (float)(x_cursor)/x_window*10, 0.0f, 0.0f);
}

void drawObjects(glm::mat4 &P, glm::mat4 &V){
	spLambert->use();//Activate shader program
	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V));

	glm::mat4 M = glm::mat4(1.0);
	M = rotate(M, -PI/3, glm::vec3(0,1,0));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M));
	glUniform4f(spLambert->u("color"), 0, 0.5, 0.1, 1);
	Models::cube.drawSolid();

	M = translate(M, glm::vec3(-7,0,-2));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M));
	glUniform4f(spLambert->u("color"), 0.2, 0.5, 1, 1);
	Models::teapot.drawSolid();

	M = translate(M, glm::vec3(1,0,-4));
	M = rotate(M, -PI/2, glm::vec3(0,1,0));
	M = scale(M, glm::vec3(1.0, 0.5, 0.1));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M));
	glUniform4f(spLambert->u("color"), 1, 0, 0, 1);
	Models::cube.drawSolid();

	M = glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(1.2f, 4.0f, 2.0f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M)); 
	glUniform4f(spLambert->u("color"), 0, 1, 0, 1);
	Models::torus.drawSolid();
}

// First Person Perspecitve
void FPP(GLFWwindow* window){
	initCamera();
	glm::mat4 V = glm::lookAt(eye, center, up);
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f);
	drawObjects(P,V);

	glfwGetCursorPos(window, &x_cursor, &y_cursor);
	std::cout << "X:" << x_cursor << std::endl;
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

//Drawing procedure
void drawScene(GLFWwindow* window) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	FPP(window);

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
	glfwSetTime(0);
	while (!glfwWindowShouldClose(window)) //As long as the window shouldnt be closed yet...
	{		
		x_change += x_speed * glfwGetTime();
		z_change += z_speed * glfwGetTime();

		glfwSetTime(0);
		drawScene(window); //Execute drawing procedure
		glfwPollEvents(); //Process callback procedures corresponding to the events that took place up to now
	}
	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Delete OpenGL context and the window.
	glfwTerminate(); //Free GLFW resources
	exit(EXIT_SUCCESS);
}