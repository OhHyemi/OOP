// 몬스터월드.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include <string>
#include <ctime>
#include "Utils.h"

class GameObject {
	bool		enabled;

protected:

	Position	pos;
	Screen&		screen;
	char*		shape;
	int			width;
	int			height;

	vector<GameObject *> children;
	GameObject*	parent;

public:
	GameObject(const char* shape, int width, int height, const Position& pos = Position{ 0, 0 })
		: height(height), width(width), shape(nullptr), pos(pos), enabled(true), parent(nullptr),
		screen(Screen::getInstance()) {
		if (!shape || strlen(shape) == 0)
			this->shape = nullptr;
		else {
			this->shape = new char[width*height];
			strncpy(this->shape, shape, width*height);
		}
		children.clear();
	}

	GameObject(int width, int height, const Position& pos = Position{ 0, 0 })
		: GameObject(nullptr, width, height, pos)
	{}

	virtual ~GameObject()
	{
		while (children.size() > 0) {
			auto back = children.back();
			children.pop_back();
			delete back;
		}
		if (shape) { delete[] shape; }
		shape = nullptr;
		width = 0, height = 0;
	}

	void setParent(GameObject* parent) {
		this->parent = parent;
	}

	void add(GameObject* child) {
		if (!child) return;

		child->setParent(this);
		children.push_back(child);
	}

	virtual bool isActive() { return enabled; }

	void setActive(bool flag = true) { enabled = flag; }

	void setShape(const char* shape) {
		if (!shape) return;
		strncpy(this->shape, shape, width*height);
	}

	virtual void setPos(int x, int y) { this->pos.x = x; this->pos.y = y; }

	int getWidth() const { return width; }
	int getHeight() const { return height; }

	Position& getPos() { return pos; }

	vector<GameObject *>& getChildren() {
		return children;
	}

	void internalDraw(const Position& inheritedPos = Position{ 0, 0 }) {
		if (!enabled) return;

		draw(inheritedPos);

		for (auto& child : children) child->internalDraw(pos + inheritedPos);
	}

	virtual void draw(const Position& inheritedPos = Position{ 0, 0 }) {
		screen.draw(shape, width, height, pos + inheritedPos);
	}

	void internalUpdate() {
		if (!enabled) return;
		update();
		for (auto& child : children) child->internalUpdate();
	}

	virtual void update() {}

	static const char rect = '\xB2';
};

class Panel : public GameObject {

public:
	Panel(const char* layout, int width, int height, const Position& pos) : GameObject(layout, width, height, pos) {}
	Panel(int width, int height, const Position& pos) : GameObject(width, height, pos) {}

	GameObject* pop() {
		auto& children = getChildren();
		auto back = children.back();
		children.pop_back();
		back->setParent(nullptr);
		return back;
	}

	void draw(const Position& inheritedPos) {
		screen.drawRect(Position{ pos.x - 1, pos.y - 1 } +inheritedPos, getWidth() + 2, getHeight() + 2);
	}
};

class Text : public Panel {

public:
	Text(const char* info, const Position& pos) : Panel(info, strlen(info), 1, pos) {}

	void draw(const Position& inheritedPos) {
		screen.draw(shape, width, 1, pos + inheritedPos);
	}
};

class Score : public Text {

	int score;
	char* buf;

public:
	Score(const Position& pos) : Text("Score:     ", pos), score(0) {
		buf = new char[strlen(shape) + 1];
	}

	~Score() {
		delete[] buf;
	}

	void addScore(int inc) {
		if (inc < 0) return;
		score += inc;
	}

	void minusScore(int inc) {
		if (inc < 0 || score < inc) return;
		score -= inc;
	}

	void draw(const Position& inheritedPos) {
		sprintf(buf, "Score: %3d", score);
		screen.draw(buf, strlen(buf), 1, pos + inheritedPos);
	}

	void setScore(int score) { this->score = score; }
};

class Map : public Panel {
	bool* map;
	bool* fruit;
	int   width;
	int	  height;
	char*  buffer;
	Score *fruitCount;
	Score *allMovementCount;

public:
	Map(int width, int height, const Position& pos)
		: map(new bool[width*height]), width(width), height(height), buffer(new char[width * height]),
		fruit(new bool[width * height]),fruitCount(nullptr), allMovementCount(nullptr),
		Panel(nullptr, width, height, pos)
	{
		for (int i = 0; i < width*height; i++)
		{
			map[i] = false;
			fruit[i] = true;
		}
		memset(buffer, '\xF9', 150);
	}

	~Map() { if (map) delete[] map; }
	
	void setFruitCountScore(Score* score) {
		fruitCount = score;
	}
	void minusFruitCountScore() {
		fruitCount->minusScore(1);
	}

	void setallMovementCountScore(Score* score) {
		allMovementCount = score;
	}
	void addAllMovementCountScore() {
		allMovementCount->addScore(1);
	}

	bool isOccupied(const Position& pos) {
		return map[pos.x + pos.y * width];
	}

	void occupiedMap(const Position& pos) {
		map[pos.x + pos.y * width] = true;
	}

	void unocuppiedMap(const Position& pos) {
		map[pos.x + pos.y * width] = false;
	}

	void eatFruit(const Position& pos) {
		fruit[pos.x + pos.y * width] = false;
	}

	bool isFruit(const Position& pos) {
		return fruit[pos.x + pos.y * width];
	}

	Score* getFruitCountScore() { return fruitCount; }
	
	
	void draw(const Position& inheritedPos)
	{
		screen.drawRect(Position{ pos.x - 1, pos.y - 1 } +inheritedPos, width + 2, height + 2);
		for (int i = 0; i < height; i++) {
						
			for (int j = 0; j < width; j++) {
				
				if (map[j + i * width]) buffer[j + i * width] = ' ';
				
			}
			
		}
		screen.draw(buffer, width, height, Position{ pos.x, pos.y} +inheritedPos);
	}
};


