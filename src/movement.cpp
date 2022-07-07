#include "../lib/movement.h"

float x_speed = 0.0f; // [radians/s]
float z_speed = 0.0f;
float x_max_speed = 1.5*PI; // [radians/s]
float z_max_speed = 1.5*PI;
float worm_wriggle_speed = 1.0f; 
float coin_rotate_speed = 2.0f;

double x_cursor = 0.0f;
double y_cursor = 0.0f;
float x_window = 1000.0f;
float y_window = 1000.0f;

float x_change = 0;
float z_change = 0;

bool w_pressed=false, a_pressed=false, d_pressed=false;
void updateMovement(){
	if (w_pressed && z_speed <= z_max_speed) z_speed += 1.5*PI/100;                         // forward
	else if (!w_pressed && z_speed > 0) z_speed -= 1.5*PI/20;                               // slow down
	else if (z_speed < 0) z_speed = 0;                                                      // stop
	if (d_pressed && z_speed > 0 && x_speed >= -x_max_speed) x_speed -= 1.5*PI/100;         // to right
	else if (x_speed < 0 && (!z_speed || (!d_pressed && !a_pressed))) x_speed += 1.5*PI/20; // slow down to right
	if (a_pressed && z_speed > 0 && x_speed <= x_max_speed) x_speed += 1.5*PI/100;          // to left
	else if (x_speed > 0 && (!z_speed || (!a_pressed && !d_pressed))) x_speed -= 1.5*PI/20; // slow down to left
	if (!d_pressed && !a_pressed && abs(x_speed) < 0.2) x_speed = 0;                        // stop
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_A) a_pressed=true;
		if (key == GLFW_KEY_D) d_pressed=true;
		if (key == GLFW_KEY_W) w_pressed=true;
		if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_A) a_pressed=false;
		if (key == GLFW_KEY_D) d_pressed=false;
		if (key == GLFW_KEY_W) w_pressed=false;
	}
}