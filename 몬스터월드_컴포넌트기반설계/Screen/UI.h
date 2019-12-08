#pragma once
#include"Utils.h"

class UI
{
protected:
	Vector2 position;

public:
	UI(Vector2 position);
	~UI();
	virtual void draw() {}
	Vector2 getPosition() { return position; }
};

