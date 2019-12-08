#include "Component.h"
#include "Transform.h"

Transform::Transform(GameObject* gameObject, 
	const Vector2& position,
	const Vector2& rotation,
	const Vector2& scale)
	: position(position), 
	rotation(rotation), 
	scale(scale),
	Component(gameObject)
{
	
}

Transform::~Transform()
{
}
