#include <iostream>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
//#include "glew\include\GL\glew.h"
#include "glew.h"
#include "glut.h"
#include <memory.h>
#include <math.h>
#include "mymath.h"

#include "inputHandler.h"
#include "sceneSetup.h"
#include "my_matrixStack.h"



#define PI       3.14159265358979323846

#define IMAGE_WIDTH  600
#define IMAGE_HEIGHT 600


 
void setTransMatrix(float *mat, float x, float y, float z);
void multiplyMatrix(float *a, float *b);
void xProduct( float *a, float *b, float *res);
void normalize(float *a);
void setIdentMatrix( float *mat, int size);
void placeCam(float posX, float posY, float posZ, float lookX, float lookY, float lookZ);
void setTransMatrix(float *mat, float x, float y, float z);
float * rotationMatrix(float x, float y, float z, float angle);
void outputCharacter(float x, float y, float z, char s) ;
void drawAxes ();
void setTop (  );

void my_GL_perspective(float viewangle, float aspectratio, float znear, float zfar);
void my_GL_lookAt( float eyex, float eyey, float eyez,float centx, float centy, float centz, float upx, float upy, float upz ) ;
void my_set_GL_translatef (float *mat, float x , float y , float z );
float* my_GL_rotationf ( float floatAngle , float x, float y, float z );


struct vec3
{
	float x;
	float y;
	float z;
};


typedef struct{
        float fieldOfView;
        float aspect;
        float nearPlane;
        float farPlane;
}perspectiveData;

int window;
int updateFlag;

perspectiveData pD;
 
// shader names
char *vertexFileName = "vertex1.txt";
char *fragmentFileName = "frag2.txt";
 
// program and shader Id
GLuint p,v,f;
 
// vert attrib locations
GLuint vertexLoc, colorLoc, normalLoc, vertexLoc2, colorLoc2;
 
// uniform var locations
GLuint projMatrixLoc, viewMatrixLoc, ambientLoc, normalMatrixLoc, lightColorLoc, lightPositionLoc, viewEyeMatrixLoc, 
shininessLoc, strengthLoc, eyeDirectionLoc , constantAttenuationLoc, linearAttenuationLoc, quadraticAttenuationLoc, tex_checkerboard, tex_color   ;
 
// vert array obj Id
GLuint vert[5];

//text arrary obj Id
GLuint text;

//texture
unsigned int tex = 0;

//GLuint loadBMP_custom(const char * imagepath);

//GLuint image = loadBMP_custom("my_texture.bmp");





// storage for matrices
float projMatrix[16];
float viewMatrix[16];
float eyeviewMatrix[16];
float ambientLight[3] = { 0.1f,  0.1f, 0.1f };
float lightColor[3] = { 1.0f, 0.0f, 0.0f };
float LightPosition[3] = { 0.0f, 3.0f, -3.0f};
float Shininess = 140;
float Strength = 60;
float EyeDirection[3] = { 0.0f,1.0f, -3.0f};
float ConstantAttenuation = 50;
float LinearAttenuation = 0;
float QuadraticAttenuation = 0;
float normalModelViewMatrix[9];

int frame=0,time,timebase=0;
char s[50];

static bool lightOn = false;

std::stack<float *> first;
float tempMatrix [16];


void xProduct( float *a, float *b, float *res) 
{
    res[0] = a[1] * b[2]  -  b[1] * a[2];
    res[1] = a[2] * b[0]  -  b[2] * a[0];
    res[2] = a[0] * b[1]  -  b[0] * a[1];
}
 
// normalize a vec3
void normalize(float *a) 
{
    float mag = sqrt(a[0] * a[0]  +  a[1] * a[1]  +  a[2] * a[2]);
    a[0] /= mag;
    a[1] /= mag;
    a[2] /= mag;
}

