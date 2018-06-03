#pragma once

#include <stdlib.h>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "material.hpp"

using namespace glm;

class Model {
private:
	// Model data
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals;
	std::vector<unsigned short> indices;

	std::vector<Material> materials;
	// What indices belong to what material index
	std::vector<int> offsets;

	// GL Buffers
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint indexbuffer;

	void makeVBOs();
	void bindBuffers();
	void drawBuffers();
	void makeModelMatrix(vec3 position, quat rotation, vec3 scale);

public:
	Model();
	~Model();
	void draw(vec3 position, quat rotation, vec3 scale);

	int importFromObj(const char*);
	int importFromDae(const char*);
};