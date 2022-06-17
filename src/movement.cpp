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

int w_pressed=0, a_pressed=0, d_pressed=0;
void updateMovement(){
	if (w_pressed && z_speed <= z_max_speed) z_speed += 1.5*PI/100;                         // do przodu
	else if (!w_pressed && z_speed > 0) z_speed -= 1.5*PI/20;                               // zwalnianie
	else if (z_speed < 0) z_speed = 0;                                                      // zatrzymywanie
	if (d_pressed && z_speed > 0 && x_speed >= -x_max_speed) x_speed -= 1.5*PI/100;         // w prawo
	else if (x_speed < 0 && (!z_speed || (!d_pressed && !a_pressed))) x_speed += 1.5*PI/20; // zwalnianie w prawo
	if (a_pressed && z_speed > 0 && x_speed <= x_max_speed) x_speed += 1.5*PI/100;          // w lewo
	else if (x_speed > 0 && (!z_speed || (!a_pressed && !d_pressed))) x_speed -= 1.5*PI/20; // zwalnianie w lewo
	if (!d_pressed && !a_pressed && abs(x_speed) < 0.2) x_speed = 0;                        // zatrzymywanie
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_A) a_pressed=1;
		if (key == GLFW_KEY_D) d_pressed=1;
		if (key == GLFW_KEY_W) w_pressed=1;
		if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_A) a_pressed=0;
		if (key == GLFW_KEY_D) d_pressed=0;
		if (key == GLFW_KEY_W) w_pressed=0;
	}
}