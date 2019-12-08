#pragma once
#include "Component.h"
#include <string>

using namespace std;

class MonsterScript 
	: public Component
{
	string monsterShape;
	int NumOfEatingFood;

public:
	MonsterScript(GameObject* gameObject, string shape = "");
	~MonsterScript();
	void start();

	void update();

	void moveRandomly();

	string getShape() { return monsterShape; }
	void setShape(string shape) { monsterShape = shape; }
	bool collsion();
	bool isInMap();
	void eatFood(GameObject* food);
	int* getNumOfEatingFood() { return &NumOfEatingFood; }

	void draw();
	
};

