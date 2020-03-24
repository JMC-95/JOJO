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
	FRIENDMANAGER->init();
	FRIENDMANAGER->getDogyeom()->setMapMemoryAdress(_mainMap);
	FRIENDMANAGER->getDogyeom()->setPosition(_mainMap->getMap()[117].rc);
	FRIENDMANAGER->getGwanu()->setMapMemoryAdress(_mainMap);
	FRIENDMANAGER->getGwanu()->setPosition(_mainMap->getMap()[317].rc);
	FRIENDMANAGER->getJangbi()->setMapMemoryAdress(_mainMap);
	FRIENDMANAGER->getJangbi()->setPosition(_mainMap->getMap()[277].rc);
	//FRIENDMANAGER->getSoldier()->setMapMemoryAdress(_mainMap);
	//FRIENDMANAGER->getSoldier()->setPosition(_mainMap->getMap()[97].rc);
	FRIENDMANAGER->getFriend()[0]->setMapMemoryAdress(_mainMap);
	FRIENDMANAGER->getFriend()[0]->setPosition(_mainMap->getMap()[97].rc);
	FRIENDMANAGER->getFriend()[1]->setMapMemoryAdress(_mainMap);
	FRIENDMANAGER->getFriend()[1]->setPosition(_mainMap->getMap()[137].rc);
	FRIENDMANAGER->getFriend()[2]->setMapMemoryAdress(_mainMap);
	FRIENDMANAGER->getFriend()[2]->setPosition(_mainMap->getMap()[179].rc);
	FRIENDMANAGER->getFriend()[3]->setMapMemoryAdress(_mainMap);
	FRIENDMANAGER->getFriend()[3]->setPosition(_mainMap->getMap()[219].rc);
	FRIENDMANAGER->getWonso()->setMapMemoryAdress(_mainMap);
	FRIENDMANAGER->getWonso()->setPosition(_mainMap->getMap()[199].rc);
	FRIENDMANAGER->getYubi()->setMapMemoryAdress(_mainMap);
	FRIENDMANAGER->getYubi()->setPosition(_mainMap->getMap()[297].rc);

	//적군
	ENEMYMANAGER->init();
	ENEMYMANAGER->getBeonju()->setMapMemoryAdress(_mainMap);
	ENEMYMANAGER->getBeonju()->setPosition(_mainMap->getMap()[204].rc);
	ENEMYMANAGER->getDongtak()->setMapMemoryAdress(_mainMap);
	ENEMYMANAGER->getDongtak()->setPosition(_mainMap->getMap()[182].rc);
	ENEMYMANAGER->getIyu()->setMapMemoryAdress(_mainMap);
	ENEMYMANAGER->getIyu()->setPosition(_mainMap->getMap()[202].rc);
	ENEMYMANAGER->getJangje()->setMapMemoryAdress(_mainMap);
	ENEMYMANAGER->getJangje()->setPosition(_mainMap->getMap()[184].rc);
	ENEMYMANAGER->getYeopo()->setMapMemoryAdress(_mainMap);
	ENEMYMANAGER->getYeopo()->setPosition(_mainMap->getMap()[188].rc);

	COLLISIONMANAGER->init();

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
	FRIENDMANAGER->update();
	ENEMYMANAGER->update();

	ANIMATIONMANAGER->update();
	COLLISIONMANAGER->update();
}

void GameScene::render()
{
	_mainMap->render(getMemDC());
	PLAYERMANAGER->render(getMemDC());
	FRIENDMANAGER->render(getMemDC());
	ENEMYMANAGER->render(getMemDC());
	_interface->render(getMemDC());
}
