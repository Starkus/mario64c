#include "scene.hpp"
#include "componentMeshRenderer.hpp"

Scene::Scene() {
}

void Scene::update() {
	for (int i = 0; i < gameObjects.size(); ++i) {
		//printf("Updating an object");
		GameObject *obj = gameObjects.get(i);
		
		obj->update();
	}
}

GameObject* Scene::spawn() {
	printf("Spawned an object\n");
	GameObject *newObj = new GameObject;
	//GameObject newObj;
	gameObjects.add(newObj);
	return newObj;
}