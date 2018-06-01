#pragma once

#include "component.hpp"
#include "model.hpp"

class ComponentMeshRenderer : public Component {
protected:
	Model *model;

public:
	ComponentMeshRenderer(GameObject*);

	void setModel(Model*);

	//void start() override;
	void update() override;
};