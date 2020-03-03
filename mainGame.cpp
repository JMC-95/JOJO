#include "stdafx.h"
#include "mainGame.h"
#include "MapTool.h"

mainGame::mainGame()
{
}

mainGame::~mainGame()
{
}

HRESULT mainGame::init()
{
	gameNode::init(true);

	//씬 추가
	SCENEMANAGER->addScene("MapTool", new MapTool);

	//현재 씬
	SCENEMANAGER->changeScene("MapTool");

	return S_OK;
}

void mainGame::release()
{
	gameNode::release();
}

void mainGame::update()
{
	gameNode::update();

	SCENEMANAGER->update();
}

void mainGame::render(/*HDC hdc*/)
{
	//흰색 비트맵
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//========================================================

	SCENEMANAGER->render();

	TIMEMANAGER->render(getMemDC());

	//========================================================
	//백버퍼의 내용을 HDC에 그린다.(건드리지말것.)
	this->getBackBuffer()->render(getHDC(), 0, 0);
}
