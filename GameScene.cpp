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
	_tank->init("pMoveRange","player");
	_tank->setTestMapMemoryAdress(mainMap);
	_tank->setTankPosition(mainMap->getMap()[193].rc);

	return S_OK;
}

void GameScene::release()
{
	SAFE_DELETE(mainMap);
	SAFE_DELETE(_tank);
}

void GameScene::update()
{
	mainMap->update();
	_tank->update();

	ANIMATIONMANAGER->update();
}

void GameScene::render()
{
	mainMap->render();
	_tank->render();
}
