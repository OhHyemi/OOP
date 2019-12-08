#include "GameObject.h"
#include "Transform.h"

/* STATIC VARIABLES and FUNCTIONS*/
vector<GameObject*> GameObject::gameObjects;

GameObject* GameObject::Find(const string& path) {
	
	for (auto obj : gameObjects)
	{
		if (obj->name == path)
			return obj;
	}
	return nullptr;
}

vector<GameObject *> GameObject ::FindObjectsWithTag(const string & path)
{
	vector<GameObject*> objs;

	for (auto obj : gameObjects)
	{
		if (obj->getTag() == path)
			objs.push_back(obj);
	}

	return objs;
}

/* General variables and functions */

GameObject::GameObject(const string& name, GameObject* parent, const string& tag)
	: name(name), tag(tag), enabled(true), parent(parent),
	transform(new Transform(this) ) {
	components.clear();
	components.push_back(transform);
	
	if (!parent)
		return;
	parent->pushToChildrens(this);
}

GameObject::~GameObject() {}

void GameObject::traverseStart() {
	if (enabled == false) return;

	for (auto comp : components)
	{
		comp->start();
	}
}

void GameObject::traverseUpdate() {
	if (enabled == false) return;

	for (auto comp : components)
	{
		comp->update();
	}

}
