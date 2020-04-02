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
		if (tiles[i].skill)
		{
			//������ ��ų ������ �����ش�.
			if (PLAYERMANAGER->getPlayer()[0]->getIsSkillCheck())
			{
				PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().atkRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);
			}
		}

		if (tiles[i].flood)
		{
			for (int j = 0; j < PLAYERMANAGER->getPlayer().size(); j++)
			{
				auto playerSelect = PLAYERMANAGER->getPlayer()[j]->getIsSelect();
				auto& playerInfo = PLAYERMANAGER->getPlayer()[j]->getPlayerInfo();

				if (playerInfo.className == "����" || playerInfo.className == "��⺴" || playerInfo.className == "�ñ⺴")
				{
					if (playerSelect)
					{
						//�̵�����
						playerInfo.moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

						//���ݹ���
						playerInfo.moveAtkRngImg->render(hdc, playerInfo.rcAtk[0].left, playerInfo.rcAtk[0].top);
						if (playerInfo.rcAtk[1].right <= WINSIZEY)
						{
							playerInfo.moveAtkRngImg->render(hdc, playerInfo.rcAtk[1].left, playerInfo.rcAtk[1].top);
						}
						playerInfo.moveAtkRngImg->render(hdc, playerInfo.rcAtk[2].left, playerInfo.rcAtk[2].top);
						playerInfo.moveAtkRngImg->render(hdc, playerInfo.rcAtk[3].left, playerInfo.rcAtk[3].top);
					}
				}

				if (playerInfo.className == "�溸��")
				{
					if (playerSelect)
					{
						//�̵�����
						playerInfo.moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

						//���ݹ���
						playerInfo.moveAtkRngImg->render(hdc, playerInfo.rcAtk[0].left, playerInfo.rcAtk[0].top);
						playerInfo.moveAtkRngImg->render(hdc, playerInfo.rcAtk[1].left, playerInfo.rcAtk[1].top);
						playerInfo.moveAtkRngImg->render(hdc, playerInfo.rcAtk[2].left, playerInfo.rcAtk[2].top);
						if (playerInfo.rcAtk[3].right <= WINSIZEY)
						{
							playerInfo.moveAtkRngImg->render(hdc, playerInfo.rcAtk[3].left, playerInfo.rcAtk[3].top);
							playerInfo.moveAtkRngImg->render(hdc, playerInfo.rcAtk[4].left, playerInfo.rcAtk[4].top);
							playerInfo.moveAtkRngImg->render(hdc, playerInfo.rcAtk[5].left, playerInfo.rcAtk[5].top);
						}
						playerInfo.moveAtkRngImg->render(hdc, playerInfo.rcAtk[6].left, playerInfo.rcAtk[6].top);
						playerInfo.moveAtkRngImg->render(hdc, playerInfo.rcAtk[7].left, playerInfo.rcAtk[7].top);
					}
				}
			}
		}
	}

	for (int j = 0; j < PLAYERMANAGER->getPlayer().size(); j++)
	{
		auto playerAtkRng = PLAYERMANAGER->getPlayer()[j]->getIsAtkRng();
		auto& playerInfo = PLAYERMANAGER->getPlayer()[j]->getPlayerInfo();

		if (playerInfo.className == "����" || playerInfo.className == "��⺴" || playerInfo.className == "�ñ⺴")
		{
			if (playerAtkRng)
			{
				playerInfo.atkRngImg->render(hdc, playerInfo.rcAtk[0].left, playerInfo.rcAtk[0].top);
				if (playerInfo.rcAtk[1].right <= WINSIZEY)
				{
					playerInfo.atkRngImg->render(hdc, playerInfo.rcAtk[1].left, playerInfo.rcAtk[1].top);
				}
				playerInfo.atkRngImg->render(hdc, playerInfo.rcAtk[2].left, playerInfo.rcAtk[2].top);
				playerInfo.atkRngImg->render(hdc, playerInfo.rcAtk[3].left, playerInfo.rcAtk[3].top);
			}
		}

		if (playerInfo.className == "�溸��")
		{
			if (playerAtkRng)
			{
				playerInfo.atkRngImg->render(hdc, playerInfo.rcAtk[0].left, playerInfo.rcAtk[0].top);
				playerInfo.atkRngImg->render(hdc, playerInfo.rcAtk[1].left, playerInfo.rcAtk[1].top);
				playerInfo.atkRngImg->render(hdc, playerInfo.rcAtk[2].left, playerInfo.rcAtk[2].top);
				if (playerInfo.rcAtk[3].right <= WINSIZEY)
				{
					playerInfo.atkRngImg->render(hdc, playerInfo.rcAtk[3].left, playerInfo.rcAtk[3].top);
					playerInfo.atkRngImg->render(hdc, playerInfo.rcAtk[4].left, playerInfo.rcAtk[4].top);
					playerInfo.atkRngImg->render(hdc, playerInfo.rcAtk[5].left, playerInfo.rcAtk[5].top);
				}
				playerInfo.atkRngImg->render(hdc, playerInfo.rcAtk[6].left, playerInfo.rcAtk[6].top);
				playerInfo.atkRngImg->render(hdc, playerInfo.rcAtk[7].left, playerInfo.rcAtk[7].top);
			}
		}
	}
}

