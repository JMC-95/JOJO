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

	draw(hdc);

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

void MainMap::draw(HDC hdc)
{
	//캐릭터의 이동 범위와 공격 범위를 보여준다.
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (tiles[i].flood)
		{
			//악진
			if (PLAYERMANAGER->getAgjin()->getIsSelect())
			{
				//이동범위
				PLAYERMANAGER->getAgjin()->getPlayerVector()[0].moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//공격범위
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

			//하후돈
			if (PLAYERMANAGER->getHahudon()->getIsSelect())
			{
				//이동범위
				PLAYERMANAGER->getHahudon()->getPlayerVector()[0].moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//공격범위
				PLAYERMANAGER->getHahudon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rcAtk[0].left, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rcAtk[0].top);
				if (PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rcAtk[1].right <= WINSIZEY)
					PLAYERMANAGER->getHahudon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rcAtk[1].left, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rcAtk[1].top);
				PLAYERMANAGER->getHahudon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rcAtk[2].left, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rcAtk[2].top);
				PLAYERMANAGER->getHahudon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rcAtk[3].left, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rcAtk[3].top);
			}

			//하후연
			if (PLAYERMANAGER->getHahuyeon()->getIsSelect())
			{
				//이동범위
				PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//공격범위
				PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rcAtk[0].left, PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rcAtk[0].top);
				if (PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rcAtk[1].right <= WINSIZEY)
					PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rcAtk[1].left, PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rcAtk[1].top);
				PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rcAtk[2].left, PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rcAtk[2].top);
				PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rcAtk[3].left, PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rcAtk[3].top);
			}

			//이전
			if (PLAYERMANAGER->getIjeon()->getIsSelect())
			{
				//이동범위
				PLAYERMANAGER->getIjeon()->getPlayerVector()[0].moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//공격범위
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

			//조홍
			if (PLAYERMANAGER->getJohong()->getIsSelect())
			{
				//이동범위
				PLAYERMANAGER->getJohong()->getPlayerVector()[0].moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//공격범위
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

			//조인
			if (PLAYERMANAGER->getJoin()->getIsSelect())
			{
				//이동범위
				PLAYERMANAGER->getJoin()->getPlayerVector()[0].moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//공격범위
				PLAYERMANAGER->getJoin()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getJoin()->getPlayerVector()[0].rcAtk[0].left, PLAYERMANAGER->getJoin()->getPlayerVector()[0].rcAtk[0].top);
				if (PLAYERMANAGER->getJoin()->getPlayerVector()[0].rcAtk[1].right <= WINSIZEY)
					PLAYERMANAGER->getJoin()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getJoin()->getPlayerVector()[0].rcAtk[1].left, PLAYERMANAGER->getJoin()->getPlayerVector()[0].rcAtk[1].top);
				PLAYERMANAGER->getJoin()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getJoin()->getPlayerVector()[0].rcAtk[2].left, PLAYERMANAGER->getJoin()->getPlayerVector()[0].rcAtk[2].top);
				PLAYERMANAGER->getJoin()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getJoin()->getPlayerVector()[0].rcAtk[3].left, PLAYERMANAGER->getJoin()->getPlayerVector()[0].rcAtk[3].top);
			}

			//조조
			if (PLAYERMANAGER->getJojo()->getIsSelect())
			{
				//이동범위
				PLAYERMANAGER->getJojo()->getPlayerVector()[0].moveRngImg->render(hdc, tiles[i].rc.left, tiles[i].rc.top);

				//공격범위
				PLAYERMANAGER->getJojo()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[0].left, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[0].top);
				if (PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[1].right <= WINSIZEY)
					PLAYERMANAGER->getJojo()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[1].left, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[1].top);
				PLAYERMANAGER->getJojo()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[2].left, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[2].top);
				PLAYERMANAGER->getJojo()->getPlayerVector()[0].mAtkRngImg->render(hdc, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[3].left, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[3].top);
			}
		}
	}

	//공격 버튼을 클릭하면 공격범위를 보여준다.
	if (PLAYERMANAGER->getJojo()->getIsAtkRng())
	{
		PLAYERMANAGER->getJojo()->getPlayerVector()[0].atkRngImg->render(hdc, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[0].left, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[0].top);
		if (PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[5].right <= WINSIZEY)
			PLAYERMANAGER->getJojo()->getPlayerVector()[0].atkRngImg->render(hdc, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[1].left, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[1].top);
		PLAYERMANAGER->getJojo()->getPlayerVector()[0].atkRngImg->render(hdc, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[2].left, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[2].top);
		PLAYERMANAGER->getJojo()->getPlayerVector()[0].atkRngImg->render(hdc, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[3].left, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[3].top);
	}
}
