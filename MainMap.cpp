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

	playerDraw(hdc);
	friendDraw(hdc);
	enemyDraw(hdc);

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

void MainMap::playerDraw(HDC hdc)
{
	//ĳ������ �̵� ������ ���� ������ �����ش�.
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (tiles[i].flood)
		{
			//����
			if (PLAYERMANAGER->getPlayer()[0]->getIsSelect())
			{
				//�̵�����
				PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//���ݹ���
				PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[0].left, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[0].top);
				PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[1].left, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[1].top);
				PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[2].left, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[2].top);
				if (PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[3].right <= WINSIZEY)
				{
					PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[3].left, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[3].top);
					PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[4].left, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[4].top);
					PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[5].left, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[5].top);
				}
				PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[6].left, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[6].top);
				PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[7].left, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[7].top);
			}

			//���ĵ�
			if (PLAYERMANAGER->getPlayer()[1]->getIsSelect())
			{
				//�̵�����
				PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//���ݹ���
				PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[0].left, PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[0].top);
				if (PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[1].right <= WINSIZEY)
				{
					PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[1].left, PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[1].top);
				}
				PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[2].left, PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[2].top);
				PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[3].left, PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[3].top);
			}

			//���Ŀ�
			if (PLAYERMANAGER->getPlayer()[2]->getIsSelect())
			{
				//�̵�����
				PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//���ݹ���
				PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[0].left, PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[0].top);
				if (PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[1].right <= WINSIZEY)
				{
					PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[1].left, PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[1].top);
				}
				PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[2].left, PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[2].top);
				PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[3].left, PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[3].top);
			}

			//����
			if (PLAYERMANAGER->getPlayer()[3]->getIsSelect())
			{
				//�̵�����
				PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//���ݹ���
				PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[0].left, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[0].top);
				PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[1].left, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[1].top);
				PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[2].left, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[2].top);
				if (PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[3].right <= WINSIZEY)
				{
					PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[3].left, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[3].top);
					PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[4].left, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[4].top);
					PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[5].left, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[5].top);
				}
				PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[6].left, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[6].top);
				PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[7].left, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[7].top);
			}

			//��ȫ
			if (PLAYERMANAGER->getPlayer()[4]->getIsSelect())
			{
				//�̵�����
				PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//���ݹ���
				PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[0].left, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[0].top);
				PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[1].left, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[1].top);
				PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[2].left, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[2].top);
				if (PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[3].right <= WINSIZEY)
				{
					PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[3].left, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[3].top);
					PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[4].left, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[4].top);
					PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[5].left, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[5].top);
				}
				PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[6].left, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[6].top);
				PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[7].left, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[7].top);
			}

			//����
			if (PLAYERMANAGER->getPlayer()[5]->getIsSelect())
			{
				//�̵�����
				PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//���ݹ���
				PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[0].left, PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[0].top);
				if (PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[1].right <= WINSIZEY)
				{
					PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[1].left, PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[1].top);
				}
				PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[2].left, PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[2].top);
				PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[3].left, PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[3].top);
			}

			//����
			if (PLAYERMANAGER->getPlayer()[6]->getIsSelect())
			{
				//�̵�����
				PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//���ݹ���
				PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rcAtk[0].left, PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rcAtk[0].top);
				if (PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rcAtk[1].right <= WINSIZEY)
				{
					PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rcAtk[1].left, PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rcAtk[1].top);
				}
				PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rcAtk[2].left, PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rcAtk[2].top);
				PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rcAtk[3].left, PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rcAtk[3].top);
			}
		}
	}

	//���� ��ư�� Ŭ���ϸ� ���ݹ����� �����ش�.
	//����
	if (PLAYERMANAGER->getPlayer()[0]->getIsAtkRng())
	{
		PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[0].left, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[0].top);
		PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[1].left, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[1].top);
		PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[2].left, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[2].top);
		if (PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[3].right <= WINSIZEY)
		{
			PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[3].left, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[3].top);
			PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[4].left, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[4].top);
			PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[5].left, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[5].top);
		}
		PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[6].left, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[6].top);
		PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[7].left, PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[7].top);
	}

	//���ĵ�
	if (PLAYERMANAGER->getPlayer()[1]->getIsAtkRng())
	{
		PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[0].left, PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[0].top);
		if (PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[1].right <= WINSIZEY)
		{
			PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[1].left, PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[1].top);
		}
		PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[2].left, PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[2].top);
		PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[3].left, PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[3].top);
	}

	//���Ŀ�
	if (PLAYERMANAGER->getPlayer()[2]->getIsAtkRng())
	{
		PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[0].left, PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[0].top);
		if (PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[1].right <= WINSIZEY)
		{
			PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[1].left, PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[1].top);
		}
		PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[2].left, PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[2].top);
		PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[3].left, PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[3].top);
	}

	//����
	if (PLAYERMANAGER->getPlayer()[3]->getIsAtkRng())
	{
		PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[0].left, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[0].top);
		PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[1].left, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[1].top);
		PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[2].left, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[2].top);
		if (PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[3].right <= WINSIZEY)
		{
			PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[3].left, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[3].top);
			PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[4].left, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[4].top);
			PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[5].left, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[5].top);
		}
		PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[6].left, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[6].top);
		PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[7].left, PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[7].top);
	}

	//��ȫ
	if (PLAYERMANAGER->getPlayer()[4]->getIsAtkRng())
	{
		PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[0].left, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[0].top);
		PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[1].left, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[1].top);
		PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[2].left, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[2].top);
		if (PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[3].right <= WINSIZEY)
		{
			PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[3].left, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[3].top);
			PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[4].left, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[4].top);
			PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[5].left, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[5].top);
		}
		PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[6].left, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[6].top);
		PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[7].left, PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[7].top);
	}

	//����
	if (PLAYERMANAGER->getPlayer()[5]->getIsAtkRng())
	{
		PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[0].left, PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[0].top);
		if (PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[1].right <= WINSIZEY)
		{
			PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[1].left, PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[1].top);
		}
		PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[2].left, PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[2].top);
		PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[3].left, PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[3].top);
	}

	if (PLAYERMANAGER->getPlayer()[6]->getIsAtkRng())
	{
		//����
		PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rcAtk[0].left, PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rcAtk[0].top);
		if (PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rcAtk[1].right <= WINSIZEY)
		{
			PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rcAtk[1].left, PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rcAtk[1].top);
		}
		PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rcAtk[2].left, PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rcAtk[2].top);
		PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().atkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rcAtk[3].left, PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rcAtk[3].top);
	}
}