void copy_proj ( float * a )
{
	a[0] = projMatrix[0];
	a[1] = projMatrix[1];
	a[2] = projMatrix[2];
	a[3] = projMatrix[3];
	a[4] = projMatrix[4];
	a[5] = projMatrix[5];
	a[6] = projMatrix[6];
	a[7] = projMatrix[7];
	a[8] = projMatrix[8];
	a[9] = projMatrix[9];
	a[10] = projMatrix[10];
	a[11] = projMatrix[11];
	a[12] = projMatrix[12];
	a[13] = projMatrix[13];
	a[14] = projMatrix[14];
	a[15] = projMatrix[15];
}



// Matrix Opt. - In Opengl 3 we need to handle our own matrix.
 
// In this form : a = a * b; 
void multiplyMatrix(float *a, float *b) 
{
    float res[16];
 
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            res[j*4 + i] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                res[j*4 + i] += a[k*4 + i] * b[j*4 + k];
            }
        }
    }
    memcpy(a, res, 16 * sizeof(float));
}

void multiplyMatrix1vf(float *a, float *b) 
{
    float res[4];

	res[0] = a[0];
	res[1] = a[2];
	res[2] = a[3];
	res[3] = 1;

    for (int i = 0; i < 4; ++i) {
		for ( int j = 0 ; j < 4 ; ++j ){
			a[i] += res[j] * b[j*4+i];
		}
	}
        
    memcpy(a, res, 16 * sizeof(float));
}



// sets the square matrix mat to the ID matrix,
void setIdentMatrix( float *mat, int size) 
{
 
    // 0s
    for (int i = 0; i < size * size; ++i)
            mat[i] = 0.0f;
 
    // diagonal 1s
    for (int i = 0; i < size; ++i)
        mat[i + i * size] = 1.0f;
}

void my_GL_lookAt( float eyex, float eyey, float eyez,
                    float centx, float centy, float centz,
                    float upx, float upy, float upz ) {

    GLfloat fwd[3], side[3], up[3];
    GLfloat L[16];

    fwd[0] = centx - eyex;
    fwd[1] = centy - eyey;
    fwd[2] = centz - eyez;

    GLfloat fwd_length = sqrt((fwd[0]*fwd[0]) + (fwd[1]*fwd[1]) + (fwd[2]*fwd[2]));
    fwd[0] = fwd[0]/fwd_length;
    fwd[1] = fwd[1]/fwd_length;
    fwd[2] = fwd[2]/fwd_length;

    side[0] = fwd[1]*upz - fwd[2]*upy;
    side[1] = fwd[2]*upx - fwd[0]*upz;
    side[2] = fwd[0]*upy - fwd[1]*upx;

	GLfloat side_length = sqrt((side[0]*side[0]) + (side[1]*side[1]) + (side[2]*side[2]));
	side[0] = side[0]/side_length;
    side[1] = side[1]/side_length;
    side[2] = side[2]/side_length;

    up[0] = side[1]*fwd[2] - side[2]*fwd[1];
    up[1] = side[2]*fwd[0] - side[0]*fwd[2];
    up[2] = side[0]*fwd[1] - side[1]*fwd[0];

	float aux[16];
	float change_look_at[16] ;

	
    viewMatrix[0] = side[0];
    viewMatrix[4] = side[1];
    viewMatrix[8] = side[2];
    viewMatrix[12] = 0.0;

    viewMatrix[1] = up[0];
    viewMatrix[5] = up[1];
    viewMatrix[9] = up[2];
    viewMatrix[13] = 0.0;

    viewMatrix[2] = -fwd[0];
    viewMatrix[6] = -fwd[1];
    viewMatrix[10] = -fwd[2];
    viewMatrix[14] = 0.0;

    viewMatrix[3] = viewMatrix[7] = viewMatrix[11] = 0.0;
    viewMatrix[15] = 1.0;

	eyeviewMatrix[0] = side[0];
    eyeviewMatrix[4] = side[1];
    eyeviewMatrix[8] = side[2];
    eyeviewMatrix[12] = 0.0;

    eyeviewMatrix[1] = up[0];
    eyeviewMatrix[5] = up[1];
    eyeviewMatrix[9] = up[2];
    eyeviewMatrix[13] = 0.0;

    eyeviewMatrix[2] = -fwd[0];
    eyeviewMatrix[6] = -fwd[1];
    eyeviewMatrix[10] = -fwd[2];
    eyeviewMatrix[14] = 0.0;

    eyeviewMatrix[3] = eyeviewMatrix[7] = eyeviewMatrix[11] = 0.0;
    eyeviewMatrix[15] = 1.0;
	
	
    
	 my_set_GL_translatef (aux, -eyex, -eyey, -eyez);

	 multiplyMatrix(eyeviewMatrix,aux );
	 multiplyMatrix(viewMatrix, aux);
	 setTop ();
}

