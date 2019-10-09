// Screen.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include <string>
#include <ctime>
#include "Utils.h"
#include"sprites.h"

using namespace std;

class GameObject 
{
	char*		shape;
	int			width;
	int			height;
	Position	pos;
	Screen&		screen;

	vector<GameObject *> children;


public:
	GameObject(const char* shape, int width, int height, const Position& pos = Position{ 0, 0 } )
		: height(height), width(width), 
		shape(nullptr), pos(pos), 
		screen(Screen::getInstance()) {
		if (!shape || strlen(shape) == 0 || width == 0 || height == 0)
		{
			this->shape = new char[1];
			this->shape[0] = 'x';
			width = 1;
			height = 1;
		} else {
			this->shape = new char[width*height];
			strncpy(this->shape, shape, width*height);
		}
		this->width = width;
		this->height = height;
	}

	virtual ~GameObject() {
		if (shape) { delete[] shape; }
		width = 0, height = 0;
	}

	void add(GameObject* child) {
		if (!child) return;
		children.push_back(child);
	}

	void setShape(const char* shape) {
		if (!shape) return;
		strncpy(this->shape, shape, width*height);
	}

	void setPos(int x, int y) { this->pos.x = x; this->pos.y = y; }

	Position& getPos() { return pos; }
	int& getWidth() { return width; }
	int& getHeight() { return height; }

	void internalDraw(const Position& accumulatedPos = Position{ 0,0 })
	{
		//draw(accumulatedPos);
		for (auto child : children) 
			child->internalDraw( pos + accumulatedPos );
	}
		
	virtual void draw()
	{		
		screen.draw(shape, width, height, pos );
	}
	virtual void draw(const Position & pos)
	{
		screen.draw(shape, width, height, pos);
	}
	void drawBlock()
	{
		screen.drawBlock(shape, width, height, pos);
	}
	void drawBlock(const Position & pos)
	{
		screen.drawBlock(shape, width, height, pos);
	}
	void internalUpdate()
	{
		update();
		for (auto child : children)
			child->internalUpdate();
	}

	virtual void update() 
	{	
	}

	vector<GameObject *>& getChildren() { return children;  }
};

class GameBoard
{
	char* shape;
	char* cleanLineShape = "\xF9\xF9\xF9\xF9\xF9\xF9\xF9\xF9\xF9\xF9";
	int width;
	int height;
	int bonus;
	Position pos;

	static GameBoard* boardInstance;

public:
	GameBoard(const char* shape, int width, int height, const Position& pos = Position{ 0, 0 }) 
		: height(height), width(width),
		shape(nullptr), pos(pos),bonus(0)
	{}

	GameBoard( int width = 10, int height = 20, const Position& pos = { 1,1 })
		:width(width), height(height), pos(pos),
		shape(new char[width * height])
	{
		memset(shape, '\xF9', width * height);

	}

	static GameBoard& getInstance() 
	{
		if (boardInstance == nullptr) 
		{
			boardInstance = new GameBoard();
		}
		return *boardInstance;
	}

	~GameBoard()
	{
		if (shape)
		{
			delete[] shape;
			shape = nullptr;
		}

	}

	Position& getPos() { return pos; }
	int& getWidth() { return width; }
	int& getHeight() { return height; }
	char* getShape() { return shape; }
	int getBonus() { return bonus; }
	
	void draw()
	{
		Screen::getInstance().draw(shape, width, height, pos);
	}

