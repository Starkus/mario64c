#include "model.hpp"
#include "common/objloader.hpp"
#include "glcontext.hpp"
#include "common/controls.hpp"

#include <glm/gtc/matrix_transform.hpp>

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
}

void Model::drawBuffers() {
	// Draw bound buffers, to bind call bindBuffers() first.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glDisableVertexAttribArray(0);
}

void Model::makeModelMatrix(vec3 position, vec3 scale) {
	computeMatricesFromInputs();
	mat4 projection = getProjectionMatrix();
	mat4 view = getViewMatrix();
	mat4 model = mat4(1.0f);
	model = glm::scale(model, scale);
	model = glm::translate(model, position);

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

void Model::draw(vec3 position, vec3 scale) {
	makeModelMatrix(position, scale);
	bindBuffers();
	drawBuffers();
}

int Model::importFromObj(const char *filename) {
	// Import model
	bool ok = loadOBJ(filename, vertices, uvs, normals);

	if (ok)
		makeVBOs();

	return ok;
}