// Generates a rotation matrix.  Angle is in radian.
float * rotationMatrix(float x, float y, float z, float angle)
{
	float tempVec[3];
	tempVec[0] = x;
	tempVec[1] = y;
	tempVec[2] = z;
    normalize(tempVec);

	vec3 axis;
	axis.x = tempVec[0];
	axis.y = tempVec[1];
	axis.z = tempVec[2];

    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

	float mat[16] = {oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
				0.0,                                0.0,                                0.0,                                1.0};

    return mat;

}

void my_set_GL_translatef (float *mat, float x , float y , float z )
{
	mat[0] = 1.0;
	mat[1] = 0.0;
	mat[2] = 0.0;
	mat[3] = 0.0;

	mat[4] = 0.0;
	mat[5] = 1.0;
	mat[6] = 0.0;
	mat[7] = 0.0;

	mat[8] = 0.0;
	mat[9] = 0.0;
	mat[10] = 1.0;
	mat[11] = 0.0;

	mat[12] = x;
	mat[13] = y;
	mat[14] = z;
	mat[15] = 1.0;
}

float* my_GL_rotationf ( float floatAngle , float x, float y, float z )
{

		float d = sqrt ( pow(x, 2) + pow ( y, 2 ) + pow ( z , 2 ) );
		x = x / d ;
		y = y / d ;
		z = z / d ;

        float* rot = new float[16]; // auto set to identity

		float b = floatAngle * ( PI / 180.0 );

		float c = cosf(b);
		float ac = 1.00f - c;
		float s = sinf(b);

		rot[0] = (x * x) * ac + c;
		rot[1] = ((x * y) * ac) + (z * s);
		rot[2] = ((x * z) * ac) - (y * s);
		rot[3] = 0.0;

		rot[4] = ((y * x) * ac) - (z * s);
		rot[5] = (y * y) * ac + c;
		rot[6] = ((y * z) * ac) + (x * s);
		rot[7] = 0.0;

		rot[8] = ((z * x) * ac) + (y * s);
		rot[9] = ((z * y) * ac) - (x * s);
		rot[10] = ((z * z) * ac) + c;
		rot[11] = 0.0;

		rot[12] = 0.0;
		rot[13] = 0.0;
		rot[14] = 0.0;
		rot[15] = 1.0;

		return rot;
}

float* my_GL_scalef (  float x, float y, float z )
{

		

        float* rot = new float[16]; // auto set to identity

	

		rot[0] = x;
		rot[1] = 0;
		rot[2] = 0;
		rot[3] = 0;

		rot[4] = 0;
		rot[5] = y;
		rot[6] = 0;
		rot[7] = 0;

		rot[8] = 0;
		rot[9] = 0;
		rot[10] = z;
		rot[11] = 0;

		rot[12] = 0.0;
		rot[13] = 0.0;
		rot[14] = 0.0;
		rot[15] = 1.0;

		return rot;
}
 

// Transformation matrix mat with a translation
void setTransMatrix(float *mat, float x, float y, float z) {
 
    setIdentMatrix(mat,4);
    mat[12] = x;
    mat[13] = y;
    mat[14] = z;
}

void changeSize(int w, int h) {
 
    float ratio;

	pD.fieldOfView = 90.0;
	pD.aspect      = (float)IMAGE_WIDTH/IMAGE_HEIGHT;
	pD.nearPlane   = 0.1;
	pD.farPlane    = 50.0;
    // place viewport to be the entire window
    glViewport(0, 0, w, h);
    ratio = (1.0f * w) / h;
    //buildProjMatrix(53.13f, ratio, 1.0f, 30.0f);
	//my_GL_perspective ( 53.13f, ratio, 1.0f, 30.0f);
	my_GL_perspective(pD.fieldOfView, pD.aspect, pD.nearPlane, pD.farPlane );

}


