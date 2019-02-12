#version 450
//#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
	mat3 normalMatrix ;
} ubo;

layout (location = 0 ) in vec3 color;
layout (location = 1 ) in vec3 VertexNormal;
layout (location = 2 ) in vec3 position;
layout (location = 3 ) in vec3 lightPos;

layout (location = 0 ) out vec3 fragColor;
layout (location = 1 ) out vec3 Normal;
layout (location = 2 ) out vec4 Position; // adding position, so we know where we are
layout (location = 3 ) out vec3 LightPos;
layout (location = 4 ) out vec3 NormalView;

void main() {

	//Position = ubo.view * vec4(position, 1.0f) ;
	Position = vec4(position, 1.0f) ;
	Normal = normalize( ubo.normalMatrix * VertexNormal);
	LightPos = lightPos;
    fragColor = color;
	gl_Position = ubo.proj * ubo.view * ubo.model * vec4(position, 1.0f);
}