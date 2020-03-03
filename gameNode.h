#pragma once
#include"image.h"

//백버퍼 이미지를 스태틱으로..
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

	//백버퍼 얻어오기
	image* getBackBuffer() { return _backBuffer; }

	//추가(메모리 DC)
	HDC getMemDC() { return _backBuffer->getMemDC(); }

	//추가 HDC얻기
	HDC getHDC() { return _hdc; }

	virtual LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};
