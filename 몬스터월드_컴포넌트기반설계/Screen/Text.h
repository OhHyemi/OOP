#pragma once
#include "UI.h"

class Text :
	public UI
{
protected:
	string contents;
public:
	Text(Vector2 position, string contents = "");
	~Text();

	void draw();
};