void setTop (  )
{
	//for ( int i = 0 ; i < 9 ; ++ i )
	//{
		//normalModelViewMatrix[i] = viewMatrix[i];
	//}
		
		normalModelViewMatrix[0] = viewMatrix[0];
		normalModelViewMatrix[1] = viewMatrix[1];
		normalModelViewMatrix[2] = viewMatrix[2];
		normalModelViewMatrix[3] = viewMatrix[4];
		normalModelViewMatrix[4] = viewMatrix[5];
		normalModelViewMatrix[5] = viewMatrix[6];
		normalModelViewMatrix[6] = viewMatrix[8];
		normalModelViewMatrix[7] = viewMatrix[9];
		normalModelViewMatrix[8] = viewMatrix[10];
	/*
		normalModelViewMatrix[0] = eyeviewMatrix[0];
		normalModelViewMatrix[1] = eyeviewMatrix[1];
		normalModelViewMatrix[2] = eyeviewMatrix[2];
		normalModelViewMatrix[3] = eyeviewMatrix[4];
		normalModelViewMatrix[4] = eyeviewMatrix[5];
		normalModelViewMatrix[5] = eyeviewMatrix[6];
		normalModelViewMatrix[6] = eyeviewMatrix[8];
		normalModelViewMatrix[7] = eyeviewMatrix[9];
		normalModelViewMatrix[8] = eyeviewMatrix[10];*/
}

void my_calcNormals()
{
	
	int j = 1, indexer = 0;

	for ( int i = 0; i < (sizeof(normals1) / (3 * sizeof(float))) ; i++ )
	{
			if ( j > 3 ){
				j = 1;
				indexer = i * 3 ;
			}
			normals1[3*i] = vertices1[indexer+1] * vertices1[indexer+5] - vertices1[indexer+4] * vertices1[indexer+2];
			normals1[3*i+1] = vertices1[indexer+3] * vertices1[indexer+2] - vertices1[indexer] * vertices1[indexer+5];
			normals1[3*i+2] = vertices1[indexer] * vertices1[indexer+4] - vertices1[indexer+1] * vertices1[indexer+3];
			
			float mag = sqrt(normals1[3*i] * normals1[3*i]  +  normals1[3*i+1] * normals1[3*i+1]  +  normals1[3*i+2] * normals1[3*i+2]);
			normals1[3*i] /= mag;
			normals1[3*i+1] /= mag;
			normals1[3*i+2] /= mag;

			j++;	
	}
}

void setupBuffers() { //*******called once in main

	
	//my_calcNormals ();

    GLuint buffers[3];
 
    glGenVertexArrays(1, vert);

    // first triangle
    glBindVertexArray(vert[0]);
    // generate 3 buffers for vert and color, normals
    glGenBuffers(3, buffers);
    // bind buffer for vertices and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexLoc);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0);

	//glBindVertexArray(vert[1]);
	//glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    //glEnableVertexAttribArray(vertexLoc2);
    //glVertexAttribPointer(vertexLoc2, 3, GL_FLOAT, 0, 0, 0);
 
    // bind buffer for colors and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors2), colors2, GL_STATIC_DRAW);
    glEnableVertexAttribArray(colorLoc);
    glVertexAttribPointer(colorLoc, 4, GL_FLOAT, 0, 0, 0);

	//glBindBuffer(GL_ARRAY_BUFFER, buffers[4]);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(colors2), colors2, GL_STATIC_DRAW);
    //glEnableVertexAttribArray(colorLoc2);
    //glVertexAttribPointer(colorLoc2, 4, GL_FLOAT, 0, 0, 0);

	//bind normals
	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals1), normals1, GL_STATIC_DRAW);
	glEnableVertexAttribArray(normalLoc);
	glVertexAttribPointer(normalLoc, 3, GL_FLOAT, 0, 0, 0);

	// First, the black-and-white checkerboard texture...
	// Bind the texture (possibly creating it)
	glBindTexture(GL_TEXTURE_2D, tex_checkerboard);
	// Allocate storage for the texture data
	glTexStorage2D(GL_TEXTURE_2D, 4, GL_R8, 8, 8);
	// Specify the data for the texture
	glTexSubImage2D(GL_TEXTURE_2D, // target
	0, // First mipmap level
	0, 0, // x and y offset
	8, 8, // width and height
	GL_RED, GL_UNSIGNED_BYTE, // format and type
	tex_checkerboard_data); // data


	// Next, the color, floating-point data.
	// Bind the next texture
	glBindTexture(GL_TEXTURE_2D, tex_color);
	// Allocate storage
	glTexStorage2D(GL_TEXTURE_2D, 2, GL_RGBA32F, 2, 2);
	// Specify the data
	glTexSubImage2D(GL_TEXTURE_2D, // target
	0, // First mipmap level
	0, 0, // x and y offset
	2, 2, // width and height
	GL_RGBA, GL_FLOAT, // format and type
	tex_color_data); // data
	
}
 


