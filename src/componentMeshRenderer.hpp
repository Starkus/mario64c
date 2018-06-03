#pragma once

#include "component.hpp"
#include "model.hpp"

class ComponentMeshRenderer : public Component {
protected:
	Model *model;

	void makeModelMatrix(vec3 position, quat rotation, vec3 scale);

public:
	ComponentMeshRenderer(GameObject*);

	void setModel(Model*);

	//void start() override;
	void update() override;
};