#include "FoodScript.h"
#include "GameObject.h"
#include "Transform.h"
#include "MonsterScript.h"
#include "MapScript.h"
#include <vector>

FoodScript::FoodScript(GameObject* gameObject)
	:foodShape("\xF9"),Component(gameObject)
{
}


FoodScript::~FoodScript()
{
}


void FoodScript::update()
{
	draw();
}

void FoodScript::draw()
{
	if (!(gameObject->isActive()))
		return;
	Screen::getInstance().draw(foodShape.c_str(), transform->getScale().x, transform->getScale().y, transform->getPosition());
}



