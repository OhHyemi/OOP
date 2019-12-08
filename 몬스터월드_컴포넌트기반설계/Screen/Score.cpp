#include "Score.h"

Score::Score(Vector2 position, int* score, string contents)
	:Text(position, contents), score(score)
{
}


Score::~Score()
{
}

void Score::draw()
{
	char *buffer = new char[contents.size() + 1];
	strncpy(buffer, contents.c_str(), contents.size());
	sprintf(&buffer[contents.size()], "%d", *score);

	Screen::getInstance().draw(buffer, strlen(buffer), 1, position);

}
