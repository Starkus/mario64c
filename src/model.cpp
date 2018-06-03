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

void Model::sendMaterial(Material *mat) {
	GLuint diffuseId = GLContext::instance()->getProgramUniform("diffuse");
	GLuint specularId = GLContext::instance()->getProgramUniform("specular");
	GLuint ambientId = GLContext::instance()->getProgramUniform("ambient");
	GLuint emmisionId= GLContext::instance()->getProgramUniform("emmision");

	vec4 c = mat->diffuse;
	glUniform4f(diffuseId, c.r, c.g, c.b, c.a);
	c = mat->specular;
	glUniform4f(specularId, c.r, c.g, c.b, c.a);
	c = mat->ambient;
	glUniform4f(ambientId, c.r, c.g, c.b, c.a);
	c = mat->emission;
	glUniform4f(emmisionId, c.r, c.g, c.b, c.a);
}

void Model::drawBuffers() {
	for (int mat = 0; mat < materials.size(); ++mat) {
		sendMaterial(&materials[mat]);

		int start;
		int count;

		if (mat == 0) {
			count = offsets[mat];
			start = 0;
		}
		else if (mat == materials.size() - 1) { // If last one
			count = indices.size() - offsets[mat - 1];
			start = offsets[mat - 1];
		}
		else {
			count = offsets[mat] - offsets[mat - 1];
			start = offsets[mat - 1];
		}

		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, ((char *)NULL + (start * sizeof(unsigned short))));
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}


Model::Model() {
	// nothing for now
}

void Model::draw() {
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
