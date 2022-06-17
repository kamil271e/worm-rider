#define GLM_FORCE_RADIANS

#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../lib/constants.h"

extern float x_speed;
extern float z_speed;
extern float x_max_speed;
extern float z_max_speed;
extern float worm_wriggle_speed; 
extern float coin_rotate_speed;

extern double x_cursor;
extern double y_cursor;
extern float x_window;
extern float y_window;

extern float x_change;
extern float z_change;

void updateMovement();

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);