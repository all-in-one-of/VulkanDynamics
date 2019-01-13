#include <stdlib.h>
#include <glut.h>
#include <iostream>



/*
float vertices1[] = {   -1.0f,-1.0f,-1.0f, // triangle 1 : begin
						-1.0f,-1.0f, 1.0f,
						-1.0f, 1.0f, 1.0f, // triangle 1 : end
						1.0f, 1.0f,-1.0f, // triangle 2 : begin
						-1.0f,-1.0f,-1.0f,
						-1.0f, 1.0f,-1.0f, // triangle 2 : end
						1.0f,-1.0f, 1.0f,
						-1.0f,-1.0f,-1.0f,
						1.0f,-1.0f,-1.0f,
						1.0f, 1.0f,-1.0f,
						1.0f,-1.0f,-1.0f,
						-1.0f,-1.0f,-1.0f,
						-1.0f,-1.0f,-1.0f,
						-1.0f, 1.0f, 1.0f,
						-1.0f, 1.0f,-1.0f,
						1.0f,-1.0f, 1.0f,
						-1.0f,-1.0f, 1.0f,
						-1.0f,-1.0f,-1.0f,
						-1.0f, 1.0f, 1.0f,
						-1.0f,-1.0f, 1.0f,
						1.0f,-1.0f, 1.0f,
						1.0f, 1.0f, 1.0f,
						1.0f,-1.0f,-1.0f,
						1.0f, 1.0f,-1.0f,
						1.0f,-1.0f,-1.0f,
						1.0f, 1.0f, 1.0f,
						1.0f,-1.0f, 1.0f,
						1.0f, 1.0f, 1.0f,
						1.0f, 1.0f,-1.0f,
						-1.0f, 1.0f,-1.0f,
						1.0f, 1.0f, 1.0f,
						-1.0f, 1.0f,-1.0f,
						-1.0f, 1.0f, 1.0f,
						1.0f, 1.0f, 1.0f,
						-1.0f, 1.0f, 1.0f,
						1.0f,-1.0f, 1.0f};//,
						//0.0f, 0.0f, //texture coords
						//1.0f, 0.0f,
						//1.0f, 0.0f,
						//0.0f, 0.0f};
 
float colors1[] = { 1.0f, 0.0f, 1.0f, 0.5f,
					1.0f, 0.0f, 1.0f, 0.5f,
					1.0f, 0.0f, 1.0f, 0.5f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f};

float normals1[] = {   -1.0f,-1.0f,-1.0f, // normal for triangle 1
						-1.0f,-1.0f, 1.0f,// normal for triangle 1
						-1.0f, 1.0f, 1.0f, // normal for triangle 1
						1.0f, 1.0f,-1.0f, // normal for triangle 2
						-1.0f,-1.0f,-1.0f, // normal for triangle 2
						-1.0f, 1.0f,-1.0f, // normal for triangle 2
						1.0f,-1.0f, 1.0f,
						-1.0f,-1.0f,-1.0f,
						1.0f,-1.0f,-1.0f,
						1.0f, 1.0f,-1.0f,
						1.0f,-1.0f,-1.0f,
						-1.0f,-1.0f,-1.0f,
						-1.0f,-1.0f,-1.0f,
						-1.0f, 1.0f, 1.0f,
						-1.0f, 1.0f,-1.0f,
						1.0f,-1.0f, 1.0f,
						-1.0f,-1.0f, 1.0f,
						-1.0f,-1.0f,-1.0f,
						-1.0f, 1.0f, 1.0f,
						-1.0f,-1.0f, 1.0f,
						1.0f,-1.0f, 1.0f,
						1.0f, 1.0f, 1.0f,
						1.0f,-1.0f,-1.0f,
						1.0f, 1.0f,-1.0f,
						1.0f,-1.0f,-1.0f,
						1.0f, 1.0f, 1.0f,
						1.0f,-1.0f, 1.0f,
						1.0f, 1.0f, 1.0f,
						1.0f, 1.0f,-1.0f,
						-1.0f, 1.0f,-1.0f,
						1.0f, 1.0f, 1.0f,
						-1.0f, 1.0f,-1.0f,
						-1.0f, 1.0f, 1.0f,
						1.0f, 1.0f, 1.0f,
						-1.0f, 1.0f, 1.0f,
						1.0f,-1.0f, 1.0f};

static const GLubyte tex_checkerboard_data[] =
{
	0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
	0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
	0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
	0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF
};

static const GLfloat tex_color_data[] =
{
// Red texel Green texel
1.0f, 0.0f, 0.0f, 1.0f, 
0.0f, 1.0f, 0.0f, 1.0f,
// Blue texel Yellow texel
0.0f, 0.0f, 1.0f, 1.0f, 
1.0f, 1.0f, 0.0f, 1.0f
};
*/