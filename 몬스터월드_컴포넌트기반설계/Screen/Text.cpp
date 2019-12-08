#include "Text.h"



Text::Text(Vector2 position, string contents)
	:UI(position), contents(contents)
{
}


Text::~Text()
{
}

void Text::draw()
{
	Screen::getInstance().draw(contents.c_str(), contents.size(),1,position);
}