void MainMap::friendDraw(HDC hdc)
{
	//ĳ������ �̵� ������ ���� ������ �����ش�.
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (tiles[i].flood)
		{
			for (int j = 0; j < FRIENDMANAGER->getFriend().size(); j++)
			{
				auto friendSelect = FRIENDMANAGER->getFriend()[j]->getIsSelect();
				auto& friendInfo = FRIENDMANAGER->getFriend()[j]->getFriendInfo();

				if (friendInfo.className == "����" || friendInfo.className == "��⺴")
				{
					if (friendSelect)
					{
						//�̵�����
						friendInfo.moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

						//���ݹ���
						friendInfo.moveAtkRngImg->render(hdc, friendInfo.rcAtk[0].left, friendInfo.rcAtk[0].top);
						if (friendInfo.rcAtk[1].right <= WINSIZEY)
						{
							friendInfo.moveAtkRngImg->render(hdc, friendInfo.rcAtk[1].left, friendInfo.rcAtk[1].top);
						}
						friendInfo.moveAtkRngImg->render(hdc, friendInfo.rcAtk[2].left, friendInfo.rcAtk[2].top);
						friendInfo.moveAtkRngImg->render(hdc, friendInfo.rcAtk[3].left, friendInfo.rcAtk[3].top);
					}
				}

				if (friendInfo.className == "�溸��")
				{
					if (friendSelect)
					{
						//�̵�����
						friendInfo.moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

						//���ݹ���
						friendInfo.moveAtkRngImg->render(hdc, friendInfo.rcAtk[0].left, friendInfo.rcAtk[0].top);
						friendInfo.moveAtkRngImg->render(hdc, friendInfo.rcAtk[1].left, friendInfo.rcAtk[1].top);
						friendInfo.moveAtkRngImg->render(hdc, friendInfo.rcAtk[2].left, friendInfo.rcAtk[2].top);
						if (friendInfo.rcAtk[3].right <= WINSIZEY)
						{
							friendInfo.moveAtkRngImg->render(hdc, friendInfo.rcAtk[3].left, friendInfo.rcAtk[3].top);
							friendInfo.moveAtkRngImg->render(hdc, friendInfo.rcAtk[4].left, friendInfo.rcAtk[4].top);
							friendInfo.moveAtkRngImg->render(hdc, friendInfo.rcAtk[5].left, friendInfo.rcAtk[5].top);
						}
						friendInfo.moveAtkRngImg->render(hdc, friendInfo.rcAtk[6].left, friendInfo.rcAtk[6].top);
						friendInfo.moveAtkRngImg->render(hdc, friendInfo.rcAtk[7].left, friendInfo.rcAtk[7].top);
					}
				}
			}
		}
	}

	for (int j = 0; j < FRIENDMANAGER->getFriend().size(); j++)
	{
		auto friendAtkrng = FRIENDMANAGER->getFriend()[j]->getIsAtkRng();
		auto& friendInfo = FRIENDMANAGER->getFriend()[j]->getFriendInfo();

		if (friendInfo.className == "����" || friendInfo.className == "��⺴")
		{
			if (friendAtkrng)
			{
				friendInfo.atkRngImg->render(hdc, friendInfo.rcAtk[0].left, friendInfo.rcAtk[0].top);
				if (friendInfo.rcAtk[1].right <= WINSIZEY)
				{
					friendInfo.atkRngImg->render(hdc, friendInfo.rcAtk[1].left, friendInfo.rcAtk[1].top);
				}
				friendInfo.atkRngImg->render(hdc, friendInfo.rcAtk[2].left, friendInfo.rcAtk[2].top);
				friendInfo.atkRngImg->render(hdc, friendInfo.rcAtk[3].left, friendInfo.rcAtk[3].top);
			}
		}

		if (friendInfo.className == "�溸��")
		{
			if (friendAtkrng)
			{
				friendInfo.atkRngImg->render(hdc, friendInfo.rcAtk[0].left, friendInfo.rcAtk[0].top);
				friendInfo.atkRngImg->render(hdc, friendInfo.rcAtk[1].left, friendInfo.rcAtk[1].top);
				friendInfo.atkRngImg->render(hdc, friendInfo.rcAtk[2].left, friendInfo.rcAtk[2].top);
				if (friendInfo.rcAtk[3].right <= WINSIZEY)
				{
					friendInfo.atkRngImg->render(hdc, friendInfo.rcAtk[3].left, friendInfo.rcAtk[3].top);
					friendInfo.atkRngImg->render(hdc, friendInfo.rcAtk[4].left, friendInfo.rcAtk[4].top);
					friendInfo.atkRngImg->render(hdc, friendInfo.rcAtk[5].left, friendInfo.rcAtk[5].top);
				}
				friendInfo.atkRngImg->render(hdc, friendInfo.rcAtk[6].left, friendInfo.rcAtk[6].top);
				friendInfo.atkRngImg->render(hdc, friendInfo.rcAtk[7].left, friendInfo.rcAtk[7].top);
			}
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
				auto enemySelect = ENEMYMANAGER->getEnemy()[j]->getIsSelect();
				auto& enemyInfo = ENEMYMANAGER->getEnemy()[j]->getEnemyInfo();

				if (enemyInfo.className == "����" || enemyInfo.className == "��⺴" || enemyInfo.className == "å��" || enemyInfo.className == "�����⺴")
				{
					if (enemySelect)
					{
						//�̵�����
						enemyInfo.moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

						//���ݹ���
						enemyInfo.moveAtkRngImg->render(hdc, enemyInfo.rcAtk[0].left, enemyInfo.rcAtk[0].top);
						if (enemyInfo.rcAtk[1].right <= WINSIZEY)
						{
							enemyInfo.moveAtkRngImg->render(hdc, enemyInfo.rcAtk[1].left, enemyInfo.rcAtk[1].top);
						}
						enemyInfo.moveAtkRngImg->render(hdc, enemyInfo.rcAtk[2].left, enemyInfo.rcAtk[2].top);
						enemyInfo.moveAtkRngImg->render(hdc, enemyInfo.rcAtk[3].left, enemyInfo.rcAtk[3].top);
					}
				}

				if (enemyInfo.className == "�溸��" || enemyInfo.className == "�ú�")
				{
					if (enemySelect)
					{
						//�̵�����
						enemyInfo.moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

						//���ݹ���
						enemyInfo.moveAtkRngImg->render(hdc, enemyInfo.rcAtk[0].left, enemyInfo.rcAtk[0].top);
						enemyInfo.moveAtkRngImg->render(hdc, enemyInfo.rcAtk[1].left, enemyInfo.rcAtk[1].top);
						enemyInfo.moveAtkRngImg->render(hdc, enemyInfo.rcAtk[2].left, enemyInfo.rcAtk[2].top);
						if (enemyInfo.rcAtk[3].right <= WINSIZEY)
						{
							enemyInfo.moveAtkRngImg->render(hdc, enemyInfo.rcAtk[3].left, enemyInfo.rcAtk[3].top);
							enemyInfo.moveAtkRngImg->render(hdc, enemyInfo.rcAtk[4].left, enemyInfo.rcAtk[4].top);
							enemyInfo.moveAtkRngImg->render(hdc, enemyInfo.rcAtk[5].left, enemyInfo.rcAtk[5].top);
						}
						enemyInfo.moveAtkRngImg->render(hdc, enemyInfo.rcAtk[6].left, enemyInfo.rcAtk[6].top);
						enemyInfo.moveAtkRngImg->render(hdc, enemyInfo.rcAtk[7].left, enemyInfo.rcAtk[7].top);
					}
				}
			}
		}
	}

	for (int j = 0; j < ENEMYMANAGER->getEnemy().size(); j++)
	{
		auto enemyAtkRng = ENEMYMANAGER->getEnemy()[j]->getIsAtkRng();
		auto& enemyInfo = ENEMYMANAGER->getEnemy()[j]->getEnemyInfo();

		if (enemyInfo.className == "����" || enemyInfo.className == "��⺴" || enemyInfo.className == "å��" || enemyInfo.className == "�����⺴")
		{
			if (enemyAtkRng)
			{
				enemyInfo.atkRngImg->render(hdc, enemyInfo.rcAtk[0].left, enemyInfo.rcAtk[0].top);
				if (enemyInfo.rcAtk[1].right <= WINSIZEY)
				{
					enemyInfo.atkRngImg->render(hdc, enemyInfo.rcAtk[1].left, enemyInfo.rcAtk[1].top);
				}
				enemyInfo.atkRngImg->render(hdc, enemyInfo.rcAtk[2].left, enemyInfo.rcAtk[2].top);
				enemyInfo.atkRngImg->render(hdc, enemyInfo.rcAtk[3].left, enemyInfo.rcAtk[3].top);
			}
		}

		if (enemyInfo.className == "�溸��" || enemyInfo.className == "�ú�")
		{
			if (enemyAtkRng)
			{
				enemyInfo.atkRngImg->render(hdc, enemyInfo.rcAtk[0].left, enemyInfo.rcAtk[0].top);
				enemyInfo.atkRngImg->render(hdc, enemyInfo.rcAtk[1].left, enemyInfo.rcAtk[1].top);
				enemyInfo.atkRngImg->render(hdc, enemyInfo.rcAtk[2].left, enemyInfo.rcAtk[2].top);
				if (enemyInfo.rcAtk[3].right <= WINSIZEY)
				{
					enemyInfo.atkRngImg->render(hdc, enemyInfo.rcAtk[3].left, enemyInfo.rcAtk[3].top);
					enemyInfo.atkRngImg->render(hdc, enemyInfo.rcAtk[4].left, enemyInfo.rcAtk[4].top);
					enemyInfo.atkRngImg->render(hdc, enemyInfo.rcAtk[5].left, enemyInfo.rcAtk[5].top);
				}
				enemyInfo.atkRngImg->render(hdc, enemyInfo.rcAtk[6].left, enemyInfo.rcAtk[6].top);
				enemyInfo.atkRngImg->render(hdc, enemyInfo.rcAtk[7].left, enemyInfo.rcAtk[7].top);
			}
		}
	}
}