void MainMap::friendDraw(HDC hdc)
{
	//ĳ������ �̵� ������ ���� ������ �����ش�.
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (tiles[i].flood)
		{
			//����
			if (FRIENDMANAGER->getFriend()[0]->getIsSelect())
			{
				//�̵�����
				FRIENDMANAGER->getFriend()[0]->getFriendInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//���ݹ���
				FRIENDMANAGER->getFriend()[0]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[0].left, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[0].top);
				if (FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[1].right <= WINSIZEY)
				{
					FRIENDMANAGER->getFriend()[0]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[1].left, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[1].top);
				}
				FRIENDMANAGER->getFriend()[0]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[2].left, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[2].top);
				FRIENDMANAGER->getFriend()[0]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[3].left, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[3].top);
			}

			//����
			if (FRIENDMANAGER->getFriend()[1]->getIsSelect())
			{
				//�̵�����
				FRIENDMANAGER->getFriend()[1]->getFriendInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//���ݹ���
				FRIENDMANAGER->getFriend()[1]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[0].left, FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[0].top);
				if (FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[1].right <= WINSIZEY)
				{
					FRIENDMANAGER->getFriend()[1]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[1].left, FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[1].top);
				}
				FRIENDMANAGER->getFriend()[1]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[2].left, FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[2].top);
				FRIENDMANAGER->getFriend()[1]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[3].left, FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[3].top);
			}

			//���
			if (FRIENDMANAGER->getFriend()[2]->getIsSelect())
			{
				//�̵�����
				FRIENDMANAGER->getFriend()[2]->getFriendInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//���ݹ���
				FRIENDMANAGER->getFriend()[2]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[0].left, FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[0].top);
				if (FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[1].right <= WINSIZEY)
				{
					FRIENDMANAGER->getFriend()[2]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[1].left, FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[1].top);
				}
				FRIENDMANAGER->getFriend()[2]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[2].left, FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[2].top);
				FRIENDMANAGER->getFriend()[2]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[3].left, FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[3].top);
			}

			//����
			if (FRIENDMANAGER->getFriend()[3]->getIsSelect())
			{
				//�̵�����
				FRIENDMANAGER->getFriend()[3]->getFriendInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//���ݹ���
				FRIENDMANAGER->getFriend()[3]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[0].left, FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[0].top);
				if (FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[1].right <= WINSIZEY)
				{
					FRIENDMANAGER->getFriend()[3]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[1].left, FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[1].top);
				}
				FRIENDMANAGER->getFriend()[3]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[2].left, FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[2].top);
				FRIENDMANAGER->getFriend()[3]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[3].left, FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[3].top);
			}

			//����
			if (FRIENDMANAGER->getFriend()[4]->getIsSelect())
			{
				//�̵�����
				FRIENDMANAGER->getFriend()[4]->getFriendInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//���ݹ���
				FRIENDMANAGER->getFriend()[4]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[0].left, FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[0].top);
				if (FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[1].right <= WINSIZEY)
				{
					FRIENDMANAGER->getFriend()[4]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[1].left, FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[1].top);
				}
				FRIENDMANAGER->getFriend()[4]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[2].left, FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[2].top);
				FRIENDMANAGER->getFriend()[4]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[3].left, FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[3].top);
			}

			//�챺����
			for (int j = 5; j < 9; j++)
			{
				if (FRIENDMANAGER->getFriend()[j]->getIsSelect())
				{
					//�̵�����
					FRIENDMANAGER->getFriend()[j]->getFriendInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

					//���ݹ���
					FRIENDMANAGER->getFriend()[j]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[0].left, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[0].top);
					FRIENDMANAGER->getFriend()[j]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[1].left, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[1].top);
					FRIENDMANAGER->getFriend()[j]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[2].left, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[2].top);
					if (FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[3].right <= WINSIZEY)
					{
						FRIENDMANAGER->getFriend()[j]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[3].left, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[3].top);
						FRIENDMANAGER->getFriend()[j]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[4].left, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[4].top);
						FRIENDMANAGER->getFriend()[j]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[5].left, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[5].top);
					}
					FRIENDMANAGER->getFriend()[j]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[6].left, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[6].top);
					FRIENDMANAGER->getFriend()[j]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[7].left, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[7].top);
				}
			}
		}
	}

	//����
	if (FRIENDMANAGER->getFriend()[0]->getIsAtkRng())
	{
		FRIENDMANAGER->getFriend()[0]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[0].left, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[0].top);
		if (FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[1].right <= WINSIZEY)
		{
			FRIENDMANAGER->getFriend()[0]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[1].left, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[1].top);
		}
		FRIENDMANAGER->getFriend()[0]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[2].left, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[2].top);
		FRIENDMANAGER->getFriend()[0]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[3].left, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[3].top);
	}

	//����
	if (FRIENDMANAGER->getFriend()[1]->getIsAtkRng())
	{
		FRIENDMANAGER->getFriend()[1]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[0].left, FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[0].top);
		if (FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[1].right <= WINSIZEY)
		{
			FRIENDMANAGER->getFriend()[1]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[1].left, FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[1].top);
		}
		FRIENDMANAGER->getFriend()[1]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[2].left, FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[2].top);
		FRIENDMANAGER->getFriend()[1]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[3].left, FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[3].top);
	}

	//���
	if (FRIENDMANAGER->getFriend()[2]->getIsAtkRng())
	{
		FRIENDMANAGER->getFriend()[2]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[0].left, FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[0].top);
		if (FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[1].right <= WINSIZEY)
		{
			FRIENDMANAGER->getFriend()[2]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[1].left, FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[1].top);
		}
		FRIENDMANAGER->getFriend()[2]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[2].left, FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[2].top);
		FRIENDMANAGER->getFriend()[2]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[3].left, FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[3].top);
	}

	//����
	if (FRIENDMANAGER->getFriend()[3]->getIsAtkRng())
	{
		FRIENDMANAGER->getFriend()[3]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[0].left, FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[0].top);
		if (FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[1].right <= WINSIZEY)
		{
			FRIENDMANAGER->getFriend()[3]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[1].left, FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[1].top);
		}
		FRIENDMANAGER->getFriend()[3]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[2].left, FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[2].top);
		FRIENDMANAGER->getFriend()[3]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[3].left, FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[3].top);
	}

	//����
	if (FRIENDMANAGER->getFriend()[4]->getIsAtkRng())
	{
		FRIENDMANAGER->getFriend()[4]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[0].left, FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[0].top);
		if (FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[1].right <= WINSIZEY)
		{
			FRIENDMANAGER->getFriend()[4]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[1].left, FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[1].top);
		}
		FRIENDMANAGER->getFriend()[4]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[2].left, FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[2].top);
		FRIENDMANAGER->getFriend()[4]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[3].left, FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[3].top);
	}

	//�챺����
	for (int j = 5; j < 9; j++)
	{
		if (FRIENDMANAGER->getFriend()[j]->getIsAtkRng())
		{
			FRIENDMANAGER->getFriend()[j]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[0].left, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[0].top);
			FRIENDMANAGER->getFriend()[j]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[1].left, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[1].top);
			FRIENDMANAGER->getFriend()[j]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[2].left, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[2].top);
			if (FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[3].right <= WINSIZEY)
			{
				FRIENDMANAGER->getFriend()[j]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[3].left, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[3].top);
				FRIENDMANAGER->getFriend()[j]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[4].left, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[4].top);
				FRIENDMANAGER->getFriend()[j]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[5].left, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[5].top);
			}
			FRIENDMANAGER->getFriend()[j]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[6].left, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[6].top);
			FRIENDMANAGER->getFriend()[j]->getFriendInfo().atkRngImg->render(hdc, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[7].left, FRIENDMANAGER->getFriend()[j]->getFriendInfo().rcAtk[7].top);
		}
	}
}

