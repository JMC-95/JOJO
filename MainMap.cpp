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

	//날씨
	weatherImg = IMAGEMANAGER->findImage(imageName);
	ANIMATIONMANAGER->addDefAnimation("sunny", "맑음", 2, false, true);
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

	//현재 날씨를 보여준다.
	weatherImg->aniRender(hdc, 972, 12, weatherAni);

	//지형과 오브젝트를 맵에서 보여준다.
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

	//마우스와 맵의 타일이 충돌하면 그 타일의 속성을 보여준다.
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&tiles[i].rc, m_ptMouse))
		{
			IMAGEMANAGER->render("tileCheck", hdc, tiles[i].rc.left, tiles[i].rc.top);
			IMAGEMANAGER->alphaRender("배경", hdc, 972, 74, 100);

			if (tiles[i].terrain == TR_PLAIN)
			{
				IMAGEMANAGER->render("평지", hdc, 972, 74);
			}
			else if (tiles[i].terrain == TR_CASTLERAND)
			{
				IMAGEMANAGER->render("성내", hdc, 972, 74);
			}
			else if (tiles[i].terrain == TR_BARRACKS)
			{
				IMAGEMANAGER->render("병영", hdc, 972, 74);
			}
			else if (tiles[i].terrain == TR_BADLANDS)
			{
				IMAGEMANAGER->render("황무지", hdc, 972, 74);
			}
			else if (tiles[i].obj == OBJ_CASTLEGATE)
			{
				IMAGEMANAGER->render("성문", hdc, 972, 74);
			}
			else if (tiles[i].obj == OBJ_CASTLEWALLS)
			{
				IMAGEMANAGER->render("성벽", hdc, 972, 74);
			}
			else if (tiles[i].obj == OBJ_MOUNTAIN)
			{
				IMAGEMANAGER->render("산지", hdc, 972, 74);
			}
			else if (tiles[i].obj == OBJ_ROCKMOUNTAIN)
			{
				IMAGEMANAGER->render("바위산", hdc, 972, 74);
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
	//캐릭터의 이동 범위와 공격 범위를 보여준다.
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (tiles[i].flood)
		{
			//기병 + 궁병
			for (int j = 0; j < 4; j++)
			{
				auto playerSelect = PLAYERMANAGER->getPlayer()[j]->getIsSelect();
				auto& playerInfo = PLAYERMANAGER->getPlayer()[j]->getPlayerInfo();

				if (playerSelect)
				{
					//이동범위
					playerInfo.moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

					//공격범위
					playerInfo.moveAtkRngImg->render(hdc, playerInfo.rcAtk[0].left, playerInfo.rcAtk[0].top);
					if (playerInfo.rcAtk[1].right <= WINSIZEY)
					{
						playerInfo.moveAtkRngImg->render(hdc, playerInfo.rcAtk[1].left, playerInfo.rcAtk[1].top);
					}
					playerInfo.moveAtkRngImg->render(hdc, playerInfo.rcAtk[2].left, playerInfo.rcAtk[2].top);
					playerInfo.moveAtkRngImg->render(hdc, playerInfo.rcAtk[3].left, playerInfo.rcAtk[3].top);
				}
			}

			//보병
			for (int j = 4; j < PLAYERMANAGER->getPlayer().size(); j++)
			{
				auto playerSelect = PLAYERMANAGER->getPlayer()[j]->getIsSelect();
				auto& playerInfo = PLAYERMANAGER->getPlayer()[j]->getPlayerInfo();

				if (playerSelect)
				{
					//이동범위
					playerInfo.moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

					//공격범위
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

	//기병 + 궁병
	for (int j = 0; j < 4; j++)
	{
		auto playerAtkRng = PLAYERMANAGER->getPlayer()[j]->getIsAtkRng();
		auto& playerInfo = PLAYERMANAGER->getPlayer()[j]->getPlayerInfo();

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

	//보병
	for (int j = 4; j < PLAYERMANAGER->getPlayer().size(); j++)
	{
		auto playerAtkRng = PLAYERMANAGER->getPlayer()[j]->getIsAtkRng();
		auto& playerInfo = PLAYERMANAGER->getPlayer()[j]->getPlayerInfo();

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

void MainMap::friendDraw(HDC hdc)
{
	//캐릭터의 이동 범위와 공격 범위를 보여준다.
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (tiles[i].flood)
		{
			//기병
			for (int j = 0; j < 5; j++)
			{
				auto friendSelect = FRIENDMANAGER->getFriend()[j]->getIsSelect();
				auto& friendInfo = FRIENDMANAGER->getFriend()[j]->getFriendInfo();

				if (friendSelect)
				{
					//이동범위
					friendInfo.moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

					//공격범위
					friendInfo.moveAtkRngImg->render(hdc, friendInfo.rcAtk[0].left, friendInfo.rcAtk[0].top);
					if (friendInfo.rcAtk[1].right <= WINSIZEY)
					{
						friendInfo.moveAtkRngImg->render(hdc, friendInfo.rcAtk[1].left, friendInfo.rcAtk[1].top);
					}
					friendInfo.moveAtkRngImg->render(hdc, friendInfo.rcAtk[2].left, friendInfo.rcAtk[2].top);
					friendInfo.moveAtkRngImg->render(hdc, friendInfo.rcAtk[3].left, friendInfo.rcAtk[3].top);
				}
			}

			//보병
			for (int j = 5; j < FRIENDMANAGER->getFriend().size(); j++)
			{
				auto friendSelect = FRIENDMANAGER->getFriend()[j]->getIsSelect();
				auto& friendInfo = FRIENDMANAGER->getFriend()[j]->getFriendInfo();

				if (friendSelect)
				{
					//이동범위
					friendInfo.moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

					//공격범위
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

	//기병
	for (int j = 0; j < 5; j++)
	{
		auto friendAtkrng = FRIENDMANAGER->getFriend()[j]->getIsAtkRng();
		auto& friendInfo = FRIENDMANAGER->getFriend()[j]->getFriendInfo();

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

	//보병
	for (int j = 5; j < FRIENDMANAGER->getFriend().size(); j++)
	{
		auto friendAtkrng = FRIENDMANAGER->getFriend()[j]->getIsAtkRng();
		auto& friendInfo = FRIENDMANAGER->getFriend()[j]->getFriendInfo();

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

void MainMap::enemyDraw(HDC hdc)
{
	//캐릭터의 이동 범위와 공격 범위를 보여준다.
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (tiles[i].flood)
		{
			//보병 + 궁병 제외
			for (int j = 0; j < 9; j++)
			{
				auto enemySelect = ENEMYMANAGER->getEnemy()[j]->getIsSelect();
				auto& enemyInfo = ENEMYMANAGER->getEnemy()[j]->getEnemyInfo();

				if (enemySelect)
				{
					//이동범위
					enemyInfo.moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

					//공격범위
					enemyInfo.moveAtkRngImg->render(hdc, enemyInfo.rcAtk[0].left, enemyInfo.rcAtk[0].top);
					if (enemyInfo.rcAtk[1].right <= WINSIZEY)
					{
						enemyInfo.moveAtkRngImg->render(hdc, enemyInfo.rcAtk[1].left, enemyInfo.rcAtk[1].top);
					}
					enemyInfo.moveAtkRngImg->render(hdc, enemyInfo.rcAtk[2].left, enemyInfo.rcAtk[2].top);
					enemyInfo.moveAtkRngImg->render(hdc, enemyInfo.rcAtk[3].left, enemyInfo.rcAtk[3].top);
				}
			}

			//보병 + 궁병 포함
			for (int j = 9; j < ENEMYMANAGER->getEnemy().size(); j++)
			{
				auto enemySelect = ENEMYMANAGER->getEnemy()[j]->getIsSelect();
				auto& enemyInfo = ENEMYMANAGER->getEnemy()[j]->getEnemyInfo();

				if (enemySelect)
				{
					//이동범위
					enemyInfo.moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

					//공격범위
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

	//보병 + 궁병 제외
	for (int j = 0; j < 9; j++)
	{
		auto enemyAtkRng = ENEMYMANAGER->getEnemy()[j]->getIsAtkRng();
		auto& enemyInfo = ENEMYMANAGER->getEnemy()[j]->getEnemyInfo();

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

	//보병 + 궁병 포함
	for (int j = 9; j < ENEMYMANAGER->getEnemy().size(); j++)
	{
		auto enemyAtkRng = ENEMYMANAGER->getEnemy()[j]->getIsAtkRng();
		auto& enemyInfo = ENEMYMANAGER->getEnemy()[j]->getEnemyInfo();

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
