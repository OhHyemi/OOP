#pragma once
#include "Text.h"
class Score :
	public Text
{
private:
	int* score;
public:
	Score(Vector2 position, int* score = nullptr, string contents = "socre: ");
	~Score();

	void setScore(int *score) { this->score = score; }
	void draw();
	
};

