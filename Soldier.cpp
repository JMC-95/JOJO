#include "stdafx.h"
#include "Soldier.h"

Soldier::Soldier()
{
}

Soldier::~Soldier()
{
}

HRESULT Soldier::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * friendImg, const char * atkImg, const char * blockImg)
{
	//구조체 정보 기입
	FriendInfo soldier;
	//이미지 및 애니메이션
	soldier.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//캐릭터 클릭시 이동범위 이미지
	soldier.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//캐릭터 클릭시 공격범위 이미지
	soldier.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//공격버튼 클릭시 공격범위 이미지
	soldier.img = IMAGEMANAGER->findImage(friendImg);			//캐릭터 이미지
	soldier.atkImg = IMAGEMANAGER->findImage(atkImg);			//공격 이미지
	soldier.blockImg = IMAGEMANAGER->findImage(blockImg);		//방어 및 피격 이미지
	//스테이터스
	soldier.level = 2;			//레벨
	soldier.hp = 125;			//체력
	soldier.mp = 35;			//마력
	soldier.atk = 54;			//공격력
	soldier.will = 56;			//정신력
	soldier.def = 56;			//방어력
	soldier.agi = 51;			//순발력
	soldier.ten = 65;			//사기
	soldier.movingCount = 6;	//이동력
	vSoldier.push_back(soldier);

	//HP ProgressBar
	_Hp = new progressBar;
	_Hp->init("images/UI/Info/HP.bmp", "images/UI/Info/Back_P.bmp", 1056, 289, 84, 12);
	_Hp->setGauge(currentHp, maxHp);
	currentHp = maxHp = 125;

	//MP ProgressBar
	_Mp = new progressBar;
	_Mp->init("images/UI/Info/MP.bmp", "images/UI/Info/Back_P.bmp", 1056, 307, 84, 12);
	_Mp->setGauge(currentMp, maxMp);
	currentMp = maxMp = 35;

	//EXP ProgressBar
	_Exp = new progressBar;
	_Exp->init("images/UI/Info/EXP.bmp", "images/UI/Info/Back_EXP.bmp", 1095, 243, 45, 12);
	_Exp->setGauge(currentExp, maxExp);
	currentExp = 0;
	maxExp = 100;

	//캐릭터 방향 및 위치
	fDirection = FRIEND_LEFT;
	startTile = endTile = -1;
	speed = 4;	//속도

	isTurn = true;
	isSelect = false;

	return S_OK;
}

void Soldier::release()
{
	SAFE_DELETE(_Hp);
	SAFE_DELETE(_Mp);
	SAFE_DELETE(_Exp);
}

void Soldier::update()
{
	if (isTurn)
	{
		mouseMove();
	}

	friendAnimation();
	friendState();
}

void Soldier::render(HDC hdc)
{
	for (int k = 0; k < vSoldier.size(); k++)
	{
		if (isTurn)
		{
			if (isAtk)
			{
				vSoldier[k].atkImg->aniRender(hdc, vSoldier[k].rc.left - 8, vSoldier[k].rc.top - 8, friendAni);
			}
			else if (isHit)
			{
				vSoldier[k].blockImg->frameRender(hdc, vSoldier[k].rc.left, vSoldier[k].rc.top, 0, 4);

				HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
				HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
				SetTextColor(hdc, RGB(255, 255, 255));
				sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
				TextOut(hdc, vSoldier[k].rc.left, vSoldier[k].rc.top, str, strlen(str));
				SelectObject(hdc, oldFont);
				DeleteObject(myFont);
			}
			else
			{
				vSoldier[k].img->aniRender(hdc, vSoldier[k].rc.left, vSoldier[k].rc.top, friendAni);
			}
		}
		else
		{
			if (isHit)
			{
				vSoldier[k].blockImg->frameRender(hdc, vSoldier[k].rc.left, vSoldier[k].rc.top, 0, 4);

				HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
				HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
				SetTextColor(hdc, RGB(255, 255, 255));
				sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
				TextOut(hdc, vSoldier[k].rc.left, vSoldier[k].rc.top, str, strlen(str));
				SelectObject(hdc, oldFont);
				DeleteObject(myFont);
			}
			else
			{
				vSoldier[k].img->frameAlphaRender(hdc, vSoldier[k].rc.left, vSoldier[k].rc.top, 0, frameY, 100);
			}
		}
	}
}