class Monster : public GameObject
{
	Map *map;
	string name;
	Score* score;	
	
public:
	Monster(string name, char shape, Position pos)
		:name(name), map(map), score(nullptr), GameObject(&shape, 1, 1, pos){}

	void setMap(Map* map) { this->map = map; }

	void moveRandomly()
	{
		int randomD = (unsigned)rand() % 8;

		Position nextPos = pos;

		switch(randomD)
		{
		case 0://왼쪽 위
			nextPos.x --;
			nextPos.y --;
			break;
		case 1://중앙 위
			nextPos.y --;	
			break;
		case 2://오른쪽 위
			nextPos.x++;
			nextPos.y--;
			break;
		case 3://왼쪽
			nextPos.x--;
			break;
		case 4://오른쪽
			nextPos.x++;
			break;
		case 5://왼쪽 아래
			nextPos.x--;
			nextPos.y++;
			break;
		case 6://중앙 아래
			nextPos.y++;
			break;
		case 7://오른쪽 아래
			nextPos.x++;
			nextPos.y++;
			break;
		}


		if ( !isInMap(nextPos) || (map->isOccupied(nextPos)))
			return;
		
		map->unocuppiedMap(pos);
		pos = nextPos;
		map->occupiedMap(pos);
		map->addAllMovementCountScore();
		
		if (!(map->isFruit(pos)))
			return;
		addScore();
		
		map->eatFruit(pos);
		map->minusFruitCountScore();
	}

	void setScore(Score* score) {
		this->score = score;
	}
	void addScore() {
		if (this->score) this->score->addScore(1);
	}

	
	bool isInMap(Position pos)
	{
		if ( map->getWidth() -1  < pos.x || pos.x < 0
			||  map->getHeight() -1 < pos.y || pos.y < 0)
			return false;
		return true;
	}
	
	void update()
	{		
		moveRandomly();
	}
	string getName() { return name; }
	
};

int main()
{
	Screen& screen = Screen::getInstance();


	vector<GameObject *> gameObjects;
	auto map = new Map(15, 10, Position{ 1, 1 });
	

	auto monster1 = new Monster("Oimi", 'O',
		Position{ map->getPos().x + map->getWidth() / 4, map->getPos().y + map->getHeight() / 4 });
	auto monster2 = new Monster("yong", 'Y',
		Position{ map->getPos().x + map->getWidth() / 4 + 2, map->getPos().y + map->getHeight() / 4 + 2 });
	auto monster3 = new Monster("Mie", 'M',
		Position{ map->getPos().x + map->getWidth() / 4 , map->getPos().y + map->getHeight() / 4 + 2 });
	auto*monster4 = new Monster("tack", 'T',
		Position{ map->getPos().x + map->getWidth() / 4 + 2, map->getPos().y + map->getHeight() / 4 });

	monster1->setMap(map);
	monster2->setMap(map);
	monster3->setMap(map);
	monster4->setMap(map);

	map->add(monster1);
	map->add(monster2);
	map->add(monster3);
	map->add(monster4);

	srand(time(nullptr));

	string mode{ "mode con cols="
		+ to_string(screen.getWidth() + 20)
		+ " lines=" + to_string(screen.getHeight() + 10) };
	system(mode.c_str());
	system("chcp 437");

	auto scorePanel = new Panel(nullptr, 15, 8, Position{ map->getWidth() + 3, 1 });

	scorePanel->add(new Text(monster1->getName().c_str(), Position{ 1,0 }));
	auto score1 = new Score(Position{ 1,1});
	scorePanel->add(score1);
	monster1->setScore(score1);
	
	scorePanel->add(new Text(monster2->getName().c_str(), Position{ 1,2 }));
	auto score2 = new Score(Position{ 1,3 });
	scorePanel->add(score2);
	monster2->setScore(score2);
	
	scorePanel->add(new Text(monster3->getName().c_str(), Position{ 1,4 }));
	auto score3 = new Score(Position{ 1,5 });
	scorePanel->add(score3);
	monster3->setScore(score3);
	
	scorePanel->add(new Text(monster4->getName().c_str(), Position{ 1,6 }));
	auto score4 = new Score(Position{ 1,7 });
	scorePanel->add(score4);
	monster4->setScore(score4);

	

	auto countPanel = new Panel(nullptr, 15, 4, Position{ 1, map->getHeight() + 3 });

	countPanel->add(new Text("moveCount", Position{ 1,0 }));
	auto moveCountScore = new Score(Position{ 1,1 });
	countPanel->add(moveCountScore);
	map->setallMovementCountScore(moveCountScore);

	countPanel->add(new Text("fruitCount", Position{ 1,2 }));
	auto fruitCountScore = new Score(Position{ 1,3 });
	countPanel->add(fruitCountScore);
	map->setFruitCountScore(fruitCountScore);
	map->getFruitCountScore()->setScore(map->getWidth()*map->getHeight());
	
			
	gameObjects.push_back(map);
	gameObjects.push_back(scorePanel);
	gameObjects.push_back(countPanel);


	screen.clear(); screen.render();
	while(true)
	{ 
	
		screen.clear();
		for (auto obj : gameObjects) obj->internalUpdate();
		
		for (auto it = gameObjects.cbegin();
			it != gameObjects.cend(); it++)
			(*it)->internalDraw();

		Sleep(100);

		screen.render();

		Input::EndOfFrame();
	}
}

