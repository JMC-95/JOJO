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
			//����
			if (PLAYERMANAGER->getAgjin()->getIsSelect())
			{
				//�̵�����
				PLAYERMANAGER->getAgjin()->getPlayerVector()[0].moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//���ݹ���
				PLAYERMANAGER->getAgjin()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[0].left, PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[0].top);
				PLAYERMANAGER->getAgjin()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[1].left, PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[1].top);
				PLAYERMANAGER->getAgjin()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[2].left, PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[2].top);
				if (PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[3].right <= WINSIZEY)
				{
					PLAYERMANAGER->getAgjin()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[3].left, PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[3].top);
					PLAYERMANAGER->getAgjin()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[4].left, PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[4].top);
					PLAYERMANAGER->getAgjin()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[5].left, PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[5].top);
				}
				PLAYERMANAGER->getAgjin()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[6].left, PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[6].top);
				PLAYERMANAGER->getAgjin()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[7].left, PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[7].top);
			}

			//���ĵ�
			if (PLAYERMANAGER->getHahudon()->getIsSelect())
			{
				//�̵�����
				PLAYERMANAGER->getHahudon()->getPlayerVector()[0].moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//���ݹ���
				PLAYERMANAGER->getHahudon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rcAtk[0].left, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rcAtk[0].top);
				if (PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rcAtk[1].right <= WINSIZEY)
					PLAYERMANAGER->getHahudon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rcAtk[1].left, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rcAtk[1].top);
				PLAYERMANAGER->getHahudon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rcAtk[2].left, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rcAtk[2].top);
				PLAYERMANAGER->getHahudon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rcAtk[3].left, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rcAtk[3].top);
			}

			//���Ŀ�
			if (PLAYERMANAGER->getHahuyeon()->getIsSelect())
			{
				//�̵�����
				PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//���ݹ���
				PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rcAtk[0].left, PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rcAtk[0].top);
				if (PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rcAtk[1].right <= WINSIZEY)
					PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rcAtk[1].left, PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rcAtk[1].top);
				PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rcAtk[2].left, PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rcAtk[2].top);
				PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rcAtk[3].left, PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rcAtk[3].top);
			}

			//����
			if (PLAYERMANAGER->getIjeon()->getIsSelect())
			{
				//�̵�����
				PLAYERMANAGER->getIjeon()->getPlayerVector()[0].moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//���ݹ���
				PLAYERMANAGER->getIjeon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[0].left, PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[0].top);
				PLAYERMANAGER->getIjeon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[1].left, PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[1].top);
				PLAYERMANAGER->getIjeon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[2].left, PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[2].top);
				if (PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[3].right <= WINSIZEY)
				{
					PLAYERMANAGER->getIjeon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[3].left, PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[3].top);
					PLAYERMANAGER->getIjeon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[4].left, PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[4].top);
					PLAYERMANAGER->getIjeon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[5].left, PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[5].top);
				}
				PLAYERMANAGER->getIjeon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[6].left, PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[6].top);
				PLAYERMANAGER->getIjeon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[7].left, PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[7].top);
			}

			//��ȫ
			if (PLAYERMANAGER->getJohong()->getIsSelect())
			{
				//�̵�����
				PLAYERMANAGER->getJohong()->getPlayerVector()[0].moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//���ݹ���
				PLAYERMANAGER->getJohong()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[0].left, PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[0].top);
				PLAYERMANAGER->getJohong()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[1].left, PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[1].top);
				PLAYERMANAGER->getJohong()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[2].left, PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[2].top);
				if (PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[3].right <= WINSIZEY)
				{
					PLAYERMANAGER->getJohong()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[3].left, PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[3].top);
					PLAYERMANAGER->getJohong()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[4].left, PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[4].top);
					PLAYERMANAGER->getJohong()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[5].left, PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[5].top);
				}
				PLAYERMANAGER->getJohong()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[6].left, PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[6].top);
				PLAYERMANAGER->getJohong()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[7].left, PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[7].top);
			}

			//����
			if (PLAYERMANAGER->getJoin()->getIsSelect())
			{
				//�̵�����
				PLAYERMANAGER->getJoin()->getPlayerVector()[0].moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//���ݹ���
				PLAYERMANAGER->getJoin()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getJoin()->getPlayerVector()[0].rcAtk[0].left, PLAYERMANAGER->getJoin()->getPlayerVector()[0].rcAtk[0].top);
				if (PLAYERMANAGER->getJoin()->getPlayerVector()[0].rcAtk[1].right <= WINSIZEY)
					PLAYERMANAGER->getJoin()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getJoin()->getPlayerVector()[0].rcAtk[1].left, PLAYERMANAGER->getJoin()->getPlayerVector()[0].rcAtk[1].top);
				PLAYERMANAGER->getJoin()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getJoin()->getPlayerVector()[0].rcAtk[2].left, PLAYERMANAGER->getJoin()->getPlayerVector()[0].rcAtk[2].top);
				PLAYERMANAGER->getJoin()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getJoin()->getPlayerVector()[0].rcAtk[3].left, PLAYERMANAGER->getJoin()->getPlayerVector()[0].rcAtk[3].top);
			}

			//����
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
