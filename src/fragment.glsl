#version 330 core

// Ouput data
out vec3 color;

in vec2 UV;
in vec4 matDiffuse;
in vec4 matSpeculat;
in vec4 matAmbient;
in vec4 matEmmision;

in vec3 Normal_cameraspace;
in vec3 LightDirection_cameraspace;

uniform sampler2D myTextureSampler;

void main() {
	vec3 n = normalize(Normal_cameraspace);
	vec3 l = normalize(LightDirection_cameraspace);

	float cosTheta = dot(n, l);
	float lum = max(0, cosTheta) + 0.3; // Smoother!

	color = texture(myTextureSampler, UV).rgb * matDiffuse.rgb * lum;
	color = matDiffuse.rgb * lum + matAmbient.rgb + matEmmision.rgb;
}