#include "stdafx.h"
#include "mainGame.h"
#include "MapTool.h"
#include "GameScene.h"

mainGame::mainGame()
{
}

mainGame::~mainGame()
{
}

HRESULT mainGame::init()
{
	gameNode::init(true);

	//게임에서 쓰일 이미지
	images();

	//씬 추가
	SCENEMANAGER->addScene("MapTool", new MapTool);
	SCENEMANAGER->addScene("GameScene", new GameScene);

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

void mainGame::images()
{
	//맵툴 타일 이미지
	IMAGEMANAGER->addFrameImage("tileMap", "images/MapTool/TileMap.bmp", 144, 480, SAMPLE_TILE_X, SAMPLE_TILE_Y, true, RGB(255, 0, 255));

	//맵툴용 이미지
	IMAGEMANAGER->addImage("map", "images/MapTool/Map.bmp", 960, 960, true, RGB(255, 0, 255));					//실제 맵의 이미지
	IMAGEMANAGER->addImage("subMap", "images/MapTool/SubMap.bmp", 240, 960, true, RGB(255, 0, 255));			//맵툴 이미지
	IMAGEMANAGER->addImage("select", "images/MapTool/Select.bmp", 30, 30, true, RGB(255, 0, 255));				//맵툴 선택 이미지

	//인게임 이미지
	IMAGEMANAGER->addImage("subWin", "images/UI/InGame/SubWindow.bmp", 240, 960, true, RGB(255, 0, 255));		//서브 윈도우 이미지
	IMAGEMANAGER->addImage("tileCheck", "images/UI/InGame/TileCheck.bmp", 48, 48, true, RGB(247, 0, 255));		//타일 체크 이미지
	IMAGEMANAGER->addImage("pMoveRange", "images/UI/InGame/pMovingRange.bmp", 48, 48, true, RGB(247, 0, 255));	//플레이어 이동범위
	IMAGEMANAGER->addImage("eMoveRange", "images/UI/InGame/eMovingRange.bmp", 48, 48, true, RGB(247, 0, 255));	//플레이어 외 이동범위 (아군, 적)
	IMAGEMANAGER->addImage("attackRange", "images/UI/InGame/AttackRange.bmp", 48, 48, true, RGB(247, 0, 255));	//이동시의 공격범위
	IMAGEMANAGER->addImage("atkRange", "images/UI/InGame/AtkRange.bmp", 48, 48, true, RGB(247, 0, 255));		//실제 공격범위

	IMAGEMANAGER->addFrameImage("player", "images/Player/아군기병.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
}
