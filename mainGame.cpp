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

	//���ӿ��� ���� �̹���
	images();

	//�� �߰�
	SCENEMANAGER->addScene("MapTool", new MapTool);
	SCENEMANAGER->addScene("GameScene", new GameScene);

	//���� ��
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
	//��� ��Ʈ��
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//========================================================

	SCENEMANAGER->render();

	TIMEMANAGER->render(getMemDC());

	//========================================================
	//������� ������ HDC�� �׸���.(�ǵ帮������.)
	this->getBackBuffer()->render(getHDC(), 0, 0);
}

void mainGame::images()
{
	//���� Ÿ�� �̹���
	IMAGEMANAGER->addFrameImage("tileMap", "images/MapTool/TileMap.bmp", 144, 480, SAMPLE_TILE_X, SAMPLE_TILE_Y, true, RGB(255, 0, 255));

	//������ �̹���
	IMAGEMANAGER->addImage("map", "images/MapTool/Map.bmp", 960, 960, true, RGB(255, 0, 255));					//���� ���� �̹���
	IMAGEMANAGER->addImage("subMap", "images/MapTool/SubMap.bmp", 240, 960, true, RGB(255, 0, 255));			//���� �̹���
	IMAGEMANAGER->addImage("select", "images/MapTool/Select.bmp", 30, 30, true, RGB(255, 0, 255));				//���� ���� �̹���

	//�� �Ӽ� �̹���
	IMAGEMANAGER->addImage("���", "images/UI/Attribute/���.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("������", "images/UI/Attribute/������.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����", "images/UI/Attribute/����.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����", "images/UI/Attribute/����.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����", "images/UI/Attribute/����.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����", "images/UI/Attribute/����.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����", "images/UI/Attribute/����.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����", "images/UI/Attribute/����.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("Ȳ����", "images/UI/Attribute/Ȳ����.bmp", 144, 96, true, RGB(247, 0, 255));

	//�ΰ��� �̹���
	IMAGEMANAGER->addImage("subWin", "images/UI/InGame/SubWindow.bmp", 240, 960, true, RGB(247, 0, 255));		//���� ������ �̹���
	IMAGEMANAGER->addImage("tileCheck", "images/UI/InGame/TileCheck.bmp", 48, 48, true, RGB(247, 0, 255));		//Ÿ�� üũ �̹���
	IMAGEMANAGER->addImage("pMoveRange", "images/UI/InGame/pMovingRange.bmp", 48, 48, true, RGB(247, 0, 255));	//�÷��̾� �̵�����
	IMAGEMANAGER->addImage("eMoveRange", "images/UI/InGame/eMovingRange.bmp", 48, 48, true, RGB(247, 0, 255));	//�÷��̾� �� �̵����� (�Ʊ�, ��)
	IMAGEMANAGER->addImage("attackRange", "images/UI/InGame/AttackRange.bmp", 48, 48, true, RGB(247, 0, 255));	//�̵����� ���ݹ���
	IMAGEMANAGER->addImage("atkRange", "images/UI/InGame/AtkRange.bmp", 48, 48, true, RGB(247, 0, 255));		//���� ���ݹ���

	//���� �̹���
	IMAGEMANAGER->addFrameImage("����", "images/UI/Weather/Sunny.bmp", 216, 200, 1, 4, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("�帲", "images/UI/Weather/Blur.bmp", 216, 200, 1, 4, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("��", "images/UI/Weather/Rain.bmp", 216, 200, 1, 4, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����", "images/UI/Weather/HeavyRain.bmp", 216, 200, 1, 4, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("��", "images/UI/Weather/Snow.bmp", 216, 200, 1, 4, true, RGB(247, 0, 255));

	//����â �̹���
	IMAGEMANAGER->addImage("����â", "images/UI/Info/Status.bmp", 240, 305, true, RGB(247, 0, 255));

	//�÷��̾� �̹���
	IMAGEMANAGER->addFrameImage("�޴�", "images/UI/InGame/Menu.bmp", 176, 242, 2, 2, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����", "images/Player/����.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����ATK", "images/Player/����ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����BH", "images/Player/����BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("���ĵ�", "images/Player/���ĵ�.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("���ĵ�ATK", "images/Player/���ĵ�ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("���ĵ�BH", "images/Player/���ĵ�BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("���Ŀ�", "images/Player/���Ŀ�.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("���Ŀ�ATK", "images/Player/���Ŀ�ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("���Ŀ�BH", "images/Player/���Ŀ�BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����", "images/Player/����.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����ATK", "images/Player/����ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����BH", "images/Player/����BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("��ȫ", "images/Player/��ȫ.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("��ȫATK", "images/Player/��ȫATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("��ȫBH", "images/Player/��ȫBH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����", "images/Player/����.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����ATK", "images/Player/����ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����BH", "images/Player/����BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����", "images/Player/����.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����ATK", "images/Player/����ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����BH", "images/Player/����BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));

	//�� �̹���
	IMAGEMANAGER->addFrameImage("����", "images/Enemy/����.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����ATK", "images/Enemy/����ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����BH", "images/Enemy/����BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));

	//�� �̹���
	IMAGEMANAGER->addImage("����Face", "images/UI/Face/����face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����Face", "images/UI/Face/����face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("��ȫFace", "images/UI/Face/��ȫface.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����Face", "images/UI/Face/����face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����Face", "images/UI/Face/����face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("���ĵ�Face", "images/UI/Face/���ĵ�face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("���Ŀ�Face", "images/UI/Face/���Ŀ�face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����Face", "images/UI/Face/����face.bmp", 64, 80, true, RGB(247, 0, 255));
}