void my_GL_perspective(float viewangle, float aspectratio, float znear, float zfar)
{
	
	setIdentMatrix(projMatrix,4);
	float r = znear * aspectratio * tan(viewangle / 2.0);
	float l = -r; 
	float t = znear * tan(viewangle / 2.0);
	float b = -t;
	float n = znear;
	float f = zfar;
	//my_GL_frustum( l,  r,  b,  t,  n,  f);

	projMatrix[0] = 2.0*n / (r-l);
	projMatrix[1] = 0.0;
	projMatrix[2] = 0.0;
	projMatrix[3] = 0.0;

	projMatrix[4] = 0.0;
	projMatrix[5] = 2.0*n / (t-b);
	projMatrix[6] = 0.0;
	projMatrix[7] = 0.0;

	projMatrix[8] = (r+l) / (r-l);
	projMatrix[9] = (t+b) / (t-b);
	projMatrix[10] = - (f+n) / (f-n); // 
	projMatrix[11] = - 1.0;

	projMatrix[12] = 0.0;
	projMatrix[13] = 0.0;
	projMatrix[14] = - 2.0*f*n / (f-n);
	projMatrix[15] = 0.0;

	// Multiply current matrix on stack by F.
	//glMultMatrixf(F);
}

void setUniforms() {
 
    // must be called after glUseProgram
	// set the variables for the shader
	//vertex
    glUniformMatrix4fv(projMatrixLoc,  1, false, projMatrix);
    glUniformMatrix4fv(viewMatrixLoc,  1, false, viewMatrix);
	glUniformMatrix3fv(normalMatrixLoc, 1,false , normalModelViewMatrix );
	glUniformMatrix4fv(viewEyeMatrixLoc,  1, false, eyeviewMatrix);

	//frag
	glUniform3fv(ambientLoc, 1, ambientLight );
	glUniform3fv(lightColorLoc, 1, lightColor );
	glUniform3fv(lightPositionLoc, 1, LightPosition );
	glUniform1f(shininessLoc,Shininess );
	glUniform1f(strengthLoc,Strength );
	glUniform3fv(eyeDirectionLoc, 1, EyeDirection );
	glUniform1f(constantAttenuationLoc,ConstantAttenuation );
	glUniform1f(linearAttenuationLoc,LinearAttenuation );
	glUniform1f(quadraticAttenuationLoc,QuadraticAttenuation );

}

