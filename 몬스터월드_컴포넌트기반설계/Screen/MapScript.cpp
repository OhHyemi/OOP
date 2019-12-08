#include "MapScript.h"
#include "GameObject.h"


MapScript::MapScript(GameObject* gameObject)
	:size(Vector2(20,10)),
	startPosition(Vector2(0,0)),
	Component(gameObject),
	numOfMonster(0),
	numOfFood(0)
{
}


MapScript::~MapScript()
{
}

void MapScript::start()
{
	objs = gameObject->getChildrens();
	setNumOfMonster();
}

void MapScript::update()
{
	updateNumOfFood();
}


void MapScript::setNumOfMonster()
{
	int monster = 0;
	for (auto obj : objs)
	{
		if (obj->isActive() && obj->getTag() == "Monster")
			monster++;
	}
	numOfMonster = monster;
}

void MapScript::updateNumOfFood()
{	
	int food = 0;
	for (auto obj : objs)
	{
		if (obj->isActive() && obj->getTag() == "Food")
			food++;
	}
	numOfFood = food;
}
