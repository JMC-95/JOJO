#pragma once
#include "gameNode.h"
#include "MapTiles.h"

class testMap : public gameNode
{
private:
	HANDLE file;
	DWORD read;
	DWORD attribute[TILE_X * TILE_Y];

	tagTile tiles[TILE_X * TILE_Y];

private:
	//세이브 파일
	const char* fileName[5] = { "SaveMap_1.map","SaveMap_2.map","SaveMap_3.map","SaveMap_4.map","SaveMap_5.map" };

public:
	testMap();
	~testMap();

	HRESULT init();
	void release();
	void update();
	void render();

	void load();

	DWORD* getAttribute() { return attribute; }

	tagTile* getMap() { return tiles; }
};
