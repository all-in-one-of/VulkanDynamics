// VulkanDynamics.cpp
//
#include "VulkanDynamics.h"
#include "Scene.h"
#include "InputHandler.h"

typedef struct {
	float fieldOfView;
	float aspect;
	float nearPlane;
	float farPlane;
}perspectiveData;

perspectiveData pD;

// storage for matrices
float projMatrix[16];
glm::mat4 viewMatrix;
glm::mat4 eyeviewMatrix;
glm::vec3 ambientLight(0.1f, 0.1f, 0.1f);
glm::vec3 lightColor ( 1.0f, 0.0f, 0.0f );
glm::vec3 LightPosition ( 0.0f, 3.0f, -3.0f );
float Shininess = 140.0f;
float Strength = 60.0f;
glm::vec3 EyeDirection ( 0.0f,1.0f, -3.0f );
float ConstantAttenuation = 50.0f;
float LinearAttenuation = 0.0f;
float QuadraticAttenuation = 0.0f;
glm::mat3 normalModelViewMatrix;

std::stack<float *> first;
float tempMatrix[16];

glm::vec3 mainEyeLoc(4.0, 4.0, 4.0);
glm::vec3 centerLoc(0.0, 0.0, 0.0);
glm::vec3 up(0.0, 0.0, 1.0);
float fov = glm::radians<float>(45.0f);

void setTop(){
	normalModelViewMatrix[0][0] = eyeviewMatrix[0][0];
	normalModelViewMatrix[0][1] = eyeviewMatrix[0][1];
	normalModelViewMatrix[0][2] = eyeviewMatrix[0][2];
	normalModelViewMatrix[1][0] = eyeviewMatrix[1][0];
	normalModelViewMatrix[1][1] = eyeviewMatrix[1][1];
	normalModelViewMatrix[1][2] = eyeviewMatrix[1][2];
	normalModelViewMatrix[2][0] = eyeviewMatrix[2][0];
	normalModelViewMatrix[2][1] = eyeviewMatrix[2][1];
	normalModelViewMatrix[2][2] = eyeviewMatrix[2][2];
}

void set_eyeView(float eyex, float eyey, float eyez,
	float centx, float centy, float centz,
	float upx, float upy, float upz) {

	float fwd[3], side[3], up[3];
	
	fwd[0] = centx - eyex;
	fwd[1] = centy - eyey;
	fwd[2] = centz - eyez;

	float fwd_length = sqrt((fwd[0] * fwd[0]) + (fwd[1] * fwd[1]) + (fwd[2] * fwd[2]));
	fwd[0] = fwd[0] / fwd_length;
	fwd[1] = fwd[1] / fwd_length;
	fwd[2] = fwd[2] / fwd_length;

	side[0] = fwd[1] * upz - fwd[2] * upy;
	side[1] = fwd[2] * upx - fwd[0] * upz;
	side[2] = fwd[0] * upy - fwd[1] * upx;

	float side_length = sqrt((side[0] * side[0]) + (side[1] * side[1]) + (side[2] * side[2]));
	side[0] = side[0] / side_length;
	side[1] = side[1] / side_length;
	side[2] = side[2] / side_length;

	up[0] = side[1] * fwd[2] - side[2] * fwd[1];
	up[1] = side[2] * fwd[0] - side[0] * fwd[2];
	up[2] = side[0] * fwd[1] - side[1] * fwd[0];

	eyeviewMatrix[0][0] = side[0];
	eyeviewMatrix[0][1] = side[1];
	eyeviewMatrix[0][2] = side[2];
	eyeviewMatrix[0][3] = 0.0;

	eyeviewMatrix[1][0] = up[0];
	eyeviewMatrix[1][1] = up[1];
	eyeviewMatrix[1][2] = up[2];
	eyeviewMatrix[1][3] = 0.0;

	eyeviewMatrix[2][0] = -fwd[0];
	eyeviewMatrix[2][1] = -fwd[1];
	eyeviewMatrix[2][2] = -fwd[2];
	eyeviewMatrix[2][3] = 0.0;

	eyeviewMatrix[3][0] = eyeviewMatrix[3][1] = eyeviewMatrix[3][2] = 0.0;
	eyeviewMatrix[3][3] = 1.0;

	glm::mat4 aux;
	aux[0][0] = aux[1][1] = aux[2][2] = aux[3][3] = 1.0f;
	aux[0][1] = aux[0][2] = aux[1][0] = aux[1][2] = aux[2][0] = aux[2][1] = aux[3][0] = aux[3][1] = aux[3][2] = 0.0f;
	aux[0][3] = -eyex;
	aux[1][3] = -eyey;
	aux[2][3] = -eyez;

	eyeviewMatrix = eyeviewMatrix * aux;
}

