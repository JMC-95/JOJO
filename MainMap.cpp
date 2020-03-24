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
			//악진
			if (PLAYERMANAGER->getPlayer()[0]->getIsSelect())
			{
				//이동범위
				PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//공격범위
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

			//하후돈
			if (PLAYERMANAGER->getPlayer()[1]->getIsSelect())
			{
				//이동범위
				PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//공격범위
				PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[0].left, PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[0].top);
				if (PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[1].right <= WINSIZEY)
				{
					PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[1].left, PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[1].top);
				}
				PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[2].left, PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[2].top);
				PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[3].left, PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[3].top);
			}

			//하후연
			if (PLAYERMANAGER->getPlayer()[2]->getIsSelect())
			{
				//이동범위
				PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//공격범위
				PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[0].left, PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[0].top);
				if (PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[1].right <= WINSIZEY)
				{
					PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[1].left, PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[1].top);
				}
				PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[2].left, PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[2].top);
				PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[3].left, PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[3].top);
			}

			//이전
			if (PLAYERMANAGER->getPlayer()[3]->getIsSelect())
			{
				//이동범위
				PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//공격범위
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

			//조홍
			if (PLAYERMANAGER->getPlayer()[4]->getIsSelect())
			{
				//이동범위
				PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//공격범위
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

			//조인
			if (PLAYERMANAGER->getPlayer()[5]->getIsSelect())
			{
				//이동범위
				PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//공격범위
				PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[0].left, PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[0].top);
				if (PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[1].right <= WINSIZEY)
				{
					PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[1].left, PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[1].top);
				}
				PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[2].left, PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[2].top);
				PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().moveAtkRngImg->render(hdc, PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[3].left, PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[3].top);
			}

			//조조
			if (PLAYERMANAGER->getPlayer()[6]->getIsSelect())
			{
				//이동범위
				PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//공격범위
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

	//공격 버튼을 클릭하면 공격범위를 보여준다.
	//악진
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

	//하후돈
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

	//하후연
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

	//이전
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

	//조홍
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

	//조인
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
		//조조
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
	//캐릭터의 이동 범위와 공격 범위를 보여준다.
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (tiles[i].flood)
		{
			//도겸
			if (FRIENDMANAGER->getFriend()[0]->getIsSelect())
			{
				//이동범위
				FRIENDMANAGER->getFriend()[0]->getFriendInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//공격범위
				FRIENDMANAGER->getFriend()[0]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[0].left, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[0].top);
				if (FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[1].right <= WINSIZEY)
				{
					FRIENDMANAGER->getFriend()[0]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[1].left, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[1].top);
				}
				FRIENDMANAGER->getFriend()[0]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[2].left, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[2].top);
				FRIENDMANAGER->getFriend()[0]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[3].left, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[3].top);
			}

			//관우
			if (FRIENDMANAGER->getFriend()[1]->getIsSelect())
			{
				//이동범위
				FRIENDMANAGER->getFriend()[1]->getFriendInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//공격범위
				FRIENDMANAGER->getFriend()[1]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[0].left, FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[0].top);
				if (FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[1].right <= WINSIZEY)
				{
					FRIENDMANAGER->getFriend()[1]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[1].left, FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[1].top);
				}
				FRIENDMANAGER->getFriend()[1]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[2].left, FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[2].top);
				FRIENDMANAGER->getFriend()[1]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[3].left, FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[3].top);
			}

			//장비
			if (FRIENDMANAGER->getFriend()[2]->getIsSelect())
			{
				//이동범위
				FRIENDMANAGER->getFriend()[2]->getFriendInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//공격범위
				FRIENDMANAGER->getFriend()[2]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[0].left, FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[0].top);
				if (FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[1].right <= WINSIZEY)
				{
					FRIENDMANAGER->getFriend()[2]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[1].left, FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[1].top);
				}
				FRIENDMANAGER->getFriend()[2]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[2].left, FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[2].top);
				FRIENDMANAGER->getFriend()[2]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[3].left, FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[3].top);
			}

			//원소
			if (FRIENDMANAGER->getFriend()[3]->getIsSelect())
			{
				//이동범위
				FRIENDMANAGER->getFriend()[3]->getFriendInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//공격범위
				FRIENDMANAGER->getFriend()[3]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[0].left, FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[0].top);
				if (FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[1].right <= WINSIZEY)
				{
					FRIENDMANAGER->getFriend()[3]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[1].left, FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[1].top);
				}
				FRIENDMANAGER->getFriend()[3]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[2].left, FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[2].top);
				FRIENDMANAGER->getFriend()[3]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[3].left, FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[3].top);
			}

			//유비
			if (FRIENDMANAGER->getFriend()[4]->getIsSelect())
			{
				//이동범위
				FRIENDMANAGER->getFriend()[4]->getFriendInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//공격범위
				FRIENDMANAGER->getFriend()[4]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[0].left, FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[0].top);
				if (FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[1].right <= WINSIZEY)
				{
					FRIENDMANAGER->getFriend()[4]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[1].left, FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[1].top);
				}
				FRIENDMANAGER->getFriend()[4]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[2].left, FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[2].top);
				FRIENDMANAGER->getFriend()[4]->getFriendInfo().moveAtkRngImg->render(hdc, FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[3].left, FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[3].top);
			}

			//우군보병
			for (int j = 5; j < 9; j++)
			{
				if (FRIENDMANAGER->getFriend()[j]->getIsSelect())
				{
					//이동범위
					FRIENDMANAGER->getFriend()[j]->getFriendInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

					//공격범위
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

	//도겸
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

	//관우
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

	//장비
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

	//원소
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

	//유비
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

	//우군보병
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
	//캐릭터의 이동 범위와 공격 범위를 보여준다.
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (tiles[i].flood)
		{
			for (int j = 0; j < ENEMYMANAGER->getEnemy().size(); j++)
			{
				//번주
				if (ENEMYMANAGER->getEnemy()[j]->getIsSelect())
				{
					//이동범위
					ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

					//공격범위
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
