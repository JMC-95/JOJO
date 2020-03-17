#include "stdafx.h"
#include "Hahudon.h"

Hahudon::Hahudon()
{
}

Hahudon::~Hahudon()
{
}

HRESULT Hahudon::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * playerImg, const char * atkImg, const char * blockImg)
{
	//구조체 정보 기입
	PlayerInfo hahudon;
	//이미지 및 애니메이션
	hahudon.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//캐릭터 클릭시 이동범위 이미지
	hahudon.mAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);		//캐릭터 클릭시 공격범위 이미지
	hahudon.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//공격버튼 클릭시 공격범위 이미지
	hahudon.img = IMAGEMANAGER->findImage(playerImg);			//캐릭터 이미지
	hahudon.atkImg = IMAGEMANAGER->findImage(atkImg);			//공격 이미지
	hahudon.blockImg = IMAGEMANAGER->findImage(blockImg);		//방어 및 피격 이미지
	ANIMATIONMANAGER->addAnimation("playerLeft", "하후돈", 4, 5, 2, false, true);
	playerAni = ANIMATIONMANAGER->findAnimation("playerLeft");
	//스테이터스
	hahudon.speed = 6;			//속도
	hahudon.movingCount = 6;	//이동범위
	vHahudon.push_back(hahudon);

	pDirection = PLAYER_LEFT;

	startTile = endTile = -1;	//A*

	isTurn = true;

	return S_OK;
}

void Hahudon::release()
{
}

void Hahudon::update()
{
	if (isTurn)
	{
		mouseMove();
	}

	playerAnimation();

	if (KEYMANAGER->isOnceKeyDown('2'))
	{
		isTurn = true;
	}
}

void Hahudon::render(HDC hdc)
{
	for (int k = 0; k < vHahudon.size(); k++)
	{
		vHahudon[k].img->aniRender(hdc, vHahudon[k].rc.left, vHahudon[k].rc.top, playerAni);
	}
}

void Hahudon::mouseMove()
{
	KEYMANAGER->reset();
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			for (int k = 0; k < vHahudon.size(); k++)
			{
				if (PtInRect(&vHahudon[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
				{
					//선택한 타일 (시작)
					startTile = i;

					isSelect = true;
					isFind = false;
					noPath = false;
					startAstar = false;

					floodFill(startTile, vHahudon[k].movingCount);
				}

				if (!PtInRect(&vHahudon[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
				{
					//선택한 맵의 x좌표와 y좌표
					mapX = mainMap->getMap()[i].rc.left + (mainMap->getMap()[i].rc.right - mainMap->getMap()[i].rc.left) / 2;
					mapY = mainMap->getMap()[i].rc.top + (mainMap->getMap()[i].rc.bottom - mainMap->getMap()[i].rc.top) / 2;

					isSelect = false;

					if (mainMap->getMap()[i].flood)
					{
						//선택한 타일 (끝)
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
		playerMove();

		if (playerX == mapX && playerY == mapY)
		{
			//isTurn = false;
		}
	}
}

void Hahudon::playerMove()
{
	for (int k = 0; k < vHahudon.size(); k++)
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

		if (vHahudon[k].rc.left > 0 || vHahudon[k].rc.right < WINSIZEY ||
			vHahudon[k].rc.top > 0 || vHahudon[k].rc.bottom < WINSIZEY)
		{
			switch (pDirection)
			{
			case PLAYER_LEFT:
				playerX -= vHahudon[k].speed;
				vHahudon[k].rc = RectMakeCenter(playerX, playerY, vHahudon[k].img->getFrameWidth(), vHahudon[k].img->getFrameHeight());
				break;
			case PLAYER_RIGHT:
				playerX += vHahudon[k].speed;
				vHahudon[k].rc = RectMakeCenter(playerX, playerY, vHahudon[k].img->getFrameWidth(), vHahudon[k].img->getFrameHeight());
				break;
			case PLAYER_UP:
				playerY -= vHahudon[k].speed;
				vHahudon[k].rc = RectMakeCenter(playerX, playerY, vHahudon[k].img->getFrameWidth(), vHahudon[k].img->getFrameHeight());
				break;
			case PLAYER_DOWN:
				playerY += vHahudon[k].speed;
				vHahudon[k].rc = RectMakeCenter(playerX, playerY, vHahudon[k].img->getFrameWidth(), vHahudon[k].img->getFrameHeight());
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

void Hahudon::playerAnimation()
{
	switch (pDirection)
	{
	case PLAYER_LEFT:
		ANIMATIONMANAGER->addAnimation("playerLeft", "하후돈", 4, 5, 2, false, true);
		playerAni = ANIMATIONMANAGER->findAnimation("playerLeft");
		ANIMATIONMANAGER->resume("playerLeft");
		break;
	case PLAYER_RIGHT:
		ANIMATIONMANAGER->addAnimation("playerRight", "하후돈", 6, 7, 2, false, true);
		playerAni = ANIMATIONMANAGER->findAnimation("playerRight");
		ANIMATIONMANAGER->resume("playerRight");
		break;
	case PLAYER_UP:
		ANIMATIONMANAGER->addAnimation("playerUp", "하후돈", 2, 3, 2, false, true);
		playerAni = ANIMATIONMANAGER->findAnimation("playerUp");
		ANIMATIONMANAGER->resume("playerUp");
		break;
	case PLAYER_DOWN:
		ANIMATIONMANAGER->addAnimation("playerDown", "하후돈", 0, 1, 2, false, true);
		playerAni = ANIMATIONMANAGER->findAnimation("playerDown");
		ANIMATIONMANAGER->resume("playerDown");
		break;
	}

	if (!isTurn)
	{
		ANIMATIONMANAGER->addAnimation("playerDie", "하후돈", 12, 13, 2, false, true);
		playerAni = ANIMATIONMANAGER->findAnimation("playerDie");
		ANIMATIONMANAGER->resume("playerDie");
	}
}

void Hahudon::setPosition(RECT rc)
{
	for (int k = 0; k < vHahudon.size(); k++)
	{
		vHahudon[k].rc = rc;
		playerX = vHahudon[k].rc.left + (vHahudon[k].rc.right - vHahudon[k].rc.left) / 2;
		playerY = vHahudon[k].rc.top + (vHahudon[k].rc.bottom - vHahudon[k].rc.top) / 2;
	}
}
