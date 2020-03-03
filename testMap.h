#pragma once
#include "gameNode.h"
#include "MapTiles.h"

class testMap : public gameNode
{
private:
	HANDLE file;
	DWORD read;
	DWORD _attribute[TILEX * TILEY];

	tagTile _tiles[TILEX * TILEY];
	tagSampleTile _sampleTiles[SAMPLETILEX * SAMPLETILEY];
	tagCurrentTile _currentTile;

private:
	int _pos[2];

	//세이브 파일
	const char* fileName[5] = { "saveMap1.map","saveMap2.map","saveMap3.map","saveMap4.map","saveMap5.map" };

public:
	testMap();
	~testMap();

	HRESULT init();
	void release();
	void update();
	void render();

	void Load();

	DWORD* getAttribute() { return _attribute; }

	tagTile* getMap() { return _tiles; }

	int getPosFirst() { return _pos[0]; }
	int getPosSecond() { return _pos[1]; }
};
