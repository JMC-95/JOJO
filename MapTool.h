#pragma once
#include "gameNode.h"
#include "MapTiles.h"

//맵툴에서 사용하는 enum문
enum CTRL
{
	CTRL_TERRAIN,
	CTRL_OBJECT,
	CTRL_ERASER,
	CTRL_DATA,
	CTRL_SAVE,
	CTRL_LOAD,
	CTRL_END
};

class MapTool : public gameNode
{
private:
	RECT btnTerrainDraw;		//지형
	RECT btnObjectDraw;			//오브젝트
	RECT btnEraser;				//지우개
	RECT btnData;				//저장 데이터
	RECT btnSave;				//세이브
	RECT btnLoad;				//로드
	RECT btnExit;				//메인메뉴로 돌아가기

	HANDLE file;
	DWORD write;
	DWORD read;

	tagTile tiles[TILE_X * TILE_Y];
	tagSampleTile sampleTiles[SAMPLE_TILE_X * SAMPLE_TILE_Y];
	tagCurrentTile currentTile;

private:
	int ctrSelect;				//버튼 선택

	//세이브 파일
	const char* fileName[5] = { "SaveMap_1.map","SaveMap_2.map","SaveMap_3.map","SaveMap_4.map","SaveMap_5.map" };

public:
	MapTool();
	~MapTool();

	HRESULT init();				//초기화
	void release();				//해제
	void update();				//연산하는곳
	void render();				//그리는곳

	void mapToolSetup();		//맵툴 초기화 (init 부분)
	void mapToolButton();		//맵툴에서 사용할 버튼
	void setMap();				//맵툴을 클릭해서 실제 맵에 그려준다.
	void save();				//세이브
	void load();				//로드

	TERRAIN terrainSelect(int frameX, int frameY);
	OBJECT objSelect(int frameX, int frameY);

	virtual LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};
