#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelspace;

out vec2 UV;
out vec3 Normal_cameraspace;
out vec3 LightDirection_cameraspace;
out vec4 matDiffuse;
out vec4 matSpeculat;
out vec4 matAmbient;
out vec4 matEmmision;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 MVP;

uniform vec4 diffuse;
uniform vec4 specular;
uniform vec4 ambient;
uniform vec4 emmision;

void main(){
	//MVP = P*V*M;

	UV = vertexUV;
	matDiffuse = diffuse;
	matSpeculat = specular;
	matAmbient = ambient;
	matEmmision = emmision;
	vec3 pos = vertexPosition_modelspace;

	gl_Position = MVP * vec4(pos, 1);

	vec3 vertexPosition_cameraspace = (V * M * vec4(pos, 1)).xyz;
	vec3 EyeDirection_cameraspace = vec3(0, 0, 0) - vertexPosition_cameraspace;

	vec3 LightPosition_worldspace = vec3(2, 0, 2);

	vec3 LightPosition_cameraspace = (V * vec4(LightPosition_worldspace, 1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

	Normal_cameraspace = (V * M * vec4(vertexNormal_modelspace, 0)).xyz;
}

