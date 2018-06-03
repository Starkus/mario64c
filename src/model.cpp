#include "model.hpp"
#include "common/objloader.hpp"
#include "glcontext.hpp"
#include "common/controls.hpp"
#include "common/vboindexer.hpp"
#include "stringUtils.hpp"
#include "daeLoader.hpp"

#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "rapidxml.hpp"

Model::~Model() {
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &indexbuffer);
}

void Model::makeVBOs() {
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);

	printf("Indices: %d\n", (int) indices.size());
	glGenBuffers(1, &indexbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
}

void Model::bindBuffers() {
	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Buffer the vertices
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
}

void Model::drawBuffers() {
	// Draw bound buffers, to bind call bindBuffers() first.
	int count = offsets[0];
	int start = 0;
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, ((char *)NULL + (start * sizeof(unsigned short))));

	count = offsets[1] - offsets[0];
	start = offsets[0];
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, ((char *)NULL + (start * sizeof(unsigned short))));

	count = indices.size() - offsets[1];
	start = offsets[1];
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, ((char *)NULL + (start * sizeof(unsigned short))));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Model::makeModelMatrix(vec3 position, quat rotation, vec3 scale) {
	computeMatricesFromInputs();
	mat4 projection = getProjectionMatrix();
	mat4 view = getViewMatrix();

	mat4 rot = mat4_cast(rotation);

	mat4 model = mat4(1.0f);
	model = glm::scale(model, scale);
	model = glm::translate(model, position);
	model *= rot;

	mat4 mvp = projection * view * model;

	// Send new matrix to the program (the shaders)
	GLuint modelMatrixId = GLContext::instance()->getProgramUniform("M");
	GLuint viewMatrixId = GLContext::instance()->getProgramUniform("V");
	GLuint projMatrixId = GLContext::instance()->getProgramUniform("P");
	GLuint mvpMatrixId = GLContext::instance()->getProgramUniform("MVP");
	glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(viewMatrixId, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projMatrixId, 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(mvpMatrixId, 1, GL_FALSE, &mvp[0][0]);
}


Model::Model() {
	// nothing for now
}

void Model::draw(vec3 position, quat rotation, vec3 scale) {
	makeModelMatrix(position, rotation, scale);
	bindBuffers();
	drawBuffers();
}

int Model::importFromObj(const char *filename) {
	// Import model
	std::vector<vec3> t_vertices;
	std::vector<vec2> t_uvs;
	std::vector<vec3> t_normals;

	// asdasfg
	std::vector<int> offsets;
	offsets.push_back(0);

	bool ok = loadOBJ(filename, t_vertices, t_uvs, t_normals);

	if (ok) {
		indexVBO(t_vertices, t_uvs, t_normals, offsets, indices, vertices, uvs, normals, offsets);

		makeVBOs();
	}

	return ok;
}

int Model::importFromDae(const char *filename) {
	int errorcode = loadDae(filename, &vertices, &normals, &uvs, &indices, &materials, &offsets);

	if (errorcode != 0)
		return errorcode;

	makeVBOs();

	return 0;
}
