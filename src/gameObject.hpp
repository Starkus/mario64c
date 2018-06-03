#pragma once

#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "component.hpp"
#include "linkedList.hpp"

using namespace glm;

class GameObject {
protected:
	LinkedList<Component*> components;

public:
	vec3 position = vec3(0);
	quat rotation = quat(1, 0, 0, 0);
	vec3 scale = vec3(1.0f);

	void update();

	LinkedList<Component*> getComponents();

	template <typename T>
	void addComponent() {
		T *newComp = new T(this);
		this->components.add(newComp);
		//return &(*components.back());
	}

	template <typename T>
	T* getComponentOfType();
};