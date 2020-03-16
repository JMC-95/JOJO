#include "stdafx.h"
#include "Jojo.h"

Jojo::Jojo()
{
}

Jojo::~Jojo()
{
}

HRESULT Jojo::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * playerImg, const char * atkImg, const char * blockImg)
{
	//구조체 정보 기입
	PlayerInfo jojo;
	//이미지 및 애니메이션
	jojo.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//캐릭터 클릭시 이동범위 이미지
	jojo.mAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);		//캐릭터 클릭시 공격범위 이미지
	jojo.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//공격버튼 클릭시 공격범위 이미지
	jojo.img = IMAGEMANAGER->findImage(playerImg);			//캐릭터 이미지
	jojo.atkImg = IMAGEMANAGER->findImage(atkImg);			//공격 이미지
	jojo.blockImg = IMAGEMANAGER->findImage(blockImg);		//방어 및 피격 이미지
	ANIMATIONMANAGER->addAnimation("playerLeft", "조조", 4, 5, 5, false, true);
	playerAni = ANIMATIONMANAGER->findAnimation("playerLeft");
	//스테이터스
	jojo.speed = 6;			//속도
	jojo.movingCount = 6;	//이동범위
	vJojo.push_back(jojo);

	pDirection = PLAYER_LEFT;

	startTile = endTile = -1;	//A*

	isTurn = true;

	return S_OK;
}

void Jojo::release()
{
}

void Jojo::update()
{
	if (isTurn)
	{
		mouseMove();

		if (startAstar && !isFind && !noPath)
		{
			while (!isFind)
			{
				aStar();
			}
		}

		if (!optimalPath.empty())
		{
			playerMove();

			if (playerX == mapX && playerY == mapY)
			{
				isTurn = false;
			}
		}
	}

	playerAnimation();

	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		isTurn = true;
		isAtk = true;
	}
}

void Jojo::render(HDC hdc)
{
	for (int k = 0; k < vJojo.size(); k++)
	{
		if (isRange)
		{
			for (int i = 0; i < TILE_X * TILE_Y; i++)
			{
				if (mainMap->getMap()[i].flood)
				{
					vJojo[k].moveRngImg->render(hdc, mainMap->getMap()[i].rc.left, mainMap->getMap()[i].rc.top);

					vJojo[k].mAtkRngImg->render(hdc, vJojo[k].rc.left - 48, vJojo[k].rc.top);
					vJojo[k].mAtkRngImg->render(hdc, vJojo[k].rc.left + 48, vJojo[k].rc.top);
					vJojo[k].mAtkRngImg->render(hdc, vJojo[k].rc.left, vJojo[k].rc.top - 48);
					vJojo[k].mAtkRngImg->render(hdc, vJojo[k].rc.left, vJojo[k].rc.top + 48);
				}
			}
		}

		if (isAtk)
		{
			vJojo[k].atkRngImg->render(hdc, vJojo[k].rc.left - 48, vJojo[k].rc.top);
			vJojo[k].atkRngImg->render(hdc, vJojo[k].rc.left + 48, vJojo[k].rc.top);
			vJojo[k].atkRngImg->render(hdc, vJojo[k].rc.left, vJojo[k].rc.top - 48);
			vJojo[k].atkRngImg->render(hdc, vJojo[k].rc.left, vJojo[k].rc.top + 48);
		}

		vJojo[k].img->aniRender(hdc, vJojo[k].rc.left, vJojo[k].rc.top, playerAni);
	}
}