void renderScene(void) {

	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(s,"FPS:%4.2f",
			frame*1000.0/(time-timebase));
		timebase = time;
		frame = 0;
	}

    glutSetWindowTitle(s);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	if ( activateTrans )
	{
		glEnable (GL_BLEND);
		glDisable (GL_DEPTH_TEST);
	}
 
	eyex = eyer * sin(theta) * cos(phi); //mouse movement	
	eyey = eyer * cos(theta);
	eyez = eyer * sin(theta) * sin(phi);

	my_GL_lookAt ( eyex,eyey,eyez,    0,0,0,    0,1.0,0 );

	EyeDirection[0] = eyex ;
	EyeDirection[1] = eyey ;
	EyeDirection[2] = eyez ;

	my_push_matrix (viewMatrix );
	
	
	if ( !stopAnimation)
	{
		  //glMultMatrixf ( my_GL_rotationf( angle2, 1.0, 0.0, 0.0 ));
		  //glMultMatrixf ( my_GL_rotationf( angle, 0.0, 1.0, 0.0 ));
		  //glMultMatrixf ( my_GL_rotationf( angle3 , 0.0, 0.0, 1.0));
		  multiplyMatrix(viewMatrix, my_GL_rotationf(angle2, 1.0, 0.0, 0.0));
		 // multiplyMatrix(viewMatrix, my_GL_scalef( 2.0, 1.0, 1.0));
		  
		  angle2 += 1;
	}
	//my_pop_matrix(viewMatrix );

	LightPosition[0] = lightPositionx ;
	LightPosition[2] = lightPositionz ;
	LightPosition[1] = lightPositiony ;
	
	//std::cout << lightPositionx << "     " << lightPositiony << "     " << lightPositionz <<  "\n" ;

	std::cout << pointx << "   " << pointy << "\n" ;


	float samplePoint1[4] = {1.0f, 1.0f, 1.0f, 1.0f };
	float aux2[16] ;//= projMatrix * viewMatrix * samplePoint1;
	copy_proj ( aux2 );
	multiplyMatrix (aux2, viewMatrix );
	//multiplyMatrix1vf ( samplePoint1, aux2 );

	//std::cout << samplePoint1  << "\n" ;

	float bArray[4]; 
   glReadPixels(pointx, pointy, 1, 1, GL_RGBA, GL_FLOAT, bArray);

   std::cout << bArray[0] << "  " << bArray[1]<< "  " << bArray[2]<< "  " << bArray[3] <<  "\n";
	
    glUseProgram(p);
	setTop();
    setUniforms();

	
	//my_set_GL_translatef (aux, -eyex, -eyey, -eyez);
	//multiplyMatrix(viewMatrix, aux);
    glBindVertexArray(vert[0]);
    glDrawArrays(GL_TRIANGLES, 0, 12*3);

	/*
	//*************DRAW NEXT CUBE
	my_push_matrix(viewMatrix );
	float aux[16];
	my_set_GL_translatef ( aux, 1.0f,3.0f, 1.0f );


	 multiplyMatrix(viewMatrix, aux);
	 setUniforms();

	 glBindVertexArray(vert[0]);
     glDrawArrays(GL_TRIANGLES, 42, 12*3);

	 my_pop_matrix(viewMatrix );
	 setUniforms();
	 //END DRAW NEW CUBE
	 */
	


	
	if ( activateTrans )
	{
		glEnable (GL_DEPTH_TEST);
		glDisable (GL_BLEND);
	}
	my_pop_matrix(viewMatrix );
	

	
	setUniforms();
	
	glBindVertexArray(vert[0]);
	glLineWidth(2.0) ;
    glDrawArrays(GL_LINES, 36, 6);



	
	
	// tex is the name of a texture object that has been
	// initialized with some texture data
	glBindTexture(GL_TEXTURE_2D, tex);
	// Simple quad with texture coordinates
	static const GLfloat quad_data[] =
	{
			// Vertex positions
			-1.0f, -1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f, 1.0f,
			// Texture coordinates
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 0.0f
	};
	// Create and initialize a buffer object
	GLuint buf;

	glGenBuffers(1, &buf);
	glBindBuffer(GL_ARRAY_BUFFER, buf);
	glBufferData(GL_ARRAY_BUFFER,sizeof(quad_data), quad_data, GL_STATIC_DRAW);
	// Setup vertex attributes
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
	(GLvoid*)(16 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Ready. Draw.
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);



    glutSwapBuffers();
}
 

void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
 
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
 
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}
 
void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
 
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
 
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}
 
