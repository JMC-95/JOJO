#include "stdafx.h"
#include "Ijeon.h"

Ijeon::Ijeon()
{
}

Ijeon::~Ijeon()
{
}

HRESULT Ijeon::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * playerImg, const char * atkImg, const char * blockImg)
{
	//구조체 정보 기입
	PlayerInfo Ijeon;
	//이미지 및 애니메이션
	Ijeon.moveRngImg = IMAGEMANAGER->findImage(moveImg);	//캐릭터 클릭시 이동범위 이미지
	Ijeon.mAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//캐릭터 클릭시 공격범위 이미지
	Ijeon.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//공격버튼 클릭시 공격범위 이미지
	Ijeon.img = IMAGEMANAGER->findImage(playerImg);			//캐릭터 이미지
	Ijeon.atkImg = IMAGEMANAGER->findImage(atkImg);			//공격 이미지
	Ijeon.blockImg = IMAGEMANAGER->findImage(blockImg);		//방어 및 피격 이미지
	ANIMATIONMANAGER->addAnimation("playerLeft", "이전", 4, 5, 2, false, true);
	playerAni = ANIMATIONMANAGER->findAnimation("playerLeft");
	//스테이터스
	Ijeon.speed = 6;			//속도
	Ijeon.movingCount = 4;		//이동범위
	vIjeon.push_back(Ijeon);

	pDirection = PLAYER_LEFT;

	count = 2;
	startTile = endTile = -1;	//A*

	isTurn = true;
	isSelect = false;

	return S_OK;
}

void Ijeon::release()
{
}

void Ijeon::update()
{
	if (isTurn)
	{
		mouseMove();
	}

	playerAnimation();
}

void Ijeon::render(HDC hdc)
{
	for (int k = 0; k < vIjeon.size(); k++)
	{
		if (isTurn)
		{
			vIjeon[k].img->aniRender(hdc, vIjeon[k].rc.left, vIjeon[k].rc.top, playerAni);
		}
		else
		{
			vIjeon[k].img->frameAlphaRender(hdc, vIjeon[k].rc.left, vIjeon[k].rc.top, 0, frameNumY, 100);
		}

		if (isClick)
		{
			IMAGEMANAGER->frameRender("메뉴", hdc, vIjeon[k].rc.left - 100, vIjeon[k].rc.top - 35, frameNumX, 0);
		}
	}
}

