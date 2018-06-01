#pragma once

#include <memory>

class GameObject;

class Component {
protected:
	//std::unique_ptr<GameObject> obj;
	GameObject *obj;

public:
	Component(GameObject *owner);

	virtual void start() {}
	virtual void update() {}
};

#include "gameObject.hpp"