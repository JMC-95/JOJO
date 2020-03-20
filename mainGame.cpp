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

	//맵 속성 이미지
	IMAGEMANAGER->addImage("배경", "images/UI/Attribute/배경.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("바위산", "images/UI/Attribute/바위산.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("병영", "images/UI/Attribute/병영.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("산지", "images/UI/Attribute/산지.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("성내", "images/UI/Attribute/성내.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("성문", "images/UI/Attribute/성문.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("성벽", "images/UI/Attribute/성벽.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("평지", "images/UI/Attribute/평지.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("황무지", "images/UI/Attribute/황무지.bmp", 144, 96, true, RGB(247, 0, 255));

	//인게임 이미지
	IMAGEMANAGER->addImage("subWin", "images/UI/InGame/SubWindow.bmp", 240, 960, true, RGB(247, 0, 255));		//서브 윈도우 이미지
	IMAGEMANAGER->addImage("tileCheck", "images/UI/InGame/TileCheck.bmp", 48, 48, true, RGB(247, 0, 255));		//타일 체크 이미지
	IMAGEMANAGER->addImage("pMoveRange", "images/UI/InGame/pMovingRange.bmp", 48, 48, true, RGB(247, 0, 255));	//플레이어 이동범위
	IMAGEMANAGER->addImage("eMoveRange", "images/UI/InGame/eMovingRange.bmp", 48, 48, true, RGB(247, 0, 255));	//플레이어 외 이동범위 (아군, 적)
	IMAGEMANAGER->addImage("attackRange", "images/UI/InGame/AttackRange.bmp", 48, 48, true, RGB(247, 0, 255));	//이동시의 공격범위
	IMAGEMANAGER->addImage("atkRange", "images/UI/InGame/AtkRange.bmp", 48, 48, true, RGB(247, 0, 255));		//실제 공격범위

	//날씨 이미지
	IMAGEMANAGER->addFrameImage("맑음", "images/UI/Weather/Sunny.bmp", 216, 200, 1, 4, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("흐림", "images/UI/Weather/Blur.bmp", 216, 200, 1, 4, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("비", "images/UI/Weather/Rain.bmp", 216, 200, 1, 4, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("폭우", "images/UI/Weather/HeavyRain.bmp", 216, 200, 1, 4, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("눈", "images/UI/Weather/Snow.bmp", 216, 200, 1, 4, true, RGB(247, 0, 255));

	//정보창 이미지
	IMAGEMANAGER->addImage("정보창", "images/UI/Info/Status.bmp", 240, 305, true, RGB(247, 0, 255));

	//플레이어 이미지
	IMAGEMANAGER->addFrameImage("메뉴", "images/UI/InGame/Menu.bmp", 176, 242, 2, 2, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("조조", "images/Player/조조.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("조조ATK", "images/Player/조조ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("조조BH", "images/Player/조조BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("하후돈", "images/Player/하후돈.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("하후돈ATK", "images/Player/하후돈ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("하후돈BH", "images/Player/하후돈BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("하후연", "images/Player/하후연.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("하후연ATK", "images/Player/하후연ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("하후연BH", "images/Player/하후연BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("조인", "images/Player/조인.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("조인ATK", "images/Player/조인ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("조인BH", "images/Player/조인BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("조홍", "images/Player/조홍.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("조홍ATK", "images/Player/조홍ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("조홍BH", "images/Player/조홍BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("악진", "images/Player/악진.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("악진ATK", "images/Player/악진ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("악진BH", "images/Player/악진BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("이전", "images/Player/이전.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("이전ATK", "images/Player/이전ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("이전BH", "images/Player/이전BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));

	//적 이미지
	IMAGEMANAGER->addFrameImage("여포", "images/Enemy/여포.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("여포ATK", "images/Enemy/여포ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("여포BH", "images/Enemy/여포BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));

	//얼굴 이미지
	IMAGEMANAGER->addImage("조조Face", "images/UI/Face/조조face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("조인Face", "images/UI/Face/조인face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("조홍Face", "images/UI/Face/조홍face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("악진Face", "images/UI/Face/악진face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("이전Face", "images/UI/Face/이전face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("하후돈Face", "images/UI/Face/하후돈face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("하후연Face", "images/UI/Face/하후연face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("여포Face", "images/UI/Face/여포face.bmp", 64, 80, true, RGB(247, 0, 255));
}
