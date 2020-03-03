#pragma once
#include"image.h"

//����� �̹����� ����ƽ����..
static image * _backBuffer = IMAGEMANAGER->addImage("backBuffer", WINSIZEX, WINSIZEY);

class gameNode
{
private:
	HDC _hdc;
	bool _managerInit;

public:
	gameNode();
	~gameNode();

	virtual HRESULT init();
	virtual HRESULT init(bool managerInit);
	virtual void release();
	virtual void update();
	virtual void render();
	virtual void render(HDC hdc);

	//����� ������
	image* getBackBuffer() { return _backBuffer; }

	//�߰�(�޸� DC)
	HDC getMemDC() { return _backBuffer->getMemDC(); }

	//�߰� HDC���
	HDC getHDC() { return _hdc; }

	virtual LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};
