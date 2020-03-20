#include "stdafx.h"
#include "Join.h"

Join::Join()
{
}

Join::~Join()
{
}

HRESULT Join::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * playerImg, const char * atkImg, const char * blockImg)
{
	//구조체 정보 기입
	PlayerInfo join;
	//이미지 및 애니메이션
	join.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//캐릭터 클릭시 이동범위 이미지
	join.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);		//캐릭터 클릭시 공격범위 이미지
	join.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//공격버튼 클릭시 공격범위 이미지
	join.img = IMAGEMANAGER->findImage(playerImg);			//캐릭터 이미지
	join.atkImg = IMAGEMANAGER->findImage(atkImg);			//공격 이미지
	join.blockImg = IMAGEMANAGER->findImage(blockImg);		//방어 및 피격 이미지
	//스테이터스
	join.level = 4;				//레벨
	join.hp = 120;				//체력
	join.mp = 18;				//마력
	join.atk = 56;				//공격력
	join.will = 41;				//정신력
	join.def = 50;				//방어력
	join.agi = 43;				//순발력
	join.ten = 39;				//사기
	join.movingCount = 6;		//이동력
	vJoin.push_back(join);

	//HP ProgressBar
	_Hp = new progressBar;
	_Hp->init("images/UI/Info/HP.bmp", "images/UI/Info/Back_P.bmp", 1056, 289, 84, 12);
	_Hp->setGauge(currentHp, maxHp);
	currentHp = maxHp = 120;
	damage = 0;

	//MP ProgressBar
	_Mp = new progressBar;
	_Mp->init("images/UI/Info/MP.bmp", "images/UI/Info/Back_P.bmp", 1056, 307, 84, 12);
	_Mp->setGauge(currentMp, maxMp);
	currentMp = maxMp = 18;

	//EXP ProgressBar
	_Exp = new progressBar;
	_Exp->init("images/UI/Info/EXP.bmp", "images/UI/Info/Back_EXP.bmp", 1095, 243, 45, 12);
	_Exp->setGauge(currentExp, maxExp);
	currentExp = 70;
	maxExp = 100;

	//캐릭터 방향 및 위치
	pDirection = PLAYER_LEFT;
	startTile = endTile = -1;
	speed = 6;	//속도

	isTurn = true;
	isSelect = false;

	return S_OK;
}

void Join::release()
{
}

void Join::update()
{
	if (isTurn)
	{
		mouseMove();
	}

	playerAnimation();
	playerState();
}

void Join::render(HDC hdc)
{
	for (int k = 0; k < vJoin.size(); k++)
	{
		if (isTurn)
		{
			vJoin[k].img->aniRender(hdc, vJoin[k].rc.left, vJoin[k].rc.top, playerAni);
		}
		else
		{
			vJoin[k].img->frameAlphaRender(hdc, vJoin[k].rc.left, vJoin[k].rc.top, 0, frameY, 100);
		}
	}
}

