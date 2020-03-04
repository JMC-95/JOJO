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
	RECT _btnTerrainDraw;		//����
	RECT _btnObjectDraw;		//������Ʈ
	RECT _btnEraser;			//���찳
	RECT _btnData;				//���� ������
	RECT _btnSave;				//���̺�
	RECT _btnLoad;				//�ε�
	RECT _btnExit;				//���θ޴��� ���ư���

	HANDLE file;
	DWORD write;
	DWORD read;

	tagTile _tiles[TILEX * TILEY];
	tagSampleTile _sampleTiles[SAMPLETILEX * SAMPLETILEY];
	tagCurrentTile _currentTile;

private:
	int _ctrSelect;				//��ư ����

	//���̺� ����
	const char* fileName[5] = { "SaveMap_1.map","SaveMap_2.map","SaveMap_3.map","SaveMap_4.map","SaveMap_5.map" };

public:
	MapTool();
	~MapTool();

	HRESULT init();				//�ʱ�ȭ
	void release();				//����
	void update();				//�����ϴ°�
	void render();				//�׸��°�

	void MapToolSetup();		//���� �ʱ�ȭ (init �κ�)
	void MapButton();			//�ʿ��� ����� ��ư
	void SetMap();				//������ Ŭ���ؼ� ���� �ʿ� �׷��ش�.
	void Save();				//���̺�
	void Load();				//�ε�

	TERRAIN terrainSelect(int frameX, int frameY);
	OBJECT objSelect(int frameX, int frameY);

	virtual LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};
