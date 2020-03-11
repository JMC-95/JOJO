#pragma once
#include "gameNode.h"
#include "MainMap.h"
#include "tank.h"

class GameScene : public gameNode
{
private:
	MainMap* mainMap;
	tank* _tank;

public:
	GameScene();
	~GameScene();

	HRESULT init();
	void release();
	void update();
	void render();
};
