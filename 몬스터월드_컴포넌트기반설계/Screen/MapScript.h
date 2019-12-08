#pragma once
#include"Component.h"
#include"Utils.h"

class MapScript
	:public Component
{
private:
	Vector2 size;
	Vector2 startPosition;

	int numOfMonster;
	int numOfFood;

	vector<GameObject*> objs;
	
public:
	MapScript(GameObject* gameObject);
	~MapScript();

	void start();
	void update();

	Vector2 getSize() { return size; }
	Vector2 getStartPosition() { return startPosition; }
	
	
	int*getNumOfMonster() { return &numOfMonster; }
	void setNumOfMonster();
	int* getNumOfFood() { return &numOfFood; };
	void updateNumOfFood();

};

