#pragma once
#include "gameNode.h"
#include "MapTiles.h"

//�������� ����ϴ� enum��
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
	RECT btnTerrainDraw;		//����
	RECT btnObjectDraw;			//������Ʈ
	RECT btnEraser;				//���찳
	RECT btnData;				//���� ������
	RECT btnSave;				//���̺�
	RECT btnLoad;				//�ε�
	RECT btnExit;				//���θ޴��� ���ư���

	HANDLE file;
	DWORD write;
	DWORD read;

	tagTile tiles[TILE_X * TILE_Y];
	tagSampleTile sampleTiles[SAMPLE_TILE_X * SAMPLE_TILE_Y];
	tagCurrentTile currentTile;

private:
	int ctrSelect;				//��ư ����

	//���̺� ����
	const char* fileName[5] = { "SaveMap_1.map","SaveMap_2.map","SaveMap_3.map","SaveMap_4.map","SaveMap_5.map" };

public:
	MapTool();
	~MapTool();

	HRESULT init();				//�ʱ�ȭ
	void release();				//����
	void update();				//�����ϴ°�
	void render();				//�׸��°�

	void mapToolSetup();		//���� �ʱ�ȭ (init �κ�)
	void mapToolButton();		//�������� ����� ��ư
	void setMap();				//������ Ŭ���ؼ� ���� �ʿ� �׷��ش�.
	void save();				//���̺�
	void load();				//�ε�

	TERRAIN terrainSelect(int frameX, int frameY);
	OBJECT objSelect(int frameX, int frameY);

	virtual LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};
