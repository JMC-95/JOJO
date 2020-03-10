#pragma once
#include "gameNode.h"
#include "testMap.h"

enum Select
{
	SELECT_START,
	SELECT_END
};

enum Direction
{
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_LEFTUP,
	DIRECTION_RIGHTUP,
	DIRECTION_LEFTDOWN,
	DIRECTION_RIGHTDOWN
};

enum State
{
	STATE_NONE,
	STATE_OPEN,
	STATE_CLOSE,
	STATE_PATH
};

class aStar : public gameNode
{
private:
	tagTile _tiles[TILEX * TILEY];

	vector<int> openList;
	vector<int> closeList;
	vector<int>::iterator iter;

	Select currentSelect;

	int startTile;
	int endTile;
	int currentTile;

	bool isFind;
	bool noPath;
	bool startAstar;

	RECT rc[5];

	HBRUSH brush;
	HFONT font, oldFont;
	char str[128];
	int temp;

	//세이브 파일
	const char* fileName[5] = { "SaveMap_1.map","SaveMap_2.map","SaveMap_3.map","SaveMap_4.map","SaveMap_5.map" };

public:
	aStar();
	~aStar();

	HRESULT init();
	void release();
	void update();
	void render();

	void Astar();
	void Load();
};
