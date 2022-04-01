/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "../lib/constants.h"
#include "../lib/allmodels.h"
#include "../lib/lodepng.h"
#include "../lib/shaderprogram.h"

float x_speed = 0.0f; // [radians/s]
float z_speed = 0.0f;

//Error processing callback procedure
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Initialization code procedure
void initOpenGLProgram(GLFWwindow* window) {
	initShaders();
	glEnable(GL_DEPTH_TEST);
	//************Place any code here that needs to be executed once, at the program start************

}

//Release resources allocated by the program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
	//************Place any code here that needs to be executed once, after the main loop ends************
}

void player_perspecitve(){
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

void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) x_speed = 1.5*PI;
		if (key == GLFW_KEY_RIGHT) x_speed = -1.5*PI;
		if (key == GLFW_KEY_UP) z_speed = 1.5*PI;
		if (key == GLFW_KEY_DOWN) z_speed = -1.5*PI;
	}

	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) x_speed = 0;
		if (key == GLFW_KEY_DOWN || key == GLFW_KEY_UP) z_speed = 0;
	}
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset){
	if (yOffset < 0) z_speed = -1.5*PI;
	else z_speed = 1.5*PI;
}

//Drawing procedure
void drawScene(GLFWwindow* window, float x_angle, float z_angle) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 V = glm::lookAt(
		glm::vec3(x_angle, 0.0f, z_angle),
		glm::vec3(x_angle, 0.0f, 0.0f),
		glm::vec3(0.0f, 2.0f, 0.0f)); //DEFAULT
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f);
	
	spLambert->use();//Activate shader program
	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V));

	player_perspecitve();

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

	window = glfwCreateWindow(1000, 1000, "OpenGL", NULL, NULL);  //Create a window 500pxx500px titled "OpenGL" and an OpenGL context associated with it. 

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
	float x_angle = 0;
	float z_angle = 0;
	glfwSetTime(0);
	while (!glfwWindowShouldClose(window)) //As long as the window shouldnt be closed yet...
	{		
		x_angle += x_speed * glfwGetTime();
		z_angle += z_speed * glfwGetTime();
		glfwSetTime(0);
		drawScene(window, x_angle, z_angle); //Execute drawing procedure
		glfwPollEvents(); //Process callback procedures corresponding to the events that took place up to now
	}
	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Delete OpenGL context and the window.
	glfwTerminate(); //Free GLFW resources
	exit(EXIT_SUCCESS);
}
