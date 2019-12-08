#include "GameEngine.h"
#include "GameObject.h"
#include "MoveScript.h"
#include "FoodScript.h"
#include "MonsterScript.h"
#include "Transform.h"
#include "MapScript.h"
#include "UI.h"
#include "Panel.h"
#include "Text.h"
#include "Score.h"


GameEngine* GameEngine::instance = nullptr;

GameEngine& GameEngine::getInstance() {
	if (instance == nullptr) {
		instance = new GameEngine();
	}
	return *instance;
}


GameEngine::GameEngine() 
	: screen(Screen::getInstance()) {
	string mode = "mode con cols=" + to_string(screen.getWidth() + 10);
	mode += " lines=" + to_string(screen.getHeight() + 5);

	std::system(mode.c_str());
	std::system("chcp 437");
	screen.clear(); screen.render();
}

void GameEngine::mainLoop() {

	vector<UI*>uis;
	auto& objs = GameObject::gameObjects;

	//맵
	GameObject map("map");
	map.addComponent<MapScript>();
	objs.push_back(&map);

	//몬스터
	GameObject monster1("monster", &map, "Monster");
	monster1.addComponent<MonsterScript>();
	monster1.getTransform()->setPosition(Vector2(3, 3));
	dynamic_cast<MonsterScript*>(monster1.getComponent<MonsterScript>())->setShape("*");

	objs.push_back(&monster1);

	GameObject monster2("ghost", &map, "Monster");
	monster2.addComponent<MonsterScript>();
	monster2.getTransform()->setPosition(Vector2(1, 1));
	dynamic_cast<MonsterScript*>(monster2.getComponent<MonsterScript>())->setShape("+");

	objs.push_back(&monster2);

	GameObject monster3("Zombie", &map, "Monster");
	monster3.addComponent<MonsterScript>();
	monster3.getTransform()->setPosition(Vector2(4, 1));
	dynamic_cast<MonsterScript*>(monster3.getComponent<MonsterScript>())->setShape("=");

	objs.push_back(&monster3);

	GameObject monster4("Wolf", &map, "Monster");
	monster4.addComponent<MonsterScript>();
	monster4.getTransform()->setPosition(Vector2(5, 5));
	dynamic_cast<MonsterScript*>(monster4.getComponent<MonsterScript>())->setShape("^");

	objs.push_back(&monster4);


	//위치 조절에 쓸 맵 정보
	MapScript* mapScript = dynamic_cast<MapScript*>(map.getComponent<MapScript>());
	
	//푸드
	for (int i = 1; i < mapScript->getSize().x; i++)
	{
		for (int j = 1; j < mapScript->getSize().y; j++)
		{
			string name = "food(" + i + j;
			name += ")";
			GameObject *food = new GameObject(name, &map, "Food");
			food->getTransform()->setPosition(mapScript->getStartPosition() + Vector2(i, j));
			food->addComponent<FoodScript>();
			objs.push_back(food);
		}
	}
	
	//UI
	{
		//박스
		Panel *mapBox = new Panel(mapScript->getStartPosition(), mapScript->getSize()+Vector2(1,1));
		uis.push_back(mapBox);
		
		Panel *scoreBox = new Panel(mapBox->getPosition() + Vector2(mapBox->getSize().x + 1,0)
			, Vector2(15, 10));
		uis.push_back(scoreBox);

		Panel *foodInfoBox = new Panel(scoreBox->getPosition() + Vector2(scoreBox->getSize().x +1, 0)
			, Vector2(20, 5));
		uis.push_back(foodInfoBox);
		
		//몬스터 이름 및 모양 + 점수
		Text *monsterName1 = new Text(scoreBox->getPosition() + Vector2(2, 1)
			, monster1.name +" "+ dynamic_cast<MonsterScript*>(monster1.getComponent<MonsterScript>())->getShape());
		uis.push_back(monsterName1);
		
		Score *mosterScore1 = new Score(monsterName1->getPosition() + Vector2(0, 1)
			, dynamic_cast<MonsterScript*>(monster1.getComponent<MonsterScript>())->getNumOfEatingFood());
		uis.push_back(mosterScore1);
		
		Text *monsterName2 = new Text(scoreBox->getPosition() + Vector2(2, 3)
			,monster2.name + " " + dynamic_cast<MonsterScript*>(monster2.getComponent<MonsterScript>())->getShape());
		uis.push_back(monsterName2);

		Score *mosterScore2 = new Score(monsterName2->getPosition() + Vector2(0, 1)
			, dynamic_cast<MonsterScript*>(monster2.getComponent<MonsterScript>())->getNumOfEatingFood());
		uis.push_back(mosterScore2);

		Text *monsterName3 = new Text(scoreBox->getPosition() + Vector2(2, 5)
			, monster3.name + " " + dynamic_cast<MonsterScript*>(monster3.getComponent<MonsterScript>())->getShape());
		uis.push_back(monsterName3);

		Score *mosterScore3 = new Score(monsterName3->getPosition() + Vector2(0, 1)
			, dynamic_cast<MonsterScript*>(monster3.getComponent<MonsterScript>())->getNumOfEatingFood());
		uis.push_back(mosterScore3);

		Text *monsterName4 = new Text(scoreBox->getPosition() + Vector2(2, 7)
			, monster4.name + " " + dynamic_cast<MonsterScript*>(monster4.getComponent<MonsterScript>())->getShape());
		uis.push_back(monsterName4);

		Score *mosterScore4 = new Score(monsterName4->getPosition() + Vector2(0, 1)
			, dynamic_cast<MonsterScript*>(monster4.getComponent<MonsterScript>())->getNumOfEatingFood());
		uis.push_back(mosterScore4);

		//몬스터 수
		Score *mostern = new Score(foodInfoBox->getPosition() + Vector2(2, 1)
			, mapScript->getNumOfMonster(), "Num of Monster:");
		uis.push_back(mostern);
		
		//남은 푸드 갯수
		Score *foodn = new Score(foodInfoBox->getPosition() + Vector2(2, 3)
			, mapScript->getNumOfFood(), "Num of Food:");
		uis.push_back(foodn);
	}

	for (auto obj : objs)
	{
		obj->traverseStart();
	}

	while (!Input::GetKeyDown(KeyCode::Esc)) {
		screen.clear();

		// update		
		for (auto obj : objs)
		{
			obj->traverseUpdate();
		
		}

		// UI draw
		for (auto ui : uis)
		{
			ui->draw();
		}

		screen.render();
		Sleep(100);

		Input::EndOfFrame();
	}
	return;
}