void Jojo::mouseMove()
{
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			for (int k = 0; k < vJojo.size(); k++)
			{
				if (PtInRect(&vJojo[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
				{
					//선택한 타일 (시작)
					startTile = i;

					isSelect = true;
					isRange = true;
					isFind = false;
					noPath = false;
					startAstar = false;

					floodFill(startTile, vJojo[k].movingCount);
				}

				if (!PtInRect(&vJojo[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
				{
					//선택한 맵의 x좌표와 y좌표
					mapX = mainMap->getMap()[i].rc.left + (mainMap->getMap()[i].rc.right - mainMap->getMap()[i].rc.left) / 2;
					mapY = mainMap->getMap()[i].rc.top + (mainMap->getMap()[i].rc.bottom - mainMap->getMap()[i].rc.top) / 2;

					isSelect = false;

					if (mainMap->getMap()[i].flood)
					{
						//선택한 타일 (끝)
						endTile = i;
					}
					else
					{
						isRange = false;
						continue;
					}

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
}

void Jojo::playerMove()
{
	for (int k = 0; k < vJojo.size(); k++)
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

		if (vJojo[k].rc.left > 0 || vJojo[k].rc.right < WINSIZEY ||
			vJojo[k].rc.top > 0 || vJojo[k].rc.bottom < WINSIZEY)
		{
			switch (pDirection)
			{
			case PLAYER_LEFT:
				playerX -= vJojo[k].speed;
				vJojo[k].rc = RectMakeCenter(playerX, playerY, vJojo[k].img->getFrameWidth(), vJojo[k].img->getFrameHeight());
				break;
			case PLAYER_RIGHT:
				playerX += vJojo[k].speed;
				vJojo[k].rc = RectMakeCenter(playerX, playerY, vJojo[k].img->getFrameWidth(), vJojo[k].img->getFrameHeight());
				break;
			case PLAYER_UP:
				playerY -= vJojo[k].speed;
				vJojo[k].rc = RectMakeCenter(playerX, playerY, vJojo[k].img->getFrameWidth(), vJojo[k].img->getFrameHeight());
				break;
			case PLAYER_DOWN:
				playerY += vJojo[k].speed;
				vJojo[k].rc = RectMakeCenter(playerX, playerY, vJojo[k].img->getFrameWidth(), vJojo[k].img->getFrameHeight());
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

void Jojo::playerAnimation()
{
	switch (pDirection)
	{
	case PLAYER_LEFT:
		ANIMATIONMANAGER->addAnimation("playerLeft", "조조", 4, 5, 5, false, true);
		playerAni = ANIMATIONMANAGER->findAnimation("playerLeft");
		ANIMATIONMANAGER->resume("playerLeft");
		break;
	case PLAYER_RIGHT:
		ANIMATIONMANAGER->addAnimation("playerRight", "조조", 6, 7, 5, false, true);
		playerAni = ANIMATIONMANAGER->findAnimation("playerRight");
		ANIMATIONMANAGER->resume("playerRight");
		break;
	case PLAYER_UP:
		ANIMATIONMANAGER->addAnimation("playerUp", "조조", 2, 3, 5, false, true);
		playerAni = ANIMATIONMANAGER->findAnimation("playerUp");
		ANIMATIONMANAGER->resume("playerUp");
		break;
	case PLAYER_DOWN:
		ANIMATIONMANAGER->addAnimation("playerDown", "조조", 0, 1, 5, false, true);
		playerAni = ANIMATIONMANAGER->findAnimation("playerDown");
		ANIMATIONMANAGER->resume("playerDown");
		break;
	}

	if (!isTurn)
	{
		ANIMATIONMANAGER->addAnimation("playerDie", "조조", 12, 13, 5, false, true);
		playerAni = ANIMATIONMANAGER->findAnimation("playerDie");
		ANIMATIONMANAGER->resume("playerDie");
	}
}

void Jojo::setPosition(RECT rc)
{
	for (int k = 0; k < vJojo.size(); k++)
	{
		vJojo[k].rc = rc;
		playerX = vJojo[k].rc.left + (vJojo[k].rc.right - vJojo[k].rc.left) / 2;
		playerY = vJojo[k].rc.top + (vJojo[k].rc.bottom - vJojo[k].rc.top) / 2;
	}
}
