#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Component;
class Transform;

class GameObject
{
	bool		enabled;
	string		name;
	string		tag;
	GameObject* parent;
	vector<Component *> components;
	vector<GameObject *> children;

	static vector<GameObject *> gameObjects;
	friend class GameEngine;

	Transform* transform;

public:
	GameObject(const string& name, GameObject* parent = nullptr, const string& tag = "");

	~GameObject();

	void traverseStart();
	void traverseUpdate();

	string getName() const { return name; }

	Transform* getTransform() { return transform; }

	static GameObject* Find(const string& path);

	static vector<GameObject *> FindObjectsWithTag(const string& path);

	string getTag() { return tag; }
	void setTag(string tag) { this->tag = tag; }

	void setParent(GameObject* parent) {this->parent = parent;}
	GameObject* getParent() { return parent; }

	vector<GameObject *>getChildrens() { return children; }
	void pushToChildrens(GameObject* child) { children.push_back(child); }

	virtual bool isActive() 
	{ return enabled; }

	void setActive(bool flag = true) 
	{ enabled = flag; }

	template<typename T>
	void addComponent() {		
		T t{ this };
		if (dynamic_cast<Component*>(&t) == nullptr) {
			return;
		}
		components.push_back(new T{ this });
	}

	template<typename T>
	Component* getComponent() {
		for (auto comp : components)
		{
			if (dynamic_cast<T *>(comp) != nullptr) return comp;
		}
		return nullptr;
	}
	void destroy(GameObject** gameObject) { gameObject = nullptr; delete gameObject; }

};

