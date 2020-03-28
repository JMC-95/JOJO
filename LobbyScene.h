#pragma once
#include "gameNode.h"

class LobbyScene : public gameNode
{
private:
	RECT menuRect[4];

	char str[128];

public:
	LobbyScene();
	~LobbyScene();

	HRESULT init();
	void release();
	void update();
	void render();
};

