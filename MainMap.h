#pragma once
#include "gameNode.h"
#include "MapTiles.h"

class MainMap : public gameNode
{
private:

public:
	MainMap();
	~MainMap();

	HRESULT init();
	void release();
	void update();
	void render();

	void Load();
};
