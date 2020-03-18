#include "stdafx.h"
#include "MainMap.h"

MainMap::MainMap()
{
}

MainMap::~MainMap()
{
}

HRESULT MainMap::init(const char* imageName)
{
	load();

	//����
	weatherImg = IMAGEMANAGER->findImage(imageName);
	ANIMATIONMANAGER->addDefAnimation("sunny", "����", 2, false, true);
	weatherAni = ANIMATIONMANAGER->findAnimation("sunny");
	ANIMATIONMANAGER->start("sunny");

	return S_OK;
}

void MainMap::release()
{
}

void MainMap::update()
{
}

void MainMap::render(HDC hdc)
{
	IMAGEMANAGER->render("map", hdc, 0, 0);
	IMAGEMANAGER->render("subWin", hdc, 960, 0);

	//���� ������ �����ش�.
	weatherImg->aniRender(hdc, 972, 12, weatherAni);

	//������ ������Ʈ�� �ʿ��� �����ش�.
	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			IMAGEMANAGER->frameRender("tileMap", hdc, tiles[i].rc.left, tiles[i].rc.top, tiles[i].terrainFrameX, tiles[i].terrainFrameY);

			if (tiles[i].obj == OBJ_NONE)continue;

			IMAGEMANAGER->frameRender("tileMap", hdc, tiles[i].rc.left, tiles[i].rc.top, tiles[i].objFrameX, tiles[i].objFrameY);
		}
	}

	draw(hdc);

	//���콺�� ���� Ÿ���� �浹�ϸ� �� Ÿ���� �Ӽ��� �����ش�.
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&tiles[i].rc, m_ptMouse))
		{
			IMAGEMANAGER->render("tileCheck", hdc, tiles[i].rc.left, tiles[i].rc.top);
			IMAGEMANAGER->alphaRender("���", hdc, 972, 74, 100);

			if (tiles[i].terrain == TR_PLAIN)
			{
				IMAGEMANAGER->render("����", hdc, 972, 74);
			}
			else if (tiles[i].terrain == TR_CASTLERAND)
			{
				IMAGEMANAGER->render("����", hdc, 972, 74);
			}
			else if (tiles[i].terrain == TR_BARRACKS)
			{
				IMAGEMANAGER->render("����", hdc, 972, 74);
			}
			else if (tiles[i].terrain == TR_BADLANDS)
			{
				IMAGEMANAGER->render("Ȳ����", hdc, 972, 74);
			}
			else if (tiles[i].obj == OBJ_CASTLEGATE)
			{
				IMAGEMANAGER->render("����", hdc, 972, 74);
			}
			else if (tiles[i].obj == OBJ_CASTLEWALLS)
			{
				IMAGEMANAGER->render("����", hdc, 972, 74);
			}
			else if (tiles[i].obj == OBJ_MOUNTAIN)
			{
				IMAGEMANAGER->render("����", hdc, 972, 74);
			}
			else if (tiles[i].obj == OBJ_ROCKMOUNTAIN)
			{
				IMAGEMANAGER->render("������", hdc, 972, 74);
			}
		}
	}
}

void MainMap::load()
{
	if (isOfnCheck)
	{
		file = CreateFile(fileName[2], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else
	{
		file = CreateFile("save/SaveMap_3.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	ReadFile(file, tiles, sizeof(tagTile) * TILE_X * TILE_Y, &read, NULL);
	CloseHandle(file);
}

void MainMap::draw(HDC hdc)
{
	//ĳ������ �̵� ������ ���� ������ �����ش�.
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (tiles[i].flood)
		{
			if (PLAYERMANAGER->getJojo()->getIsSelect())
			{
				//�̵�����
				PLAYERMANAGER->getJojo()->getPlayerVector()[0].moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//���ݹ���
				PLAYERMANAGER->getJojo()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[0].left, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[0].top);
				if (PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[1].right <= WINSIZEY)
					PLAYERMANAGER->getJojo()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[1].left, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[1].top);
				PLAYERMANAGER->getJojo()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[2].left, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[2].top);
				PLAYERMANAGER->getJojo()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[3].left, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[3].top);
			}

			if (PLAYERMANAGER->getHahudon()->getIsSelect())
			{
				//�̵�����
				PLAYERMANAGER->getHahudon()->getPlayerVector()[0].moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//���ݹ���
				PLAYERMANAGER->getHahudon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc.left - 48, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc.top);
				PLAYERMANAGER->getHahudon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc.left + 48, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc.top);
				PLAYERMANAGER->getHahudon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc.left, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc.top - 48);
				PLAYERMANAGER->getHahudon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc.left, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc.top + 48);
			}
		}
	}

	//���� ��ư�� Ŭ���ϸ� ���ݹ����� �����ش�.
	if (PLAYERMANAGER->getJojo()->getIsAtkRng())
	{
		PLAYERMANAGER->getJojo()->getPlayerVector()[0].atkRngImg->render(hdc, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[0].left, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[0].top);
		if (PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[5].right <= WINSIZEY)
			PLAYERMANAGER->getJojo()->getPlayerVector()[0].atkRngImg->render(hdc, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[1].left, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[1].top);
		PLAYERMANAGER->getJojo()->getPlayerVector()[0].atkRngImg->render(hdc, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[2].left, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[2].top);
		PLAYERMANAGER->getJojo()->getPlayerVector()[0].atkRngImg->render(hdc, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[3].left, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[3].top);
	}
}