void MainMap::enemyDraw(HDC hdc)
{
	//ĳ������ �̵� ������ ���� ������ �����ش�.
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (tiles[i].flood)
		{
			for (int j = 0; j < ENEMYMANAGER->getEnemy().size(); j++)
			{
				//����
				if (ENEMYMANAGER->getEnemy()[j]->getIsSelect())
				{
					//�̵�����
					ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

					//���ݹ���
					ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().moveAtkRngImg->render(hdc, ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rcAtk[0].left, ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rcAtk[0].top);
					if (ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rcAtk[1].right <= WINSIZEY)
					{
						ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().moveAtkRngImg->render(hdc, ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rcAtk[1].left, ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rcAtk[1].top);
					}
					ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().moveAtkRngImg->render(hdc, ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rcAtk[2].left, ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rcAtk[2].top);
					ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().moveAtkRngImg->render(hdc, ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rcAtk[3].left, ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rcAtk[3].top);
				}
			}
		}
	}

	for (int j = 0; j < ENEMYMANAGER->getEnemy().size(); j++)
	{
		if (ENEMYMANAGER->getEnemy()[j]->getIsAtkRng())
		{
			ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().atkRngImg->render(hdc, ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rcAtk[0].left, ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rcAtk[0].top);
			if (ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rcAtk[1].right <= WINSIZEY)
			{
				ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().atkRngImg->render(hdc, ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rcAtk[1].left, ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rcAtk[1].top);
			}
			ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().atkRngImg->render(hdc, ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rcAtk[2].left, ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rcAtk[2].top);
			ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().atkRngImg->render(hdc, ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rcAtk[3].left, ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rcAtk[3].top);
		}
	}
}
