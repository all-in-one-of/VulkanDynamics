#version 450
 
layout(binding = 1) uniform UniformFragBufferObject {
	vec3 Ambient;
	vec3 LightColor;
    vec3 LightPosition; // location of the light, eye space
	float Shininess;
	float Strength;
	vec3 EyeDirection;
	float ConstantAttenuation; // attenuation coefficients
	float LinearAttenuation;
	float QuadraticAttenuation;
	mat4 viewMatrix ;
	mat4 eyeViewMatrix;
} ufbo;

layout (location = 0 ) in vec4 Color;
layout (location = 1 ) in vec3 Normal;
layout (location = 2 ) in vec4 Position;
layout (location = 3 ) in vec3 lightPos;
layout (location = 4 ) in vec3 NormalView;

layout (location = 0 ) out vec4 outputF;

void main()
{	
	vec3 lightPosView = vec3 (ufbo.eyeViewMatrix * vec4 (ufbo.LightPosition, 1.0));
	vec3 lightDirectionView = lightPosView - vec3(Position);
	float lightDistance = length(lightDirectionView);

	// normalize the light direction vector, so
	// that a dot products give cosines
	lightDirectionView = lightDirectionView / lightDistance;

	vec3 reflection_eye = reflect (-lightDirectionView, Normal);
	vec3 surface_to_viewer_eye = normalize (-vec3(Position));

	// model how much light is available for this fragment
	float attenuation = 1.0 / (ufbo.ConstantAttenuation + ufbo.LinearAttenuation * lightDistance + ufbo.QuadraticAttenuation * lightDistance * lightDistance);

	//attenuation = 0;

	// the direction of maximum highlight also changes per fragment
	vec3 halfVector = normalize(lightDirectionView + ufbo.EyeDirection);

	float diffuse = max(0.0, dot(Normal, lightDirectionView));
	float specular = max (dot (reflection_eye, surface_to_viewer_eye), 0.0);

	if (diffuse == 0.0)
		specular = 0.0;
	else
		specular = pow(specular, ufbo.Shininess) * ufbo.Strength;

	vec3 scatteredLight = ufbo.Ambient + ufbo.LightColor * diffuse ;
	vec3 reflectedLight = ufbo.LightColor * specular * attenuation;
	vec3 rgb = min(Color.rgb * scatteredLight + reflectedLight ,vec3(1.0));
	outputF = vec4(rgb, Color.a);
}