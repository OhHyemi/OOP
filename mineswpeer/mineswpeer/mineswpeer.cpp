#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include "Utils.h"

using namespace std;

class Screen {
	int width;
	int height;
	char* canvas;

	static Screen* instance;
	Screen(int width = 70, int height = 25)
		: width(width), height(height),
		canvas(new char[(width + 1)*height])
	{
		Borland::initialize();
	}
public:
	static Screen& getInstance() {
		if (instance == nullptr) {
			instance = new Screen();
		}
		return *instance;
	}

	~Screen() {
		if (instance) {

			delete[] canvas;
			instance = nullptr;
		}
	}

	
	void draw(const char* shape, int w, int h)
	{
		if (!shape) return;
		for (int i = 0; i < h; i++)
		{
			strncpy(&canvas[i*(width + 1)], &shape[i*w], w);
		}
	}

	void render()
	{
		for (int i = 0; i < height; i++)
			canvas[width + i * (width + 1)] = '\n';
		canvas[width + (height - 1) * (width + 1)] = '\0';
		Borland::gotoxy(0, 0);
		cout << canvas;
	}

	void clear()
	{
		memset(canvas, ' ', (width + 1)*height);
		canvas[width + (height - 1)*(width + 1)] = '\0';
	}

};

Screen* Screen::instance = nullptr;

class MineSwpeer
{
	int width;
	int height;
	int* mines;
	int nMines;
	int foundMines;
	char* mineMap;
	char* mineMapForShow;
	bool* visited;
	bool gameOver;

	Screen& screen;


public:
	MineSwpeer(int width, int height, int nMines)
		: width(width), height(height), nMines(nMines),
		mineMap(new char[(width + 1) * height]),
		mineMapForShow(new char[(width + 1) * height]),
		mines(new int[nMines]),
		visited(new bool[(width + 1) * height]),
		gameOver(false),foundMines(0),
		screen(Screen::getInstance())
	{

		Borland::initialize();
	}
	virtual~MineSwpeer()
	{
		if (mines) { delete[] mines; }
		if (mineMap) { delete[] mineMap; }
		if (visited) { delete[] visited; }
		width = 0, height = 0;
	}
	void Start()
	{
		memset(mineMap, '/', (width + 1)*height);
		memset(mineMapForShow, '/', (width + 1)*height);
		for (int i = 0; i < height; i++)
		{
			mineMapForShow[width + i * (width + 1)] = ' ';
		}

		for (int i = 0; i < (width + 1)*height; i++)
		{
			visited[i] = false;
		}
		drawMap();
		placeMines();
	}

	void Update(const Position pos)
	{
		digMap(pos);
		drawMap();
	}

	bool IsGameOver(const Position pos)
	{
		if (mineMap[pos.x + pos.y * (width + 1) ] == '*')
		{
			drawAnswer();
			return true;
		}
		return false;
	}

	bool WinGame()
	{
		int foundMine = 0;
		for (int i = 0; i < (width + 1) * height; i++)
		{
			if (mineMapForShow[i] == '/')
				foundMine++;
		}
		if (foundMine == nMines)
			return true;
		else
			return false;
	}

	void ShowGameInfo()
	{
		Borland::gotoxy(width + 5, height / 2);
		cout << "width : " << width;
		Borland::gotoxy(width + 5, height / 2 + 2);
		cout << "height : " << height;
		Borland::gotoxy(width + 5, height / 2 + 4);
		cout << "mines : " << nMines;
	}
private:

	void drawMap()
	{
		screen.draw(mineMapForShow, width+1, height);
	}

	void drawAnswer()
	{
		screen.draw(mineMap, width + 1, height);
	}

	void placeMines()
	{
		srand((unsigned int)time(NULL));

		int randomMinePlace;

		for (int i = 0; i < nMines; i++)
		{
			randomMinePlace = rand() % ((width + 1) * height);
			mines[i] = randomMinePlace;
			for (int j = 0; j < i; j++)
			{
				if (mines[i] == mines[j] || (randomMinePlace % (width + 1) == width))
				{
					i--;
					break;
				}
			}
		}

		for (int i = 0; i < nMines; i++)
		{
			mineMap[mines[i]] = '*';
		}

		setAdjacentMineNum();

	}

