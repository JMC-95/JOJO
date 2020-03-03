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
	RECT _btnTerrainDraw;		//지형
	RECT _btnObjectDraw;		//오브젝트
	RECT _btnEraser;			//지우개
	RECT _btnData;				//저장 데이터
	RECT _btnSave;				//세이브
	RECT _btnLoad;				//로드
	RECT _btnExit;				//메인메뉴로 돌아가기

	HANDLE file;
	DWORD write;
	DWORD read;

	tagTile _tiles[TILEX * TILEY];
	tagSampleTile _sampleTiles[SAMPLETILEX * SAMPLETILEY];
	tagCurrentTile _currentTile;

private:
	int _ctrSelect;				//버튼 선택

	//세이브 파일
	const char* fileName[5] = { "saveMap1.map","saveMap2.map","saveMap3.map","saveMap4.map","saveMap5.map" };

public:
	MapTool();
	~MapTool();

	HRESULT init();				//초기화
	void release();				//해제
	void update();				//연산하는곳
	void render();				//그리는곳

	void MapToolSetup();		//맵툴 초기화 (init 부분)
	void MapButton();			//맵에서 사용할 버튼
	void SetMap();				//맵툴을 클릭해서 실제 맵에 그려준다.
	void Save();				//세이브
	void Load();				//로드

	TERRAIN terrainSelect(int frameX, int frameY);
	OBJECT objSelect(int frameX, int frameY);

	virtual LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};
