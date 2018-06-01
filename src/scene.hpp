#pragma once

#include <stdlib.h>

#include "gameObject.hpp"
#include "linkedList.hpp"

class Scene {
public:
	LinkedList<GameObject*> gameObjects;

	Scene();
	void update();

	GameObject* spawn();
};