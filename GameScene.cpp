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
