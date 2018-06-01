#version 330 core

// Ouput data
out vec3 color;

in vec2 UV;
in vec3 Normal_cameraspace;
in vec3 LightDirection_cameraspace;

uniform sampler2D myTextureSampler;

void main() {
	vec3 n = normalize(Normal_cameraspace);
	vec3 l = normalize(LightDirection_cameraspace);

	float cosTheta = dot(n, l);
	float lum = max(0, cosTheta) + 0.3; // Smoother!

	// Output color = red 
	color = texture(myTextureSampler, UV).rgb * lum;
}