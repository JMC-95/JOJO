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
	//mainMap = new MainMap;
	//mainMap->init();

	_test = new testMap;
	_test->init();

	//ÅÊÅ©
	_tank = new tank;
	_tank->init();
	_tank->setTestMapMemoryAdress(_test);
	_tank->setTankPosition(_test->getMap()[199].rc);

	return S_OK;
}

void GameScene::release()
{
	//SAFE_DELETE(mainMap);
	SAFE_DELETE(_test);
	SAFE_DELETE(_tank);
}

void GameScene::update()
{
	//mainMap->update();
	_tank->update();
	_test->update();

	ANIMATIONMANAGER->update();
}

void GameScene::render()
{
	//mainMap->render();
	_test->render();
	_tank->render();
}
