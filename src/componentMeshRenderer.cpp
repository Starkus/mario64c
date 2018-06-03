#include "componentMeshRenderer.hpp"
//#include "gameObject.hpp"

// By design, theres probably no way to actually call this constructor
/*ComponentMeshRenderer::ComponentMeshRenderer(Model *model) {
	this->model = model;
}*/
ComponentMeshRenderer::ComponentMeshRenderer(GameObject *owner) : Component(owner) {
}

void ComponentMeshRenderer::update() {
	model->draw(obj->position, obj->rotation, obj->scale);
}

void ComponentMeshRenderer::setModel(Model *model) {
	this->model = model;
}