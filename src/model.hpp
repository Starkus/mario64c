#pragma once

#include <stdlib.h>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace glm;

class Model {
private:
	// Model data
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals;

	// GL Buffers
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;

	void makeVBOs();
	void bindBuffers();
	void drawBuffers();
	void makeModelMatrix(vec3 position, vec3 scale);

public:
	Model();
	void draw(vec3 position, vec3 scale);

	int importFromObj(const char*);
};