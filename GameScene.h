#pragma once
#include "gameNode.h"
#include "testMap.h"

class GameScene : public gameNode
{
private:
	testMap* test;

public:
	GameScene();
	~GameScene();

	HRESULT init();
	void release();
	void update();
	void render();
};