void Soldier::mouseMove()
{
	for (int k = 0; k < vSoldier.size(); k++)
	{
		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			if (PtInRect(&vSoldier[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
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
					for (int j = 0; j < 8; j++)
					{
						vSoldier[k].rcAtk[0] = RectMake(vSoldier[k].rc.left - 48, vSoldier[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
						vSoldier[k].rcAtk[1] = RectMake(vSoldier[k].rc.left - 48, vSoldier[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
						vSoldier[k].rcAtk[2] = RectMake(vSoldier[k].rc.left - 48, vSoldier[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
						vSoldier[k].rcAtk[3] = RectMake(vSoldier[k].rc.left + 48, vSoldier[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
						vSoldier[k].rcAtk[4] = RectMake(vSoldier[k].rc.left + 48, vSoldier[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
						vSoldier[k].rcAtk[5] = RectMake(vSoldier[k].rc.left + 48, vSoldier[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
						vSoldier[k].rcAtk[6] = RectMake(vSoldier[k].rc.left, vSoldier[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
						vSoldier[k].rcAtk[7] = RectMake(vSoldier[k].rc.left, vSoldier[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
						atkList.push_back(vSoldier[k].rcAtk[j]);
					}

					//이동범위
					if (!isStop)
					{
						floodFill(startTile, vSoldier[k].movingCount);
					}
				}
			}

			if (!PtInRect(&vSoldier[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
			{
				if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
				{
					if (mainMap->getMap()[i].flood)
					{
						//선택한 맵의 x좌표와 y좌표
						mapX = mainMap->getMap()[i].rc.left + (mainMap->getMap()[i].rc.right - mainMap->getMap()[i].rc.left) / 2;
						mapY = mainMap->getMap()[i].rc.top + (mainMap->getMap()[i].rc.bottom - mainMap->getMap()[i].rc.top) / 2;
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
					else
					{
						isSelect = false;
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

		friendAstar();
		friendMenu();
		friendCollision();
	}
}

void Soldier::friendMove()
{
	for (int k = 0; k < vSoldier.size(); k++)
	{
		stackX = optimalPath.top().rc.left + (optimalPath.top().rc.right - optimalPath.top().rc.left) / 2;
		stackY = optimalPath.top().rc.top + (optimalPath.top().rc.bottom - optimalPath.top().rc.top) / 2;

		if (!isMove)
		{
			if (friendX > stackX)
			{
				fDirection = FRIEND_LEFT;
			}
			else if (friendX < stackX)
			{
				fDirection = FRIEND_RIGHT;
			}
			else if (friendY > stackY)
			{
				fDirection = FRIEND_UP;
			}
			else if (friendY < stackY)
			{
				fDirection = FRIEND_DOWN;
			}

			isMove = true;
		}

		if (vSoldier[k].rc.left > 0 || vSoldier[k].rc.right < WINSIZEY ||
			vSoldier[k].rc.top > 0 || vSoldier[k].rc.bottom < WINSIZEY)
		{
			switch (fDirection)
			{
			case FRIEND_LEFT:
				friendX -= speed;
				vSoldier[k].rc = RectMakeCenter(friendX, friendY, vSoldier[k].img->getFrameWidth(), vSoldier[k].img->getFrameHeight());
				break;
			case FRIEND_RIGHT:
				friendX += speed;
				vSoldier[k].rc = RectMakeCenter(friendX, friendY, vSoldier[k].img->getFrameWidth(), vSoldier[k].img->getFrameHeight());
				break;
			case FRIEND_UP:
				friendY -= speed;
				vSoldier[k].rc = RectMakeCenter(friendX, friendY, vSoldier[k].img->getFrameWidth(), vSoldier[k].img->getFrameHeight());
				break;
			case FRIEND_DOWN:
				friendY += speed;
				vSoldier[k].rc = RectMakeCenter(friendX, friendY, vSoldier[k].img->getFrameWidth(), vSoldier[k].img->getFrameHeight());
				break;
			}

			if (friendX == stackX && friendY == stackY)
			{
				isMove = false;
				optimalPath.pop();
			}
		}
	}
}

void Soldier::friendAstar()
{
	for (int k = 0; k < vSoldier.size(); k++)
	{
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
				friendMove();

			if (friendX == mapX && friendY == mapY)
			{
				isStop = true;
				isClick = true;

				//공격범위
				for (int j = 0; j < 8; j++)
				{
					vSoldier[k].rcAtk[0] = RectMake(vSoldier[k].rc.left - 48, vSoldier[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
					vSoldier[k].rcAtk[1] = RectMake(vSoldier[k].rc.left - 48, vSoldier[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					vSoldier[k].rcAtk[2] = RectMake(vSoldier[k].rc.left - 48, vSoldier[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					vSoldier[k].rcAtk[3] = RectMake(vSoldier[k].rc.left + 48, vSoldier[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
					vSoldier[k].rcAtk[4] = RectMake(vSoldier[k].rc.left + 48, vSoldier[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					vSoldier[k].rcAtk[5] = RectMake(vSoldier[k].rc.left + 48, vSoldier[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					vSoldier[k].rcAtk[6] = RectMake(vSoldier[k].rc.left, vSoldier[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					vSoldier[k].rcAtk[7] = RectMake(vSoldier[k].rc.left, vSoldier[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(vSoldier[k].rcAtk[j]);
				}

				//메뉴선택 렉트
				for (int j = 0; j < 5; j++)
				{
					rcMenu[0] = RectMake(vSoldier[k].rc.left - 97, vSoldier[k].rc.top - 30, 82, 20);
					rcMenu[1] = RectMake(vSoldier[k].rc.left - 97, vSoldier[k].rc.top - 9, 82, 20);
					rcMenu[2] = RectMake(vSoldier[k].rc.left - 97, vSoldier[k].rc.top + 12, 82, 20);
					rcMenu[3] = RectMake(vSoldier[k].rc.left - 97, vSoldier[k].rc.top + 38, 82, 20);
					rcMenu[4] = RectMake(vSoldier[k].rc.left - 97, vSoldier[k].rc.top + 63, 82, 20);
					menuList.push_back(rcMenu[j]);
				}
			}
		}
	}
}

void Soldier::friendMenu()
{
	for (int k = 0; k < vSoldier.size(); k++)
	{
		//메뉴
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
					isStop = false;
					isClick = false;
				}
				if (PtInRect(&rcMenu[4], m_ptMouse))	//취소
				{
					atkList.clear();
					menuList.clear();

					isSelect = false;
					isStop = false;
					isClick = false;
				}
			}
		}
	}
}

void Soldier::friendCollision()
{
	for (int k = 0; k < vSoldier.size(); k++)
	{
		RECT temp;

		if (IntersectRect(&temp, &vSoldier[k].rcAtk[0], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
			IntersectRect(&temp, &vSoldier[k].rcAtk[1], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
			IntersectRect(&temp, &vSoldier[k].rcAtk[2], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
			IntersectRect(&temp, &vSoldier[k].rcAtk[3], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc))
		{
			isTarget = true;
			frameX = 1;

			if (PtInRect(&ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc, m_ptMouse) &&
				KEYMANAGER->isStayKeyDown(VK_LBUTTON) && isAtkRng)
			{
				isAtkRng = false;
				isAtk = true;

				if (friendX > ENEMYMANAGER->getYeopo()->getEnemyX())
					fDirection = FRIEND_LEFT;
				else if (friendX < ENEMYMANAGER->getYeopo()->getEnemyX())
					fDirection = FRIEND_RIGHT;
				else if (friendY > ENEMYMANAGER->getYeopo()->getEnemyY())
					fDirection = FRIEND_UP;
				else if (friendY < ENEMYMANAGER->getYeopo()->getEnemyY())
					fDirection = FRIEND_DOWN;
			}
		}
		else
		{
			frameX = 0;
		}
	}
}

void Soldier::friendAnimation()
{
	if (isTurn)
	{
		if (isAtk)
		{
			switch (fDirection)
			{
			case FRIEND_LEFT:
				ANIMATIONMANAGER->addAnimation("leftAtk", "우군보병ATK", 12, 15, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("leftAtk");
				ANIMATIONMANAGER->resume("leftAtk");
				break;
			case FRIEND_RIGHT:
				ANIMATIONMANAGER->addAnimation("rightAtk", "우군보병ATK", 8, 11, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("rightAtk");
				ANIMATIONMANAGER->resume("rightAtk");
				break;
			case FRIEND_UP:
				ANIMATIONMANAGER->addAnimation("upAtk", "우군보병ATK", 4, 7, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("upAtk");
				ANIMATIONMANAGER->resume("upAtk");
				break;
			case FRIEND_DOWN:
				ANIMATIONMANAGER->addAnimation("downAtk", "우군보병ATK", 0, 3, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("downAtk");
				ANIMATIONMANAGER->resume("downAtk");
				break;
			}
		}
		else
		{
			switch (fDirection)
			{
			case FRIEND_LEFT:
				ANIMATIONMANAGER->addAnimation("friendLeft", "우군보병", 4, 5, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendLeft");
				ANIMATIONMANAGER->resume("friendLeft");
				break;
			case FRIEND_RIGHT:
				ANIMATIONMANAGER->addAnimation("friendRight", "우군보병", 6, 7, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendRight");
				ANIMATIONMANAGER->resume("friendRight");
				break;
			case FRIEND_UP:
				ANIMATIONMANAGER->addAnimation("friendUp", "우군보병", 2, 3, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendUp");
				ANIMATIONMANAGER->resume("friendUp");
				break;
			case FRIEND_DOWN:
				ANIMATIONMANAGER->addAnimation("friendDown", "우군보병", 0, 1, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendDown");
				ANIMATIONMANAGER->resume("friendDown");
				break;
			}
		}
	}
	else
	{
		switch (fDirection)
		{
		case FRIEND_LEFT:
			frameY = 10;
			break;
		case FRIEND_RIGHT:
			frameY = 11;
			break;
		case FRIEND_UP:
			frameY = 9;
			break;
		case FRIEND_DOWN:
			frameY = 8;
			break;
		}
	}
}

void Soldier::friendState()
{
	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);
}

void Soldier::setPosition(RECT rc)
{
	for (int k = 0; k < vSoldier.size(); k++)
	{
		vSoldier[k].rc = rc;
		friendX = vSoldier[k].rc.left + (vSoldier[k].rc.right - vSoldier[k].rc.left) / 2;
		friendY = vSoldier[k].rc.top + (vSoldier[k].rc.bottom - vSoldier[k].rc.top) / 2;
	}
}
