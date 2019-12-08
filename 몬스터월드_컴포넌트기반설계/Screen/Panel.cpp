#include "Panel.h"



Panel::Panel(Vector2 position, Vector2 size)
	:UI(position), size(size)
{
}


Panel::~Panel()
{
}

void Panel::draw()
{
	Screen::getInstance().drawRect(position, size.x, size.y);
}