	void setAdjacentMineNum()
	{
		for (int i = 0; i < height; i++)
		{
			mineMap[width + i * (width + 1)] = ' ';
		}

		for (int i = 0; i < (width + 1) * height; i++)
		{
			int mineCount = 0;

			if (mineMap[i] == '*' || mineMap[i] == ' ')
				continue;

			if (mineMap[i - 1] == '*' && (i % (width + 1) != 0)) mineCount++;
			if (mineMap[i + 1] == '*' && (i % (width + 1) != width - 1)) mineCount++;

			if (i > width)
			{
				if (mineMap[i - (width + 1)] == '*') mineCount++;
				if (mineMap[i - (width + 1) - 1] == '*' && (i % (width + 1) != 0)) mineCount++;
				if (mineMap[i - (width + 1) + 1] == '*' && (i % (width + 1) != width - 1)) mineCount++;
			}
			if (i < ((width + 1)*(height - 1)))
			{
				if (mineMap[i + (width + 1)] == '*') mineCount++;
				if (mineMap[i + (width + 1) - 1] == '*' && (i % (width + 1) != 0)) mineCount++;
				if (mineMap[i + (width + 1) + 1] == '*' && (i % (width + 1) != width - 1)) mineCount++;
			}
			mineMap[i] = mineCount + '0';


		}
		mineMap[width + (height - 1)*(width + 1)] = '\0';
	}

	void digMap(Position pos)
	{
		Borland::gotoxy(0, 0);
		if ((pos.x + pos.y * (width + 1) + 1) % (width + 1) == 0 || pos.y > height ||
			pos.x < 0 || pos.y < 0 )
			return;

		int mPos = pos.x + pos.y * (width + 1);

		
		if (mineMap[mPos] == '*')
		{
			visited[mPos] = true;
			return;
		}

		if (visited[mPos] || mineMap[mPos] != '0')
		{
			mineMapForShow[mPos] = mineMap[mPos];
			return;
		}
		else
		{
			visited[mPos] = true;
			pos.x -= 1; pos.y -= 1;//왼쪽 위
			digMap(pos);
			pos.x += 1;	//위
			digMap(pos);
			pos.x += 1; //오른쪽 위
			digMap(pos);
			pos.y += 1; //오른쪽 가운데
			digMap(pos);
			pos.y += 1; //오른쪽 아래
			digMap(pos);
			pos.x -= 1; //아래
			digMap(pos);
			pos.x -= 1; //왼쪽 아래
			digMap(pos);
			pos.y -= 1; //왼쪽 가운데
			digMap(pos);

		}
		
		mineMapForShow[mPos] = mineMap[mPos];//콘솔에 눈에 봉
	}
	
	bool winGame()
	{
		int mine = 0;
		for (int i = 0; i < height * (width + 1); i++)
		{
			if (mineMapForShow[i] == '-')
				mine++;
		}

		if (mine == nMines)
			return true;
		else
			return false;
	}
};




int main()
{
	MineSwpeer mineSwpeer(15, 15, 50);
	Screen&	screen = Screen::getInstance();
	INPUT_RECORD InputRecord;
	DWORD Events;
	bool win = false;
	bool gameover = false;

	screen.clear();
	mineSwpeer.Start();
	screen.render();


	while (true)
	{
		mineSwpeer.ShowGameInfo();

		ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &InputRecord, 1, &Events);
		if (InputRecord.EventType == MOUSE_EVENT) {
			if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
				COORD coord;
				coord.X = InputRecord.Event.MouseEvent.dwMousePosition.X;
				coord.Y = InputRecord.Event.MouseEvent.dwMousePosition.Y;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
				Position pos;
				pos.x = coord.X;
				pos.y = coord.Y;
				
				mineSwpeer.Update(pos);
				win = mineSwpeer.WinGame();
				gameover = mineSwpeer.IsGameOver(pos);
				if (win || gameover)
					break;
				
				screen.render();
			}
		}

	}
	screen.render();
	screen.clear();
	Sleep(1000);
	screen.render();

	Borland::gotoxy( 25, 5 );
	
	if(gameover)
		cout << "^_^@@@@@@@@@GAME OVER@@@@@@@@@^_^ " <<endl;
	if(win)
		cout << ">_<★★★★★YOU WIN★★★★★>_< " << endl;
	
	return 0;

}

