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
	mainMap->init("¸¼À½");

	PLAYERMANAGER->init();
	PLAYERMANAGER->getJojo()->setMapMemoryAdress(mainMap);
	PLAYERMANAGER->getJojo()->setPosition(mainMap->getMap()[197].rc);
	PLAYERMANAGER->getHahudon()->setMapMemoryAdress(mainMap);
	PLAYERMANAGER->getHahudon()->setPosition(mainMap->getMap()[176].rc);
	PLAYERMANAGER->getHahuyeon()->setMapMemoryAdress(mainMap);
	PLAYERMANAGER->getHahuyeon()->setPosition(mainMap->getMap()[196].rc);
	PLAYERMANAGER->getIjeon()->setMapMemoryAdress(mainMap);
	PLAYERMANAGER->getIjeon()->setPosition(mainMap->getMap()[177].rc);
	PLAYERMANAGER->getJohong()->setMapMemoryAdress(mainMap);
	PLAYERMANAGER->getJohong()->setPosition(mainMap->getMap()[237].rc);
	PLAYERMANAGER->getJoin()->setMapMemoryAdress(mainMap);
	PLAYERMANAGER->getJoin()->setPosition(mainMap->getMap()[236].rc);
	PLAYERMANAGER->getAgjin()->setMapMemoryAdress(mainMap);
	PLAYERMANAGER->getAgjin()->setPosition(mainMap->getMap()[216].rc);

	return S_OK;
}

void GameScene::release()
{
	SAFE_DELETE(mainMap);
}

void GameScene::update()
{
	mainMap->update();
	PLAYERMANAGER->update();
	ANIMATIONMANAGER->update();
}

void GameScene::render()
{
	mainMap->render(getMemDC());
	PLAYERMANAGER->render(getMemDC());
}
