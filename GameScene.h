#pragma once
#include "gameNode.h"
//#include "MainMap.h"
#include "testMap.h"
#include "tank.h"

class GameScene : public gameNode
{
private:
	//MainMap* mainMap;
	testMap* _test;
	tank* _tank;

public:
	GameScene();
	~GameScene();

	HRESULT init();
	void release();
	void update();
	void render();
};
