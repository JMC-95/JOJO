#pragma once
#include "gameNode.h"

class LobbyScene : public gameNode
{
private:
	vector<RECT> vMenu;
	vector<RECT> vOut;
	vector<RECT> vEquipment;
	vector<RECT> vBuy;
	vector<RECT> vSell;

private:
	RECT menuRect[4];
	RECT outRect[9];
	RECT equipRect[3];
	RECT buyRect[5];
	RECT sellRect[3];

	int playerNum;
	int playerCount;
	int frameX;

	char str[128];

	bool isOut;
	bool isEquipment;
	bool isBuy;
	bool isSell;

public:
	LobbyScene();
	~LobbyScene();

	HRESULT init();
	void release();
	void update();
	void render();
};
