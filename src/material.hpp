#pragma once

#include <glm/glm.hpp>

using namespace glm;

class Material {
public:
	vec4 diffuse;
	vec4 specular;
	vec4 ambient;
	vec4 emission;
};