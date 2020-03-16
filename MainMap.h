#pragma once
#include "MapTiles.h"

class MainMap
{
private:
	//�� �ε��
	HANDLE file;
	DWORD read;
	DWORD attribute[TILE_X * TILE_Y];

	//Ÿ��
	tagTile tiles[TILE_X * TILE_Y];

	//���̺� ����
	const char* fileName[5] = { "SaveMap_1.map","SaveMap_2.map","SaveMap_3.map","SaveMap_4.map","SaveMap_5.map" };

public:
	MainMap();
	~MainMap();

	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	void load();

	DWORD* getAttribute() { return attribute; }

	tagTile* getMap() { return tiles; }
};
