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
	test = new testMap;
	test->init();

	return S_OK;
}

void GameScene::release()
{
	SAFE_DELETE(test);
}

void GameScene::update()
{
	test->update();
}

void GameScene::render()
{
	test->render();
}
