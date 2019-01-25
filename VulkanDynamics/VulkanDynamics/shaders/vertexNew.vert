#version 450

//uniform mat4 viewMatrix, projMatrix, eyeViewMatrix;
//uniform mat3 NormalMatrix;
//uniform mat3 NormalViewMatrix;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
	mat3 normalMatrix ;
} ubo;

layout (location = 0 ) in vec4 color;
layout (location = 1 ) in vec3 VertexNormal;
layout (location = 2 ) in vec4 position;
layout (location = 3 ) in vec3 lightPos;

layout (location = 0 ) out vec4 Color;
layout (location = 1 ) out vec3 Normal;
layout (location = 2 ) out vec4 Position; // adding position, so we know where we are
layout (location = 3 ) out vec3 LightPos;
layout (location = 4 ) out vec3 NormalView;

void main()
{
	Color = color;
	//Normal = normalize( transpose(inverse(NormalMatrix)) * VertexNormal);
	//Normal = normalize( NormalMatrix * VertexNormal);
	Normal = normalize( ubo.normalMatrix * VertexNormal);
	Position = ubo.view * position; // pre-perspective space
	gl_Position = ubo.proj * ubo.view * position;
}