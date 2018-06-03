#include "componentMeshRenderer.hpp"
#include "glcontext.hpp"
#include "common/controls.hpp"
#include <glm/gtc/matrix_transform.hpp>
//#include "gameObject.hpp"

// By design, theres probably no way to actually call this constructor
/*ComponentMeshRenderer::ComponentMeshRenderer(Model *model) {
	this->model = model;
}*/
ComponentMeshRenderer::ComponentMeshRenderer(GameObject *owner) : Component(owner) {
}

void ComponentMeshRenderer::update() {
	makeModelMatrix(obj->position, obj->rotation, obj->scale);
	model->draw();
}

void ComponentMeshRenderer::makeModelMatrix(vec3 position, quat rotation, vec3 scale) {
	mat4 rot = mat4_cast(rotation);

	mat4 model = mat4(1.0f);
	model = glm::scale(model, scale);
	model = glm::translate(model, position);
	model *= rot;

	// Calculate MVP
	mat4 mvp = getProjectionMatrix() * getViewMatrix() * model;

	// Send new matrix to the program (the shaders)
	GLuint modelMatrixId = GLContext::instance()->getProgramUniform("M");
	GLuint mvpMatrixId = GLContext::instance()->getProgramUniform("MVP");
	glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(mvpMatrixId, 1, GL_FALSE, &mvp[0][0]);
}

void ComponentMeshRenderer::setModel(Model *model) {
	this->model = model;
}