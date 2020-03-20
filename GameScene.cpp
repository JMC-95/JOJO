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
	_mainMap = new MainMap;
	_mainMap->init("맑음");

	_interface = new Interface;

	//아군
	PLAYERMANAGER->init();
	PLAYERMANAGER->getAgjin()->setMapMemoryAdress(_mainMap);
	PLAYERMANAGER->getAgjin()->setPosition(_mainMap->getMap()[216].rc);
	PLAYERMANAGER->getHahudon()->setMapMemoryAdress(_mainMap);
	PLAYERMANAGER->getHahudon()->setPosition(_mainMap->getMap()[176].rc);
	PLAYERMANAGER->getHahuyeon()->setMapMemoryAdress(_mainMap);
	PLAYERMANAGER->getHahuyeon()->setPosition(_mainMap->getMap()[196].rc);
	PLAYERMANAGER->getIjeon()->setMapMemoryAdress(_mainMap);
	PLAYERMANAGER->getIjeon()->setPosition(_mainMap->getMap()[177].rc);
	PLAYERMANAGER->getJohong()->setMapMemoryAdress(_mainMap);
	PLAYERMANAGER->getJohong()->setPosition(_mainMap->getMap()[237].rc);
	PLAYERMANAGER->getJoin()->setMapMemoryAdress(_mainMap);
	PLAYERMANAGER->getJoin()->setPosition(_mainMap->getMap()[236].rc);
	PLAYERMANAGER->getJojo()->setMapMemoryAdress(_mainMap);
	PLAYERMANAGER->getJojo()->setPosition(_mainMap->getMap()[197].rc);

	//우군


	//적군
	ENEMYMANAGER->init();
	ENEMYMANAGER->getYeopo()->setMapMemoryAdress(_mainMap);
	ENEMYMANAGER->getYeopo()->setPosition(_mainMap->getMap()[188].rc);

	return S_OK;
}

void GameScene::release()
{
	SAFE_DELETE(_mainMap);
	SAFE_DELETE(_interface);
}

void GameScene::update()
{
	_mainMap->update();
	PLAYERMANAGER->update();
	ENEMYMANAGER->update();

	ANIMATIONMANAGER->update();
}

void GameScene::render()
{
	_mainMap->render(getMemDC());
	PLAYERMANAGER->render(getMemDC());
	ENEMYMANAGER->render(getMemDC());
	_interface->render(getMemDC());
}