	void drawBlock(const char* shape, int w, int h, const Position& pos)
	{
		if (!shape) return;
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				if (shape[i * w + j] == '\xB0')
					strncpy(&(this->shape)[(pos.y + i - 1) * width + pos.x - 1 + j], &shape[i * w + j], 1);
			}
		}
	}

	void eraseBlockLine()
	{
		for (int h = 0; h < height; h++)
		{
			for (int w = 0; w < width; w++)
			{
				if (shape[h * width + w] != '\xB0')
					break;
				if (w == width - 1)
				{
					strncpy(&shape[h * width + w - 9], cleanLineShape, 10);
					dropLines(h);
					bonus += 10;
				}

			}

		}
	}

	void dropLines(int h)
	{
		for (int i = h - 1; i > 0; i--)
		{
			strncpy(&shape[(i+1) * width], &shape[i * width], 10);
		}
		strncpy(&shape[0], cleanLineShape, 10);
		
	}
};
	GameBoard* GameBoard::boardInstance = nullptr;

	class Block : public GameObject
	{

		vector<string> sprites;
		int current;
		bool stacked;
		GameBoard& gameBoard;
		Position pPos;


	public:

		Block(vector<string>& sprites, int w, int h,
			const Position& pos = Position{ 0,0 })
			: sprites(sprites),
			current(0),
			stacked(false),
			GameObject(sprites[current].c_str(), w, h, pos),
			gameBoard(GameBoard::getInstance())
		{}
		vector<string> getSprites() { return sprites; }
		void setSprites(vector<string> sprites) { this->sprites = sprites; };
		string getShape() { return sprites[0]; }
		
		
		bool inRectR()//오른쪽
		{
			for (int h = 0; h < getHeight(); h++)
			{
				for (int w = 0; w < getWidth(); w++)
				{
					if (sprites[current].c_str()[h*getWidth() + w] == '\xB0'
						&& getPos().x + w + 1 == gameBoard.getPos().x + gameBoard.getWidth()
						)
						return false;
				}
			}
			return true;
		}
		bool inRectL()//왼쪽
		{
			for (int h = 0; h < getHeight(); h++)
			{
				for (int w = getWidth() - 1; w >= 0; w--)
				{
					if (sprites[current].c_str()[h*getWidth() + w] == '\xB0'
						&& getPos().x + w == gameBoard.getPos().x)
						return false;
				}
			}
			return true;
		}

		bool ableRotation()
		{
			const char* nextSprite = sprites[(current + 1) % sprites.size()].c_str();

			for (int h = 0; h < getHeight(); h++)
			{
				for (int w = 0; w < getWidth(); w++)
				{
					if (nextSprite[h * getWidth() + w] == '\xB0'
						&& getPos().x + getWidth() - w - 1 == gameBoard.getPos().x + gameBoard.getWidth())
						return false;
					if (nextSprite[h * getWidth() + w] == '\xB0'
						&& getPos().x + getWidth() - w - 1 == gameBoard.getPos().x - 1)
						return false;
				}
			}
			return true;
		}
		

		bool checkOtherBlock()
		{
			for (int h = getHeight() - 1; h >= 0; h--)
			{
				for (int w = 0; w < getWidth(); w++)
				{
					if (sprites[current].c_str()[h * getWidth() + w] == '\xB0'
						&& gameBoard.getShape()[(getPos().y + h) * (gameBoard.getWidth())
						+ (getPos().x) + w - 1] != '\xF9')
						return true;
				}
			}
			return false;
		}

		bool isStacked() { return stacked; }

		void update() {

			if (stacked)
			{
				//gameBoard.checkBlockLine();
				stacked = false;
				getPos().x = 5;
				getPos().y = 1;
				return;
			}

			if (Input::GetKeyDown(KeyCode::Right)) {
				if (!inRectR() )
					getPos().x--;

				getPos().x++;
			}
			if (Input::GetKeyDown(KeyCode::Left)) {
				if (!inRectL() )
					getPos().x++;

				getPos().x--;
			}
			if (Input::GetKeyDown(KeyCode::Up)) {
				if (ableRotation())
				{
					current = (current + 1) % sprites.size();
					setShape(sprites[current].c_str());
				}
			}
			if (Input::GetKeyDown(KeyCode::Down)) {
				if (checkOtherBlock())
					return;
				if (getPos().y + 2 < gameBoard.getPos().y + gameBoard.getHeight())
					getPos().y += 2;
			}
			if (checkOtherBlock())
			{
				stacked = true;
				gameBoard.drawBlock(sprites[current].c_str(), getWidth(), getHeight(), getPos());
				return;
			}
			else
				getPos().y = (getPos().y + 1) % (gameBoard.getHeight());
		}

	};



	int main()
	{

		Screen&	 screen = Screen::getInstance();
		GameBoard& gameboard = GameBoard::getInstance();
		
		vector<Block*> blocks;
		
		Block *block1 = new Block{ sprites1, 3,3, (5, 1) };
		Block *block2 = new Block{ sprites2, 3,3, (5, 1) };
		Block *block3 = new Block{ sprites3, 3,3, (5, 1) };
		Block *block4 = new Block{ sprites4, 2,2, (5, 1) };
		Block *block5 = new Block{ sprites5, 4,4, (5, 1) };
		Block *block6 = new Block{ sprites6, 3,3, (5, 1) };
		Block *block7 = new Block{ sprites6, 3,3, (5, 1) };

		blocks.push_back(block1);
		blocks.push_back(block2);
		blocks.push_back(block3);
		blocks.push_back(block4);
		blocks.push_back(block5);
		blocks.push_back(block6);
		blocks.push_back(block7);


		srand(time(nullptr));

		string mode{ "mode con cols="
			+ to_string(screen.getWidth()+20)
			+ " lines=" + to_string(screen.getHeight()+10) };
		system(mode.c_str());
		system("chcp 437");

		screen.clear(); screen.render();
		
		int random = (unsigned)rand() % blocks.size();
		int next = 0;
		int score = 0;
		
		Block *nextBlock;
		nextBlock = blocks[next];
		
		string nextblock = "Next Block";
		string yourScore = "Your Score :";

		while (true)
		{
			screen.clear(); //ui.clear();
			gameboard.draw();
			screen.render(); //ui.render();

			screen.drawRect((0, 0), 12, 22);//게임판테두리


			if (blocks[random]->isStacked())
			{
				random = next;
				next = (unsigned)rand() % blocks.size();
				score += 4;

				nextBlock = blocks[next];
			}
			
			blocks[random]->update();
			blocks[random]->drawBlock();

			//ui
			screen.drawRect(Position(15, 0), 20, 8);//다음블록알려주는판테두리
			screen.draw(nextblock.c_str(), nextblock.size(),1, Position(20, 2));
			screen.draw((yourScore + to_string(score + gameboard.getBonus())).c_str(), to_string(score).size() +12 , 1, Position(15, 10));
			screen.drawBlock(blocks[next]->getShape().c_str(), blocks[next]->getWidth(),blocks[next]->getHeight(), Position(22,3));
		
			screen.render();
			
			Sleep(250);
			gameboard.eraseBlockLine();
			gameboard.draw();
			
			screen.render();
					
			Input::EndOfFrame();
			
		}

		return 0;
	}