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
#include "../lib/allmodels.h"
#include "../lib/lodepng.h"
#include "../lib/shaderprogram.h"
#include "../lib/myCube.h"
#include "../lib/utils.h"
#include "coin.cpp"
#include "worm.cpp"
#include "../lib/OBJ_loader.h"

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

GLuint tex;
ShaderProgram *sp;

std::vector<Coin> CoinVector;
Worm worm;
objl::Loader loader;

const unsigned int myDesertVertexCount = 14406;

float myDesertVertices[myDesertVertexCount*4];
float myDesertNormals[myDesertVertexCount*4];
float myDesertTexCoords[myDesertVertexCount*2];

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

void loadOBJ(){
	// Initialize Loader
	objl::Loader Loader;

	// Load .obj File
	bool loadout = Loader.LoadFile("desert.obj");

	// Check to see if it loaded

	// If so continue
	if (loadout)
	{
		// Create/Open e1Out.txt
		std::ofstream file("e1Out.txt");

		// Go through each loaded mesh and out its contents
		for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
		{
			// Copy one of the loaded meshes to be our current mesh
			objl::Mesh curMesh = Loader.LoadedMeshes[i];

			// Print Mesh Name
			file << "Mesh " << i << ": " << curMesh.MeshName << "\n";

			// Print Vertices
			file << "Vertices:\n";

			// Go through each vertex and print its number,
			//  position, normal, and texture coordinate
			for (int j = 0; j < curMesh.Vertices.size(); j++)
			{
				file << "V" << j << ": " <<
					"P(" << curMesh.Vertices[j].Position.X << ", " << curMesh.Vertices[j].Position.Y << ", " << curMesh.Vertices[j].Position.Z << ") " <<
					"N(" << curMesh.Vertices[j].Normal.X << ", " << curMesh.Vertices[j].Normal.Y << ", " << curMesh.Vertices[j].Normal.Z << ") " <<
					"TC(" << curMesh.Vertices[j].TextureCoordinate.X << ", " << curMesh.Vertices[j].TextureCoordinate.Y << ")\n";
			}

			// Print Indices
			file << "Indices:\n";

			// Go through every 3rd index and print the
			//	triangle that these indices represent
			for (int j = 0; j < curMesh.Indices.size(); j += 3)
			{
				file << "T" << j / 3 << ": " << curMesh.Indices[j] << ", " << curMesh.Indices[j + 1] << ", " << curMesh.Indices[j + 2] << "\n";
			}

			// Print Material
			file << "Material: " << curMesh.MeshMaterial.name << "\n";
			file << "Ambient Color: " << curMesh.MeshMaterial.Ka.X << ", " << curMesh.MeshMaterial.Ka.Y << ", " << curMesh.MeshMaterial.Ka.Z << "\n";
			file << "Diffuse Color: " << curMesh.MeshMaterial.Kd.X << ", " << curMesh.MeshMaterial.Kd.Y << ", " << curMesh.MeshMaterial.Kd.Z << "\n";
			file << "Specular Color: " << curMesh.MeshMaterial.Ks.X << ", " << curMesh.MeshMaterial.Ks.Y << ", " << curMesh.MeshMaterial.Ks.Z << "\n";
			file << "Specular Exponent: " << curMesh.MeshMaterial.Ns << "\n";
			file << "Optical Density: " << curMesh.MeshMaterial.Ni << "\n";
			file << "Dissolve: " << curMesh.MeshMaterial.d << "\n";
			file << "Illumination: " << curMesh.MeshMaterial.illum << "\n";
			file << "Ambient Texture Map: " << curMesh.MeshMaterial.map_Ka << "\n";
			file << "Diffuse Texture Map: " << curMesh.MeshMaterial.map_Kd << "\n";
			file << "Specular Texture Map: " << curMesh.MeshMaterial.map_Ks << "\n";
			file << "Alpha Texture Map: " << curMesh.MeshMaterial.map_d << "\n";
			file << "Bump Map: " << curMesh.MeshMaterial.map_bump << "\n";

			// Leave a space to separate from the next mesh
			file << "\n";
		}

		// Close File
		file.close();
	}
	// If not output an error
	else
	{
		// Create/Open e1Out.txt
		std::ofstream file("e1Out.txt");

		// Output Error
		file << "Failed to Load File. May have failed to find it or it was not an .obj file.\n";

		// Close File
		file.close();
	}

}

void loadDesert(){
	bool loadout = loader.LoadFile("obj/desert.obj");
	if (loadout){
		std::cout << "Made it" << std::endl;
		std::cout << loader.LoadedMeshes[0].Vertices.size() << std::endl;
		for(int i = 0; i < myDesertVertexCount; i++){
			
			myDesertVertices[i*4] = loader.LoadedVertices[i].Position.X;
			myDesertVertices[i*4+1] = loader.LoadedVertices[i].Position.Y;
			myDesertVertices[i*4+2] = loader.LoadedVertices[i].Position.Z;
			myDesertVertices[i*4+3] = 1.0f;

			myDesertNormals[i*4] = loader.LoadedVertices[i].Normal.X;
			myDesertNormals[i*4+1] = loader.LoadedVertices[i].Normal.Y;
			myDesertNormals[i*4+2] = loader.LoadedVertices[i].Normal.Z;
			myDesertNormals[i*4+3] = 0.0f;

			myDesertTexCoords[i*4] = loader.LoadedVertices[i].TextureCoordinate.X;
			myDesertTexCoords[i*4+1] = loader.LoadedVertices[i].TextureCoordinate.Y;

			std::cout << i << std::endl;
		}
		/*objl::Mesh curMesh = loader.LoadedMeshes[0];
		for (int i = 0; i < myDesertVertexCount; i++){
			myDesertVertices[i*4] = curMesh.Vertices[i].Position.X;
			myDesertVertices[i*4+1] = curMesh.Vertices[i].Position.Y;
			myDesertVertices[i*4+2] = curMesh.Vertices[i].Position.Z;
			myDesertVertices[i*4+3] = 1.0f;

			myDesertNormals[i*4] = curMesh.Vertices[i].Normal.X;
			myDesertNormals[i*4+1] = curMesh.Vertices[i].Normal.Y;
			myDesertNormals[i*4+2] = curMesh.Vertices[i].Normal.Z;
			myDesertNormals[i*4+3] = 0.0f;

			myDesertTexCoords[i*4] = curMesh.Vertices[i].TextureCoordinate.X;
			myDesertTexCoords[i*4+1] = curMesh.Vertices[i].TextureCoordinate.Y;

			std::cout << i << std::endl;
		}*/
	}
}

//Initialization code procedure
void initOpenGLProgram(GLFWwindow* window) {
	tex = readTexture("tex/gold.png");
	initShaders();
	initCoins();
	glEnable(GL_DEPTH_TEST);
	//loadDesert();

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
	// std::cout << "X:" << x_cursor << std::endl;
	// std::cout << "(X:" << eye.x << ", Y:" << eye.y << ", Z:" << eye.z << ")" << std::endl;
}

//Drawing procedure
void drawScene(GLFWwindow* window, float coin_rotation, std::vector<float> worm_rotation) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0.15, 1.0f);

	FPP(window, coin_rotation, worm_rotation);

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