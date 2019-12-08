#include "MonsterScript.h"
#include "GameObject.h"
#include "Utils.h"
#include "Transform.h"
#include "MapScript.h"
#include <time.h>


MonsterScript::MonsterScript(GameObject* gameObject, string shape)
	:NumOfEatingFood(0), Component(gameObject)
{
}

MonsterScript::~MonsterScript()
{
}

void MonsterScript::start()
{
	srand(time(nullptr));
}

void MonsterScript::update()
{
	moveRandomly();
	draw();
}

//�����ϰ� �����̱�
void MonsterScript::moveRandomly()
{
	Vector2 myPosition = gameObject->getTransform()->getPosition();

	int randomX = (unsigned)rand() % 3 - 1;
	int randomY = (unsigned)rand() % 3 - 1;

	gameObject->getTransform()->setPosition(gameObject->getTransform()->getPosition() + Vector2(randomX, randomY));

	if (!isInMap() || collsion())
	{
		gameObject->getTransform()->setPosition(myPosition);
		return;
	}
}

//���Ϳ� �ε����� true ��ȯ, Ǫ��� �ε����� Ǫ�带 �԰� false ��ȯ
bool MonsterScript::collsion()
{
	vector<GameObject*> objs = gameObject->getParent()->getChildrens();
	for (auto obj : objs)
	{
		//enable ���°� �ƴϰų� ���ٸ� �Լ��� �� ������..
		if (!obj || !obj->isActive())
			continue;

		if (obj != gameObject
			&& obj->getTag() == "Monster"
			&& gameObject->getTransform()->getPosition() == obj->getTransform()->getPosition())
			return true;
		if (obj->getTag() == "Food"
			&& gameObject->getTransform()->getPosition() == obj->getTransform()->getPosition())
		{
			eatFood(obj);
			return false;
		}
	}
	return false;
}

bool MonsterScript::isInMap()
{
	MapScript *map = dynamic_cast<MapScript*>(gameObject->Find("map")->getComponent<MapScript>());
	if (!map)
		return false;

	if (gameObject->getTransform()->getPosition().x <= map->getStartPosition().x ||
		gameObject->getTransform()->getPosition().x >= map->getStartPosition().x + map->getSize().x ||
		gameObject->getTransform()->getPosition().y >= map->getStartPosition().y + map->getSize().y ||
		gameObject->getTransform()->getPosition().y <= map->getStartPosition().y)
		return false;
	
	return true;
}


void MonsterScript::eatFood(GameObject* food)
{
	food->setActive(false);
	food->destroy(&food);
	NumOfEatingFood++;
}

void MonsterScript::draw()
{
	if (!(gameObject->isActive())) 
		return;
	Screen::getInstance().draw(monsterShape.c_str(), transform->getScale().x, transform->getScale().y, transform->getPosition());
}


