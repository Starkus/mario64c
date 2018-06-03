#include "model.hpp"
#include "common/objloader.hpp"
#include "glcontext.hpp"
#include "common/controls.hpp"
#include "common/vboindexer.hpp"
#include "stringUtils.hpp"

#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include "rapidxml.hpp"

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
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
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
	std::vector<vec3> t_vertices;
	std::vector<vec2> t_uvs;
	std::vector<vec3> t_normals;

	bool ok = loadOBJ(filename, t_vertices, t_uvs, t_normals);

	if (ok) {
		indexVBO(t_vertices, t_uvs, t_normals, indices, vertices, uvs, normals);

		makeVBOs();
	}

	return ok;
}

int Model::importFromDae(const char *filename) {
	using namespace rapidxml;

	// Temp vectors
	std::vector<vec3> t_vertices;
	std::vector<vec2> t_uvs;
	std::vector<vec3> t_normals;

	// Read file into string
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (!in)
	{
		printf("Impossible to open COLLADA file '%s'\n", filename);
		return -1;
	}

	std::string contents;
	in.seekg(0, std::ios::end);
	contents.resize(in.tellg());
	in.seekg(0, std::ios::beg);
	in.read(&contents[0], contents.size());
	in.close();

	// Parse the xml file
	xml_document<> doc;
	doc.parse<0>((char*) contents.c_str());

	// Get the big nodes
	xml_node<> *root = doc.first_node("COLLADA");
	xml_node<> *geometry = root->first_node("library_geometries")->
			first_node("geometry");
	// Just first mesh for now
	xml_node<> *mesh = geometry->first_node("mesh");

	// Iterate through the source nodes, which have vertices, normals and uvs
	for (xml_node<> *n = mesh->first_node("source"); n;
			n = n->next_sibling("source")) {
		std::string source = n->first_attribute("id")->value();

		// Get the float array
		const char* s = n->first_node("float_array")->value();

		// Check where we are outside of the loop below
		bool atVerts = source.find("positions") != std::string::npos;
		bool atNorms = source.find("normals") != std::string::npos;
		bool atUVs = source.find("map") != std::string::npos;

		// Array is in a string so split it into "words"
		std::vector<std::string> words = splitString(s, ' ');
		
		float x, y;

		int vecIndex = 0;
		for (std::vector<std::string>::iterator w = words.begin(); w != words.end(); ++w) {
			printf("%s\n", (*w).c_str());

			float cur = std::stof(*w);

			switch (vecIndex) {
			case 0:
				x = cur;
				break;

			case 1:
				y = cur;

				if (atUVs) {
					uvs.push_back(vec2(x, y));
					vecIndex = 0;
					continue;
				}
				break;

			case 2:
				if (atVerts)
					vertices.push_back(vec3(x, y, cur));
				else if (atNorms)
					normals.push_back(vec3(x, y, cur));

				break;
			}

			// Increment vector index and loop back to 0 at 3
			vecIndex = (vecIndex+1) % 3;
		}
	}

	// Now build up the vertex group
	// For each group we need to copy the corresponding vert, normal and uv
	// into the temp vectors
	for (xml_node<> *n = mesh->first_node("triangles"); n;
				n = n->next_sibling("source")) {
		const char* s = n->first_node("p")->value();

		std::vector<std::string> words = splitString(s, ' ');
		vec3 vert;
		vec2 uv;
		vec3 norm;

		int i = 0;
		for (std::vector<std::string>::iterator w = words.begin(); w != words.end(); ++w) {
			printf("%s\n", (*w).c_str());

			int vecIndex = std::stoi(*w);

			switch (i) {
			case 0:
				vert = vertices[vecIndex];
				break;
			case 1:
				norm = normals[vecIndex];
				break;
			case 2:
				uv = uvs[vecIndex];

				t_vertices.push_back(vert);
				t_normals.push_back(norm);
				t_uvs.push_back(uv);
				break;
			default:
				break;
			}

			i = (i + 1) % 3;
		}
	}

	// These have the raw stuffs instead of all copies in order,
	// so we wipe them clean
	vertices.clear();
	normals.clear();
	uvs.clear();

	// Now index in OpenGLs way
	indexVBO(t_vertices, t_uvs, t_normals, indices, vertices, uvs, normals);
	makeVBOs();

	return 0;
}
