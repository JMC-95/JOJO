#pragma once
#include "MapTiles.h"

class MainMap
{
private:
	//맵 로드용
	HANDLE file;
	DWORD read;
	DWORD attribute[TILE_X * TILE_Y];

	//타일
	tagTile tiles[TILE_X * TILE_Y];

private:
	image* weatherImg;		//날씨 이미지
	animation* weatherAni;	//날씨 애니메이션

	//세이브 파일
	const char* fileName[5] = { "SaveMap_1.map","SaveMap_2.map","SaveMap_3.map","SaveMap_4.map","SaveMap_5.map" };

public:
	MainMap();
	~MainMap();

	HRESULT init(const char* imageName);
	void release();
	void update();
	void render(HDC hdc);

	void load();

	DWORD* getAttribute() { return attribute; }

	tagTile* getMap() { return tiles; }
};