GLuint initShaders() {
 

	
    char *vertShader = NULL,*fragShader = NULL;
 
    GLuint p,v,f;
 
    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);
    vertShader = getTxtFile(vertexFileName);
    fragShader = getTxtFile(fragmentFileName);
    const char * vv = vertShader;
    const char * ff = fragShader;
    glShaderSource(v, 1, &vv,NULL);
    glShaderSource(f, 1, &ff,NULL);
    free(vertShader);
	free(fragShader);
    glCompileShader(v);
    glCompileShader(f);
    printShaderInfoLog(v);
    printShaderInfoLog(f);
    p = glCreateProgram();
    glAttachShader(p,v);
    glAttachShader(p,f);
    glBindFragDataLocation(p, 0, "outputF");
    glLinkProgram(p);
    printProgramInfoLog(p);
	//vertex shader
	//in vars
    vertexLoc = glGetAttribLocation(p,"position");
    colorLoc = glGetAttribLocation(p, "color"); 
	normalLoc = glGetAttribLocation(p, "VertexNormal"); 
	//uniforms
    projMatrixLoc = glGetUniformLocation(p, "projMatrix");
    viewMatrixLoc = glGetUniformLocation(p, "viewMatrix");
	normalMatrixLoc = glGetUniformLocation (p, "NormalMatrix" );
	viewEyeMatrixLoc = glGetUniformLocation (p, "eyeViewMatrix" );

	//fragment shader
	ambientLoc = glGetUniformLocation(p,"Ambient" );
	lightColorLoc = glGetUniformLocation(p,"LightColor" );
	lightPositionLoc = glGetUniformLocation(p,"LightPosition" );
	shininessLoc = glGetUniformLocation(p,"Shininess" );
	strengthLoc = glGetUniformLocation(p,"Strength" );
	eyeDirectionLoc = glGetUniformLocation(p,"EyeDirection" );
	constantAttenuationLoc = glGetUniformLocation(p,"ConstantAttenuation" );
	linearAttenuationLoc = glGetUniformLocation(p,"LinearAttenuation" );
	quadraticAttenuationLoc = glGetUniformLocation(p,"QuadraticAttenuation" );
	
    return(p);
}


void drawAxes ()
{
	/*glColor3f( 1.0, 1.0, 1.0 );
	glBegin(GL_LINES );
	glVertex4f( 0, 0, 0, 1);
	glVertex4f( 0, 2, 0, 1);
	glEnd();
	glBegin(GL_LINES );
	glVertex4f( 0, 0, 0, 1);
	glVertex4f( 2, 0, 0, 1);
	glEnd();
	glBegin(GL_LINES );
	glVertex4f( 0, 0, 0, 1);
	glVertex4f( 0, 0, 2, 1);
	glEnd();

	glColor3f (1.0, 1.0, 1.0);
	*/
        outputCharacter(0.0f, 0.0f, 1.98f, 'z');
		 outputCharacter(0.0f, 1.98f, 0.0f, 'y');
		  outputCharacter(1.98f, 0.0f, 0.0f, 'x');
}

void outputCharacter(float x, float y, float z, char s) 
{
  
  glRasterPos4f(x,y, z,1.0);
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s );
  
}
 
int main(int argc, char **argv) 
{
	// sets up glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(640,480);
    glutCreateWindow("ICS Graphics");
    glutSetWindowTitle(s);
	// call back functions
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);
	glutKeyboardFunc( readKeyboard );
	glutSpecialFunc(readSpecialKeys);
	glutMouseFunc(mouseButtHandler);
	glutMotionFunc(mouseMoveHandler);
	glutPassiveMotionFunc( mouseMoveHandler );

	// check if a particular extension is available on your platform
    glewInit();
    if (glewIsSupported("GL_VERSION_3_3"))
        printf("OpenGL 3.3 is supported\n");
    else 
	{
        printf("OpenGL 3.3 not supported\n");
        exit(1);
    }
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0,0.0,0.0,1.0);
    p = initShaders(); 
    setupBuffers(); 
    glutMainLoop();
 
    return(0); 
}