//need to load in the value of the uniform buffers for the frag and vert shader + load attributes
void loadInitialVariables(MainVulkApplication & _app) {
	_app.ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	_app.ubo.view = glm::lookAt(mainEyeLoc, centerLoc, up);
	_app.ubo.proj = glm::perspective(fov, _app.swapChainExtent.width / (float)_app.swapChainExtent.height, 0.1f, 9.0f);
	_app.ubo.proj[1][1] *= -1;
	glm::mat3 viewMatrix3x3(_app.ubo.view);
	_app.ubo.normalMatrix = glm::inverseTranspose(viewMatrix3x3);

	_app.ufo.Ambient = ambientLight;
	_app.ufo.LightColor = lightColor;
	_app.ufo.LightPosition = LightPosition;
	_app.ufo.Reflectivity = Shininess;
	_app.ufo.Strength = Strength;
	_app.ufo.EyeDirection = EyeDirection;
	_app.ufo.ConstantAttenuation = ConstantAttenuation;
	_app.ufo.LinearAttenuation = LinearAttenuation;
	_app.ufo.QuadraticAttenuation = QuadraticAttenuation;
	_app.ufo.viewMatrix = viewMatrix;
	_app.ufo.eyeViewMatrix = eyeviewMatrix;
}

void updateUniformBuffer(MainVulkApplication & _app) {
	_app.ubo.view = glm::lookAt(mainEyeLoc, centerLoc, up);
	_app.ubo.proj = glm::perspective(fov, _app.swapChainExtent.width / (float)_app.swapChainExtent.height, 0.1f, 10.0f);
	_app.ubo.proj[1][1] *= -1;
	//_app.ubo.model = glm::rotate(glm::mat4(1.0f),  glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	_app.ubo.model = glm::mat4(1.0f);
}

/*
void loadModel(MainVulkApplication & _app) {

	int size = sizeof(vertices1) / sizeof(float);

	for ( int i = 0 ; i < size ; i += 3 ){
		VertexNew tempNew;

		tempNew.pos = glm::vec3(vertices1[i], vertices1[i+1], vertices1[i+2]);
		tempNew.color = glm::vec4(colors1[i], colors1[i + 1], colors1[i + 2], colors1[i + 3]);
		tempNew.vertexNormal = glm::vec3( normals1[i], normals1[i + 1], normals1[i + 2]);
		tempNew.lightPos = LightPosition;

		_app.vertices.push_back(tempNew);
		_app.indices.push_back(_app.vertices.size());
	}
}
*/

void mainLoop(MainVulkApplication & _app) {

	glfwSetKeyCallback(_app.window, readInput_callback);
	glfwSetCursorPosCallback(_app.window, mouse_cursor_callback);
	glfwSetMouseButtonCallback(_app.window, mouse_button_callback);

	while (!glfwWindowShouldClose(_app.window)) {
		glfwPollEvents();
		updateUniformBuffer(_app);
		_app.drawFrame();
	}

	vkDeviceWaitIdle(_app.device);
 }

int main() {
	MainVulkApplication app;

	try {
		set_eyeView(5.0f, 5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f);
		//loadModel(app);
		app.setup();
		loadInitialVariables(app);
		mainLoop(app);
		app.cleanupApp();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
