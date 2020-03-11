#include "stdafx.h"
#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

HRESULT GameScene::init()
{
	mainMap = new MainMap;
	mainMap->init();

	//ÅÊÅ©
	_tank = new tank;
	_tank->init("player");
	_tank->setTestMapMemoryAdress(mainMap);
	_tank->setTankPosition(mainMap->getMap()[199].rc);

	return S_OK;
}

void GameScene::release()
{
	SAFE_DELETE(mainMap);
	SAFE_DELETE(_tank);
}

void GameScene::update()
{
	_tank->update();
	mainMap->update();

	ANIMATIONMANAGER->update();
}

void GameScene::render()
{
	mainMap->render();
	_tank->render();
}
