#pragma once
#include "gameNode.h"
#include "MainMap.h"

class GameScene : public gameNode
{
private:
	MainMap* mainMap;

public:
	GameScene();
	~GameScene();

	HRESULT init();
	void release();
	void update();
	void render();
};
