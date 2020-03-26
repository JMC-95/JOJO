#pragma once
#include "gameNode.h"
#include "MainMap.h"
#include "Interface.h"

class GameScene : public gameNode
{
private:
	MainMap* _mainMap;
	Interface* _interface;

	int pTurnCount;
	int pTurn;
	int fTurnCount;
	int fTurn;
	int eTurnCount;
	int eTurn;

	char str[128];

public:
	GameScene();
	~GameScene();

	HRESULT init();
	void release();
	void update();
	void render();

	void gameTurn();
};
