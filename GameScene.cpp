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
	for (int i = 0; i < PLAYERMANAGER->getPlayer().size(); i++)
	{
		PLAYERMANAGER->getPlayer()[i]->setMapMemoryAdress(_mainMap);
	}
	PLAYERMANAGER->getPlayer()[0]->setPosition(_mainMap->getMap()[216].rc);
	PLAYERMANAGER->getPlayer()[1]->setPosition(_mainMap->getMap()[176].rc);
	PLAYERMANAGER->getPlayer()[2]->setPosition(_mainMap->getMap()[196].rc);
	PLAYERMANAGER->getPlayer()[3]->setPosition(_mainMap->getMap()[177].rc);
	PLAYERMANAGER->getPlayer()[4]->setPosition(_mainMap->getMap()[237].rc);
	PLAYERMANAGER->getPlayer()[5]->setPosition(_mainMap->getMap()[236].rc);
	PLAYERMANAGER->getPlayer()[6]->setPosition(_mainMap->getMap()[197].rc);

	//우군
	FRIENDMANAGER->init();
	for (int i = 0; i < FRIENDMANAGER->getFriend().size(); i++)
	{
		FRIENDMANAGER->getFriend()[i]->setMapMemoryAdress(_mainMap);
	}
	FRIENDMANAGER->getFriend()[0]->setPosition(_mainMap->getMap()[117].rc);
	FRIENDMANAGER->getFriend()[1]->setPosition(_mainMap->getMap()[317].rc);
	FRIENDMANAGER->getFriend()[2]->setPosition(_mainMap->getMap()[277].rc);
	FRIENDMANAGER->getFriend()[3]->setPosition(_mainMap->getMap()[199].rc);
	FRIENDMANAGER->getFriend()[4]->setPosition(_mainMap->getMap()[297].rc);
	FRIENDMANAGER->getFriend()[5]->setPosition(_mainMap->getMap()[97].rc);
	FRIENDMANAGER->getFriend()[6]->setPosition(_mainMap->getMap()[137].rc);
	FRIENDMANAGER->getFriend()[7]->setPosition(_mainMap->getMap()[179].rc);
	FRIENDMANAGER->getFriend()[8]->setPosition(_mainMap->getMap()[219].rc);

	//적군
	ENEMYMANAGER->init();
	for (int i = 0; i < ENEMYMANAGER->getEnemy().size(); i++)
	{
		ENEMYMANAGER->getEnemy()[i]->setMapMemoryAdress(_mainMap);
	}
	ENEMYMANAGER->getEnemy()[0]->setPosition(_mainMap->getMap()[204].rc);
	ENEMYMANAGER->getEnemy()[1]->setPosition(_mainMap->getMap()[182].rc);
	ENEMYMANAGER->getEnemy()[2]->setPosition(_mainMap->getMap()[202].rc);
	ENEMYMANAGER->getEnemy()[3]->setPosition(_mainMap->getMap()[184].rc);
	ENEMYMANAGER->getEnemy()[4]->setPosition(_mainMap->getMap()[188].rc);

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

	//auto& playerVector = PLAYERMANAGER->getPlayer();
	//for (auto player = playerVector.begin(); player != playerVector.end(); )
	//{
	//	int hp = (*player)->getCurrentHp();
	//
	//	if (hp == 0)
	//	{
	//		player = playerVector.erase(player);
	//	}
	//	else
	//	{
	//		player++;
	//	}
	//}

	//if (KEYMANAGER->isOnceKeyDown('2'))
	//{
	//	PLAYERMANAGER->getPlayer().erase(PLAYERMANAGER->getPlayer().begin());
	//}
}

void GameScene::render()
{
	_mainMap->render(getMemDC());
	PLAYERMANAGER->render(getMemDC());
	FRIENDMANAGER->render(getMemDC());
	ENEMYMANAGER->render(getMemDC());
	_interface->render(getMemDC());
}