void Join::mouseMove()
{
	for (int k = 0; k < vJoin.size(); k++)
	{
		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			if (PtInRect(&vJoin[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
			{
				if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
				{
					//선택한 타일 (캐릭터)
					startTile = i;

					isSelect = true;
					isFind = false;
					noPath = false;
					startAstar = false;

					//공격범위
					for (int j = 0; j < 4; j++)
					{
						vJoin[k].rcAtk[0] = RectMake(vJoin[k].rc.left - 48, vJoin[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
						vJoin[k].rcAtk[1] = RectMake(vJoin[k].rc.left + 48, vJoin[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
						vJoin[k].rcAtk[2] = RectMake(vJoin[k].rc.left, vJoin[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
						vJoin[k].rcAtk[3] = RectMake(vJoin[k].rc.left, vJoin[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
						atkList.push_back(vJoin[k].rcAtk[j]);
					}

					//이동범위
					if (!isStop)
					{
						floodFill(startTile, vJoin[k].movingCount);
					}
				}
			}

			if (!PtInRect(&vJoin[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
			{
				if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
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
			if (!isStop)
			{
				playerMove();
			}

			if (playerX == mapX && playerY == mapY)
			{
				isStop = true;
				isClick = true;

				//공격범위
				for (int j = 0; j < 4; j++)
				{
					vJoin[k].rcAtk[0] = RectMake(vJoin[k].rc.left - 48, vJoin[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
					vJoin[k].rcAtk[1] = RectMake(vJoin[k].rc.left + 48, vJoin[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
					vJoin[k].rcAtk[2] = RectMake(vJoin[k].rc.left, vJoin[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					vJoin[k].rcAtk[3] = RectMake(vJoin[k].rc.left, vJoin[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(vJoin[k].rcAtk[j]);
				}

				//메뉴선택 렉트
				for (int j = 0; j < 5; j++)
				{
					rcMenu[0] = RectMake(vJoin[k].rc.left - 97, vJoin[k].rc.top - 30, 82, 20);
					rcMenu[1] = RectMake(vJoin[k].rc.left - 97, vJoin[k].rc.top - 9, 82, 20);
					rcMenu[2] = RectMake(vJoin[k].rc.left - 97, vJoin[k].rc.top + 12, 82, 20);
					rcMenu[3] = RectMake(vJoin[k].rc.left - 97, vJoin[k].rc.top + 38, 82, 20);
					rcMenu[4] = RectMake(vJoin[k].rc.left - 97, vJoin[k].rc.top + 63, 82, 20);
					menuList.push_back(rcMenu[j]);
				}
			}
		}

		if (isClick)
		{
			if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
			{
				if (PtInRect(&rcMenu[0], m_ptMouse) && isTarget)	//공격
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

					//isClick = false;
				}
				if (PtInRect(&rcMenu[2], m_ptMouse))	//도구
				{
					atkList.clear();
					menuList.clear();

					//isClick = false;
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

					isStop = false;
					isClick = false;
				}
			}
		}
	}
}

void Join::playerMove()
{
	for (int k = 0; k < vJoin.size(); k++)
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

		if (vJoin[k].rc.left > 0 || vJoin[k].rc.right < WINSIZEY ||
			vJoin[k].rc.top > 0 || vJoin[k].rc.bottom < WINSIZEY)
		{
			switch (pDirection)
			{
			case PLAYER_LEFT:
				playerX -= speed;
				vJoin[k].rc = RectMakeCenter(playerX, playerY, vJoin[k].img->getFrameWidth(), vJoin[k].img->getFrameHeight());
				break;
			case PLAYER_RIGHT:
				playerX += speed;
				vJoin[k].rc = RectMakeCenter(playerX, playerY, vJoin[k].img->getFrameWidth(), vJoin[k].img->getFrameHeight());
				break;
			case PLAYER_UP:
				playerY -= speed;
				vJoin[k].rc = RectMakeCenter(playerX, playerY, vJoin[k].img->getFrameWidth(), vJoin[k].img->getFrameHeight());
				break;
			case PLAYER_DOWN:
				playerY += speed;
				vJoin[k].rc = RectMakeCenter(playerX, playerY, vJoin[k].img->getFrameWidth(), vJoin[k].img->getFrameHeight());
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

void Join::playerAnimation()
{
	if (isTurn)
	{
		switch (pDirection)
		{
		case PLAYER_LEFT:
			ANIMATIONMANAGER->addAnimation("playerLeft", "조인", 4, 5, 2, false, true);
			playerAni = ANIMATIONMANAGER->findAnimation("playerLeft");
			ANIMATIONMANAGER->resume("playerLeft");
			break;
		case PLAYER_RIGHT:
			ANIMATIONMANAGER->addAnimation("playerRight", "조인", 6, 7, 2, false, true);
			playerAni = ANIMATIONMANAGER->findAnimation("playerRight");
			ANIMATIONMANAGER->resume("playerRight");
			break;
		case PLAYER_UP:
			ANIMATIONMANAGER->addAnimation("playerUp", "조인", 2, 3, 2, false, true);
			playerAni = ANIMATIONMANAGER->findAnimation("playerUp");
			ANIMATIONMANAGER->resume("playerUp");
			break;
		case PLAYER_DOWN:
			ANIMATIONMANAGER->addAnimation("playerDown", "조인", 0, 1, 2, false, true);
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
			frameY = 10;
			break;
		case PLAYER_RIGHT:
			frameY = 11;
			break;
		case PLAYER_UP:
			frameY = 9;
			break;
		case PLAYER_DOWN:
			frameY = 8;
			break;
		}
	}
}

void Join::playerState()
{
	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);
}

void Join::setPosition(RECT rc)
{
	for (int k = 0; k < vJoin.size(); k++)
	{
		vJoin[k].rc = rc;
		playerX = vJoin[k].rc.left + (vJoin[k].rc.right - vJoin[k].rc.left) / 2;
		playerY = vJoin[k].rc.top + (vJoin[k].rc.bottom - vJoin[k].rc.top) / 2;
	}
}
