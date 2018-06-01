#include "gameObject.hpp"

void GameObject::update() {
	for (int i = 0; i < components.size(); ++i) {
		//printf("Updating Component");
		Component *comp = components.get(i);
		comp->update();
	}
}

LinkedList<Component*> GameObject::getComponents() {
	return components;
}

template <typename T>
T* GameObject::getComponentOfType() {
	for (int i = 0; i < components.size(); ++i) {
		Component component = components.get(i);
		if (dynamic_cast<T*>(&component) != nullptr) {
			return &component;
		}
	}

	return nullptr;
}
