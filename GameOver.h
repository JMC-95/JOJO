#pragma once
#include "gameNode.h"

class GameOver : public gameNode
{
private:
	bool isGameClear;
	bool isGameOver;

public:
	GameOver();
	~GameOver();

	HRESULT init();
	void release();
	void update();
	void render();
};