void Ijeon::mouseMove()
{
	for (int k = 0; k < vIjeon.size(); k++)
	{
		if (!PLAYERMANAGER->getAgjin()->getIsSelect() && !PLAYERMANAGER->getHahudon()->getIsSelect() &&
			!PLAYERMANAGER->getHahuyeon()->getIsSelect() && !PLAYERMANAGER->getJohong()->getIsSelect() &&
			!PLAYERMANAGER->getJoin()->getIsSelect() && !PLAYERMANAGER->getJojo()->getIsSelect() &&
			KEYMANAGER->isStayKeyDown(VK_LBUTTON))
		{
			for (int i = 0; i < TILE_X * TILE_Y; i++)
			{
				if (PtInRect(&vIjeon[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
				{
					//선택한 타일 (캐릭터)
					startTile = i;
					count -= 1;

					isSelect = true;
					isFind = false;
					noPath = false;
					startAstar = false;

					//공격범위
					for (int j = 0; j < 8; j++)
					{
						vIjeon[k].rcAtk[0] = RectMake(vIjeon[k].rc.left - 48, vIjeon[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
						vIjeon[k].rcAtk[1] = RectMake(vIjeon[k].rc.left - 48, vIjeon[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
						vIjeon[k].rcAtk[2] = RectMake(vIjeon[k].rc.left - 48, vIjeon[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
						vIjeon[k].rcAtk[3] = RectMake(vIjeon[k].rc.left + 48, vIjeon[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
						vIjeon[k].rcAtk[4] = RectMake(vIjeon[k].rc.left + 48, vIjeon[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
						vIjeon[k].rcAtk[5] = RectMake(vIjeon[k].rc.left + 48, vIjeon[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
						vIjeon[k].rcAtk[6] = RectMake(vIjeon[k].rc.left, vIjeon[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
						vIjeon[k].rcAtk[7] = RectMake(vIjeon[k].rc.left, vIjeon[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
						atkList.push_back(vIjeon[k].rcAtk[j]);
					}

					//이동범위
					if (!isCancel)
					{
						floodFill(startTile, vIjeon[k].movingCount);
					}
				}

				if (!PtInRect(&vIjeon[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
				{
					//선택한 맵의 x좌표와 y좌표
					mapX = mainMap->getMap()[i].rc.left + (mainMap->getMap()[i].rc.right - mainMap->getMap()[i].rc.left) / 2;
					mapY = mainMap->getMap()[i].rc.top + (mainMap->getMap()[i].rc.bottom - mainMap->getMap()[i].rc.top) / 2;

					if (mainMap->getMap()[i].flood)
					{
						//선택한 타일 (목표)
						endTile = i;

						//이순간 Astar가 시작된다.
						//Astar에 필요한 모든 것을 초기화 시켜주자.
						openList.clear();
						closeList.clear();

						if (startTile != -1 && endTile != -1)
						{
							startAstar = true;
							currentTile = startTile;

							//시작 지점을 openList에 넣자
							openList.push_back(currentTile);
						}
					}

					for (int i = 0; i < TILE_X * TILE_Y; i++)
					{
						if (mainMap->getMap()[i].flood)
						{
							mainMap->getMap()[i].flood = false;
						}
					}
				}
			}
		}

		//목표 타일을 클릭하면 A* 시작
		if (startAstar && !isFind && !noPath)
		{
			while (!isFind)
			{
				aStar();
			}
		}

		//목표 타일을 클릭하면 캐릭터 이동
		if (!optimalPath.empty())
		{
			if (!isCancel)
			{
				playerMove();
			}

			if (playerX == mapX && playerY == mapY)
			{
				isCancel = true;
				isClick = true;

				//공격범위
				for (int j = 0; j < 8; j++)
				{
					vIjeon[k].rcAtk[0] = RectMake(vIjeon[k].rc.left - 48, vIjeon[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
					vIjeon[k].rcAtk[1] = RectMake(vIjeon[k].rc.left - 48, vIjeon[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					vIjeon[k].rcAtk[2] = RectMake(vIjeon[k].rc.left - 48, vIjeon[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					vIjeon[k].rcAtk[3] = RectMake(vIjeon[k].rc.left + 48, vIjeon[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
					vIjeon[k].rcAtk[4] = RectMake(vIjeon[k].rc.left + 48, vIjeon[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					vIjeon[k].rcAtk[5] = RectMake(vIjeon[k].rc.left + 48, vIjeon[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					vIjeon[k].rcAtk[6] = RectMake(vIjeon[k].rc.left, vIjeon[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					vIjeon[k].rcAtk[7] = RectMake(vIjeon[k].rc.left, vIjeon[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(vIjeon[k].rcAtk[j]);
				}

				//메뉴선택 렉트
				for (int j = 0; j < 5; j++)
				{
					rcMenu[0] = RectMake(vIjeon[k].rc.left - 97, vIjeon[k].rc.top - 30, 82, 20);
					rcMenu[1] = RectMake(vIjeon[k].rc.left - 97, vIjeon[k].rc.top - 9, 82, 20);
					rcMenu[2] = RectMake(vIjeon[k].rc.left - 97, vIjeon[k].rc.top + 12, 82, 20);
					rcMenu[3] = RectMake(vIjeon[k].rc.left - 97, vIjeon[k].rc.top + 38, 82, 20);
					rcMenu[4] = RectMake(vIjeon[k].rc.left - 97, vIjeon[k].rc.top + 63, 82, 20);
					menuList.push_back(rcMenu[j]);
				}
			}
		}

		if (isClick)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				if (PtInRect(&rcMenu[0], m_ptMouse) && isAtk)	//공격
				{
					atkList.clear();
					menuList.clear();

					isAtkRng = true;
					isClick = false;
				}
				if (PtInRect(&rcMenu[1], m_ptMouse))	//책략
				{
					atkList.clear();
					menuList.clear();

					isClick = false;
				}
				if (PtInRect(&rcMenu[2], m_ptMouse))	//도구
				{
					atkList.clear();
					menuList.clear();

					isClick = false;
				}
				if (PtInRect(&rcMenu[3], m_ptMouse))	//대기
				{
					atkList.clear();
					menuList.clear();

					isTurn = false;
					isSelect = false;
					isClick = false;
				}
				if (PtInRect(&rcMenu[4], m_ptMouse))	//취소
				{
					atkList.clear();
					menuList.clear();

					isCancel = false;
					isClick = false;
				}
			}
		}

		RECT temp;

		if (IntersectRect(&temp, &vIjeon[k].rcAtk[0], &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vIjeon[k].rcAtk[1], &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vIjeon[k].rcAtk[2], &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vIjeon[k].rcAtk[3], &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc))
		{
			isAtk = true;
			frameNumX = 1;
		}
		else
		{
			frameNumX = 0;
		}
	}
}

void Ijeon::playerMove()
{
	for (int k = 0; k < vIjeon.size(); k++)
	{
		stackX = optimalPath.top().rc.left + (optimalPath.top().rc.right - optimalPath.top().rc.left) / 2;
		stackY = optimalPath.top().rc.top + (optimalPath.top().rc.bottom - optimalPath.top().rc.top) / 2;

		if (!isMove)
		{
			if (playerX > stackX)
			{
				pDirection = PLAYER_LEFT;
			}
			else if (playerX < stackX)
			{
				pDirection = PLAYER_RIGHT;
			}
			else if (playerY > stackY)
			{
				pDirection = PLAYER_UP;
			}
			else if (playerY < stackY)
			{
				pDirection = PLAYER_DOWN;
			}

			isMove = true;
		}

		if (vIjeon[k].rc.left > 0 || vIjeon[k].rc.right < WINSIZEY ||
			vIjeon[k].rc.top > 0 || vIjeon[k].rc.bottom < WINSIZEY)
		{
			switch (pDirection)
			{
			case PLAYER_LEFT:
				playerX -= vIjeon[k].speed;
				vIjeon[k].rc = RectMakeCenter(playerX, playerY, vIjeon[k].img->getFrameWidth(), vIjeon[k].img->getFrameHeight());
				break;
			case PLAYER_RIGHT:
				playerX += vIjeon[k].speed;
				vIjeon[k].rc = RectMakeCenter(playerX, playerY, vIjeon[k].img->getFrameWidth(), vIjeon[k].img->getFrameHeight());
				break;
			case PLAYER_UP:
				playerY -= vIjeon[k].speed;
				vIjeon[k].rc = RectMakeCenter(playerX, playerY, vIjeon[k].img->getFrameWidth(), vIjeon[k].img->getFrameHeight());
				break;
			case PLAYER_DOWN:
				playerY += vIjeon[k].speed;
				vIjeon[k].rc = RectMakeCenter(playerX, playerY, vIjeon[k].img->getFrameWidth(), vIjeon[k].img->getFrameHeight());
				break;
			}

			if (playerX == stackX && playerY == stackY)
			{
				isMove = false;
				optimalPath.pop();
			}
		}
	}
}

void Ijeon::playerAnimation()
{
	if (isTurn)
	{
		switch (pDirection)
		{
		case PLAYER_LEFT:
			ANIMATIONMANAGER->addAnimation("playerLeft", "이전", 4, 5, 2, false, true);
			playerAni = ANIMATIONMANAGER->findAnimation("playerLeft");
			ANIMATIONMANAGER->resume("playerLeft");
			break;
		case PLAYER_RIGHT:
			ANIMATIONMANAGER->addAnimation("playerRight", "이전", 6, 7, 2, false, true);
			playerAni = ANIMATIONMANAGER->findAnimation("playerRight");
			ANIMATIONMANAGER->resume("playerRight");
			break;
		case PLAYER_UP:
			ANIMATIONMANAGER->addAnimation("playerUp", "이전", 2, 3, 2, false, true);
			playerAni = ANIMATIONMANAGER->findAnimation("playerUp");
			ANIMATIONMANAGER->resume("playerUp");
			break;
		case PLAYER_DOWN:
			ANIMATIONMANAGER->addAnimation("playerDown", "이전", 0, 1, 2, false, true);
			playerAni = ANIMATIONMANAGER->findAnimation("playerDown");
			ANIMATIONMANAGER->resume("playerDown");
			break;
		}
	}
	else
	{
		switch (pDirection)
		{
		case PLAYER_LEFT:
			frameNumY = 10;
			break;
		case PLAYER_RIGHT:
			frameNumY = 11;
			break;
		case PLAYER_UP:
			frameNumY = 9;
			break;
		case PLAYER_DOWN:
			frameNumY = 8;
			break;
		}
	}
}

void Ijeon::setPosition(RECT rc)
{
	for (int k = 0; k < vIjeon.size(); k++)
	{
		vIjeon[k].rc = rc;
		playerX = vIjeon[k].rc.left + (vIjeon[k].rc.right - vIjeon[k].rc.left) / 2;
		playerY = vIjeon[k].rc.top + (vIjeon[k].rc.bottom - vIjeon[k].rc.top) / 2;
	}
}
