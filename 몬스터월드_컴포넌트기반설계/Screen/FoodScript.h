#pragma once
#include "Component.h"
#include "Utils.h"

class FoodScript
	:public Component
{
	string foodShape;
public:
	FoodScript(GameObject* gameObject);
	~FoodScript();

	void update();
	void draw();
	
};

