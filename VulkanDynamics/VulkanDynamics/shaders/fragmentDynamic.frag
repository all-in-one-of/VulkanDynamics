#version 450
//#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 1) uniform UniformFragBufferObject {
	vec3 Ambient;
	vec3 LightColor;
    vec3 LightPosition; // location of the light, eye space
	float Shininess;
	float Strength;
	vec3 EyeDirection;
	float ConstantAttenuation; // attenuation coefficients
	float LinearAttenuation;
	float QuadraticAttenuation;
	mat3 viewMatrix ;
	mat3 eyeViewMatrix;
} ufbo;

layout (location = 0 ) in vec3 fragColor;
layout (location = 1 ) in vec3 Normal;
layout (location = 2 ) in vec4 Position;
layout (location = 3 ) in vec3 lightPos;
layout (location = 4 ) in vec3 NormalView;

layout (location = 0) out vec4 outColor;

void main() {

	//vec3 lightPosView = ufbo.eyeViewMatrix * ufbo.LightPosition;
	//vec3 lightDirectionView = ufbo.LightPosition - vec3(Position);
	vec3 lightDirectionView = lightPos - vec3(Position);
	float lightDistance = length(lightDirectionView);

	// normalize the light direction vector, so
	// that a dot products give cosines
	lightDirectionView = normalize ( lightDirectionView );

	vec3 reflection_eye = reflect (-lightDirectionView, Normal);
	vec3 surface_to_viewer_eye = normalize (-vec3(Position));

	// model how much light is available for this fragment, specular component
	float attenuation = 1.0 / (ufbo.ConstantAttenuation + ufbo.LinearAttenuation * lightDistance + ufbo.QuadraticAttenuation * lightDistance * lightDistance);

	// the direction of maximum highlight also changes per fragment
	//vec3 halfVector = normalize(lightDirectionView + ufbo.EyeDirection);

	float diffuse = max(0.0, dot(Normal, lightDirectionView));
	//float specular = max (dot (reflection_eye, surface_to_viewer_eye), 0.0);

	//if (diffuse == 0.0)
		//specular = 0.0;
	//else
		//specular = pow(specular, ufbo.Shininess) * ufbo.Strength;

	vec3 scatteredLight = ufbo.Ambient + (ufbo.LightColor * diffuse) ;
	//vec3 reflectedLight = ufbo.LightColor * specular * attenuation;
	
	//vec3 rgb = min(fragColor.rgb * scatteredLight + reflectedLight ,vec3(1.0));

	vec3 rgb = min(fragColor.rgb * scatteredLight ,vec3(1.0));

    outColor = vec4 ( scatteredLight, 1.0f );
	//outColor = vec4 ( ufbo.LightPosition  , 1.0f );
	//outColor = vec4 ( vec3 (0.0, 0.0 , 1.0  ) , 1.0f );
	
	
}