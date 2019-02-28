#ifndef  __INPUTHANDLER_H
#define  __INPUTHANDLER_H

#include <stdlib.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

bool stopAnimation = true;
static int lightNumber = 0;
static int	lightMode = 0;
static bool lightOn = false;
int motionMode = 0;
double startX = 0;
double startY = 0;

double pointx;
double pointy;

bool activateTrans = false;

float lightPositionx = 0.0;
float lightPositionz = 0.0;
float lightPositiony = 0.0;
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
	else if (key == GLFW_KEY_O && action == GLFW_PRESS)
		lightOn = !lightOn;

	else if (key == GLFW_KEY_E && action == GLFW_PRESS)
		lightPositiony += 1.0;

	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
		lightPositiony -= 1.0;

	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		lightPositionx += 1.0;

	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		lightPositionx -= 1.0;

	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		lightPositionz -= 1.0;

	else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		lightPositionz += 1.0;

	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {

	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {

	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {

	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {

	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {

	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {

	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {

	}
}

void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos) {
	pointx = xpos;
	pointy = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		std::cout << "moooo" << std::endl;
		startX = pointx;
		startY = pointy;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_REPEAT) {
		std::cout << pointx << std::endl;
		phi = phi + (pointx - startX) / 100.0;
		startX = pointx;
		theta = theta - (pointy - startY) / 100.0;
		startY = pointy;
	}
}

#endif