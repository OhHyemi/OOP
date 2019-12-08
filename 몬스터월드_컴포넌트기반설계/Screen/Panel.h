#pragma once
#include "UI.h"

class Panel :
	public UI
{
	Vector2 size;

public:
	Panel(Vector2 position, Vector2 size = Vector2(1,1));
	~Panel();

	void setSize(Vector2 size) { this->size = size; }
	Vector2 getSize() { return size; }

	void draw();

};

