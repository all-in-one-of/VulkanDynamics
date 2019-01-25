#include <stdlib.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

bool stopAnimation = true;
static int lightNumber = 0;
static int	lightMode = 0;
static bool lightOn = false;
int motionMode = 0;
int startX = 0;
int startY = 0;

int pointx;
int pointy;

bool activateTrans = false;

float lightPositionx = 0.0;
float lightPositionz = 0.0;
float lightPositiony = 0.0;
float theta = 0.0;
float phi = 0.0;
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
	switch (key) {
		case  0x1B: /* esc */
		case  'q':
			//    kill( getpid(), SIGHUP );
			break;
		case ' ':
			break;

		case ',':
			break;

		case '.':
			break;
		case 't':
			activateTrans = !activateTrans;
			break;
		case 's':
			stopAnimation = !stopAnimation;

			break;

		case 'o':
			lightOn = !lightOn;
			break;

		case '1':


			break;
		case 'm':
			break;
		case 'e':
			lightPositiony += 1.0;
			break;
		case 'd':
			lightPositiony -= 1.0;
			break;
		default:
			break;
	}
	
}

/*
void readSpecialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		//phi += 1 / 100.0;

		//theta += 1 / 50.0;

		lightPositionx += 1.0;

		//startRot ++;
		//std::cout << "fff\n";

		break;

	case GLUT_KEY_LEFT:
		//theta -= 1 / 50.0;
		lightPositionx -= 1.0;
		break;

	case GLUT_KEY_DOWN:
		lightPositionz -= 1.0;
		break;

	case GLUT_KEY_UP:
		lightPositionz += 1.0;
		break;

	}
	//glutPostRedisplay( );
}

void mouseButtHandler(int button, int state, int x, int y)
{
	motionMode = 0;
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			motionMode = 1;
			startX = x;
			startY = y;
		}
		break;
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_DOWN) {
			motionMode = 2;
			startX = x;
			startY = y;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			motionMode = 3;
			startX = x;
			startY = y;
		}
		break;
	}
	//glutPostRedisplay( );
}

void mouseMoveHandler(int x, int y) {
	switch (motionMode) {
	case 0:
		break;

	case 1:
		//if ( x > 300 ) lookAtxAngle +=1 ;
		//if ( x < 300 ) lookAtxAngle -=1 ;
		//lookAtyAngle += (300  - y) / 50 ;

		phi = phi + (x - startX) / 100.0;
		startX = x;
		theta = theta - (y - startY) / 100.0;
		startY = y;

		break;

	case 2:
		//xdistance = xdistance - (x - startX)/100.0;
		//ydistance = ydistance - (y - startY)/100.0;
		//startX = x;
		//startY = y;
		eyer = eyer + (y - startY) / 10.0;
		startY = y;

		break;

	case 3:
		//zdistance = zdistance - (y - startY)/10.0;
		//startX = x;
		//startY = y;
		pointx = x;
		pointy = y;
		break;
	}
	//glutPostRedisplay( );
}
*/