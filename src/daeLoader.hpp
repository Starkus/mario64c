#pragma once

#include <stdlib.h>
#include <fstream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "common/vboindexer.hpp"
#include "rapidxml.hpp"
#include "stringUtils.hpp"

using namespace glm;
using namespace rapidxml;
using namespace std;

int readFile(const char *filename, string *contentstr, xml_document<> *out) {
	ifstream in(filename, ios::in | ios::binary);
	if (!in)
	{
		printf("Impossible to open COLLADA file '%s'\n", filename);
		return 1;
	}

	in.seekg(0, ios::end);
	contentstr->resize(in.tellg());
	in.seekg(0, ios::beg);
	in.read(&(*contentstr)[0], contentstr->size());
	in.close();

	// Create the xml file
	out->parse<0>((char*)contentstr->c_str());

	xml_node<> *root = out->first_node("COLLADA");

	return 0;
}

xml_node<>* getMesh(xml_document<> *doc) {
	// Get the big nodes
	xml_node<> *root = doc->first_node("COLLADA");
	xml_node<> *geometry = root->first_node("library_geometries")->
		first_node("geometry");
	// Just first mesh for now
	return geometry->first_node("mesh");
}

void loadUpRawData(xml_node<> *mesh, vector<vec3> *vertices,
			vector<vec3> *normals, vector<vec2> *uvs) {
	// Iterate through the source nodes, which have vertices, normals and uvs
	for (xml_node<> *n = mesh->first_node("source"); n;
		n = n->next_sibling("source")) {
		std::string source = n->first_attribute("id")->value();

		// Get the float array
		const char* s = n->first_node("float_array")->value();

		// Check where we're at outside of the loop below
		bool atVerts = source.find("positions") != string::npos;
		bool atNorms = source.find("normals") != string::npos;
		bool atUVs = source.find("map") != string::npos;

		// Array is in a string so split it into "words"
		vector<string> words = splitString(s, ' ');

		float x, y;

		int vecIndex = 0;
		for (vector<string>::iterator w = words.begin(); w != words.end(); ++w) {
			printf("%s\n", (*w).c_str());

			float cur = stof(*w);

			switch (vecIndex) {
			case 0:
				x = cur;
				break;

			case 1:
				y = cur;

				if (atUVs) {
					uvs->push_back(vec2(x, y));
					vecIndex = 0;
					continue;
				}
				break;

			case 2:
				if (atVerts)
					vertices->push_back(vec3(x, y, cur));
				else if (atNorms)
					normals->push_back(vec3(x, y, cur));

				break;
			}

			// Increment vector index and loop back to 0 at 3
			vecIndex = (vecIndex + 1) % 3;
		}
	}
}

void generateNeededBundles(xml_node<> *mesh, vector<vec3> *verts_in, vector<vec3> *norms_in, vector<vec2> *uvs_in, 
			vector<vec3> *verts_out, vector<vec3> *norms_out, vector<vec2> *uvs_out) {
	// Generate bundles
	for (xml_node<> *n = mesh->first_node("triangles"); n;
		n = n->next_sibling("triangles")) {
		const char* s = n->first_node("p")->value();

		vector<string> words = splitString(s, ' ');
		vec3 vert;
		vec2 uv;
		vec3 norm;

		int i = 0;
		for (vector<string>::iterator w = words.begin(); w != words.end(); ++w) {
			printf("%s\n", (*w).c_str());

			int vecIndex = stoi(*w);

			switch (i) {
			case 0:
				vert = (*verts_in)[vecIndex];
				break;
			case 1:
				norm = (*norms_in)[vecIndex];
				break;
			case 2:
				uv = (*uvs_in)[vecIndex];

				(*verts_out).push_back(vert);
				(*norms_out).push_back(norm);
				(*uvs_out).push_back(uv);
				break;
			default:
				break;
			}

			i = (i + 1) % 3;
		}
	}
}


int loadDae(const char *filename, vector<vec3> *vertices, vector<vec3> *normals,
	vector<vec2> *uvs, vector<unsigned short> *indices) {

	xml_document<> doc;
	// Need to take care of this memory manually
	string *contentstr = new string;

	int errorcode = readFile(filename, contentstr, &doc);
	if (!errorcode == 0)
		return errorcode;

	xml_node<> *mesh = getMesh(&doc);

	vector<vec3> t_vertices;
	vector<vec2> t_uvs;
	vector<vec3> t_normals;

	loadUpRawData(mesh, vertices, normals, uvs);

	generateNeededBundles(mesh, vertices, normals, uvs, &t_vertices, &t_normals, &t_uvs);

	// These have the raw stuffs instead of all copies in order,
	// so we wipe them clean
	vertices->clear();
	normals->clear();
	uvs->clear();

	// Now index in OpenGLs way
	indexVBO(t_vertices, t_uvs, t_normals, *indices, *vertices, *uvs, *normals);

	// Free the file content string
	delete contentstr;

	return 0;
}