#ifndef  __INPUTHANDLER_H__
#define  __INPUTHANDLER_H__

#include <stdlib.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

bool stopAnimation = true;
int motionMode = 0;
double startX = 0;
double startY = 0;

bool lbutton_down = false;

double pointx;
double pointy;

bool activateTrans = false;

float lightPositionx = 0.0;
float lightPositionz = 0.0;
float lightPositiony = 0.0;
float lightPositionIncrement = 0.7;
char changeLightPos[3] = {};
double theta = 0.0;
double phi = 0.0;
float eyex;
float eyey;
float eyez;
float eyer = -5.0;
float gazex;
float gazey;
float gazez = -5;
float angle = 0;    // in degrees
float angle2 = 0;   // in degrees 
float angle3 = 0;   // in degrees

GLfloat zdistance = 0.0;

void readInput_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		stopAnimation = !stopAnimation;
	}
	else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		//    kill( getpid(), SIGHUP );
		exit(0);
	}

	else if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		lightPositiony = lightPositionIncrement;
		changeLightPos[1] = 1;
	}

	else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		lightPositiony = -lightPositionIncrement;
		changeLightPos[1] = 1;
	}

	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		lightPositionx = lightPositionIncrement;
		changeLightPos[0] = 1;
	}

	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		lightPositionx = -lightPositionIncrement;
		changeLightPos[0] = 1;
	}

	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		lightPositionz = -lightPositionIncrement;
		changeLightPos[2] = 1;
	}

	else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		lightPositionz = lightPositionIncrement;
		changeLightPos[2] = 1;
	}

	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {

	}
}

void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos) {
	pointx = xpos;
	pointy = ypos;

	if (lbutton_down) {
		std::cout << pointx << std::endl;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	if (button == GLFW_MOUSE_BUTTON_LEFT ) {
		if (GLFW_PRESS == action) {
			lbutton_down = true;
			startX = pointx;
			startY = pointy;
			
		}
		else if (GLFW_RELEASE == action)
			lbutton_down = false;
	}
}
/*
static void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (GLFW_PRESS == action)
			lbutton_down = true;
		else if (GLFW_RELEASE == action)
			lbutton_down = false;
	}

	if (lbutton_down) {
		// do your drag here
	}
}
*/
#endif 