#pragma once
#include "gameNode.h"
#include "MainMap.h"
#include "Interface.h"
#include "progressBar.h"

class GameScene : public gameNode
{
private:
	MainMap* _mainMap;
	Interface* _interface;

public:
	GameScene();
	~GameScene();

	HRESULT init();
	void release();
	void update();
	void render();
};
