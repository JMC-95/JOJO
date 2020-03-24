#include "stdafx.h"
#include "Gwanu.h"

Gwanu::Gwanu()
{
}

Gwanu::~Gwanu()
{
}

HRESULT Gwanu::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * friendImg, const char * atkImg, const char * blockImg)
{
	//구조체 정보 기입
	FriendInfo gwanu;
	//이미지 및 애니메이션
	gwanu.moveRngImg = IMAGEMANAGER->findImage(moveImg);	//캐릭터 클릭시 이동범위 이미지
	gwanu.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//캐릭터 클릭시 공격범위 이미지
	gwanu.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//공격버튼 클릭시 공격범위 이미지
	gwanu.img = IMAGEMANAGER->findImage(friendImg);			//캐릭터 이미지
	gwanu.atkImg = IMAGEMANAGER->findImage(atkImg);			//공격 이미지
	gwanu.blockImg = IMAGEMANAGER->findImage(blockImg);		//방어 및 피격 이미지
	//스테이터스
	gwanu.level = 6;		//레벨
	gwanu.hp = 132;			//체력
	gwanu.mp = 17;			//마력
	gwanu.atk = 70;			//공격력
	gwanu.will = 44;		//정신력
	gwanu.def = 57;			//방어력
	gwanu.agi = 61;			//순발력
	gwanu.ten = 61;			//사기
	gwanu.movingCount = 6;	//이동력
	vGwanu.push_back(gwanu);

	//HP ProgressBar
	_Hp = new progressBar;
	_Hp->init("images/UI/Info/HP.bmp", "images/UI/Info/Back_P.bmp", 1056, 289, 84, 12);
	_Hp->setGauge(currentHp, maxHp);
	currentHp = maxHp = 132;

	//MP ProgressBar
	_Mp = new progressBar;
	_Mp->init("images/UI/Info/MP.bmp", "images/UI/Info/Back_P.bmp", 1056, 307, 84, 12);
	_Mp->setGauge(currentMp, maxMp);
	currentMp = maxMp = 17;

	//EXP ProgressBar
	_Exp = new progressBar;
	_Exp->init("images/UI/Info/EXP.bmp", "images/UI/Info/Back_EXP.bmp", 1095, 243, 45, 12);
	_Exp->setGauge(currentExp, maxExp);
	currentExp = 0;
	maxExp = 100;

	//캐릭터 방향 및 위치
	fDirection = FRIEND_LEFT;
	startTile = endTile = -1;
	speed = 6;	//속도

	isTurn = true;
	isSelect = false;

	return S_OK;
}

void Gwanu::release()
{
	SAFE_DELETE(_Hp);
	SAFE_DELETE(_Mp);
	SAFE_DELETE(_Exp);
}

void Gwanu::update()
{
	if (isTurn)
	{
		mouseMove();
	}

	friendAnimation();
	friendState();
}

void Gwanu::render(HDC hdc)
{
	for (int k = 0; k < vGwanu.size(); k++)
	{
		if (isTurn)
		{
			if (isAtk)
			{
				vGwanu[k].atkImg->aniRender(hdc, vGwanu[k].rc.left - 8, vGwanu[k].rc.top - 8, friendAni);
			}
			else if (isHit)
			{
				vGwanu[k].blockImg->frameRender(hdc, vGwanu[k].rc.left, vGwanu[k].rc.top, 0, 4);

				HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
				HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
				SetTextColor(hdc, RGB(255, 255, 255));
				sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
				TextOut(hdc, vGwanu[k].rc.left, vGwanu[k].rc.top, str, strlen(str));
				SelectObject(hdc, oldFont);
				DeleteObject(myFont);
			}
			else
			{
				vGwanu[k].img->aniRender(hdc, vGwanu[k].rc.left, vGwanu[k].rc.top, friendAni);
			}
		}
		else
		{
			if (isHit)
			{
				vGwanu[k].blockImg->frameRender(hdc, vGwanu[k].rc.left, vGwanu[k].rc.top, 0, 4);

				HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
				HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
				SetTextColor(hdc, RGB(255, 255, 255));
				sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
				TextOut(hdc, vGwanu[k].rc.left, vGwanu[k].rc.top, str, strlen(str));
				SelectObject(hdc, oldFont);
				DeleteObject(myFont);
			}
			else
			{
				vGwanu[k].img->frameAlphaRender(hdc, vGwanu[k].rc.left, vGwanu[k].rc.top, 0, frameY, 100);
			}
		}
	}
}

void Gwanu::mouseMove()
{
	for (int k = 0; k < vGwanu.size(); k++)
	{
		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			if (PtInRect(&vGwanu[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
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
						vGwanu[k].rcAtk[0] = RectMake(vGwanu[k].rc.left - 48, vGwanu[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
						vGwanu[k].rcAtk[1] = RectMake(vGwanu[k].rc.left + 48, vGwanu[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
						vGwanu[k].rcAtk[2] = RectMake(vGwanu[k].rc.left, vGwanu[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
						vGwanu[k].rcAtk[3] = RectMake(vGwanu[k].rc.left, vGwanu[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
						atkList.push_back(vGwanu[k].rcAtk[j]);
					}

					//이동범위
					if (!isStop)
					{
						floodFill(startTile, vGwanu[k].movingCount);
					}
				}
			}

			if (!PtInRect(&vGwanu[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
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

void Gwanu::friendMove()
{
	for (int k = 0; k < vGwanu.size(); k++)
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

		if (vGwanu[k].rc.left > 0 || vGwanu[k].rc.right < WINSIZEY ||
			vGwanu[k].rc.top > 0 || vGwanu[k].rc.bottom < WINSIZEY)
		{
			switch (fDirection)
			{
			case FRIEND_LEFT:
				friendX -= speed;
				vGwanu[k].rc = RectMakeCenter(friendX, friendY, vGwanu[k].img->getFrameWidth(), vGwanu[k].img->getFrameHeight());
				break;
			case FRIEND_RIGHT:
				friendX += speed;
				vGwanu[k].rc = RectMakeCenter(friendX, friendY, vGwanu[k].img->getFrameWidth(), vGwanu[k].img->getFrameHeight());
				break;
			case FRIEND_UP:
				friendY -= speed;
				vGwanu[k].rc = RectMakeCenter(friendX, friendY, vGwanu[k].img->getFrameWidth(), vGwanu[k].img->getFrameHeight());
				break;
			case FRIEND_DOWN:
				friendY += speed;
				vGwanu[k].rc = RectMakeCenter(friendX, friendY, vGwanu[k].img->getFrameWidth(), vGwanu[k].img->getFrameHeight());
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

void Gwanu::friendAstar()
{
	for (int k = 0; k < vGwanu.size(); k++)
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
				for (int j = 0; j < 4; j++)
				{
					vGwanu[k].rcAtk[0] = RectMake(vGwanu[k].rc.left - 48, vGwanu[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
					vGwanu[k].rcAtk[1] = RectMake(vGwanu[k].rc.left + 48, vGwanu[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
					vGwanu[k].rcAtk[2] = RectMake(vGwanu[k].rc.left, vGwanu[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					vGwanu[k].rcAtk[3] = RectMake(vGwanu[k].rc.left, vGwanu[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(vGwanu[k].rcAtk[j]);
				}

				//메뉴선택 렉트
				for (int j = 0; j < 5; j++)
				{
					rcMenu[0] = RectMake(vGwanu[k].rc.left - 97, vGwanu[k].rc.top - 30, 82, 20);
					rcMenu[1] = RectMake(vGwanu[k].rc.left - 97, vGwanu[k].rc.top - 9, 82, 20);
					rcMenu[2] = RectMake(vGwanu[k].rc.left - 97, vGwanu[k].rc.top + 12, 82, 20);
					rcMenu[3] = RectMake(vGwanu[k].rc.left - 97, vGwanu[k].rc.top + 38, 82, 20);
					rcMenu[4] = RectMake(vGwanu[k].rc.left - 97, vGwanu[k].rc.top + 63, 82, 20);
					menuList.push_back(rcMenu[j]);
				}
			}
		}
	}
}

void Gwanu::friendMenu()
{
	for (int k = 0; k < vGwanu.size(); k++)
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

void Gwanu::friendCollision()
{
	for (int k = 0; k < vGwanu.size(); k++)
	{
		RECT temp;

		if (IntersectRect(&temp, &vGwanu[k].rcAtk[0], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
			IntersectRect(&temp, &vGwanu[k].rcAtk[1], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
			IntersectRect(&temp, &vGwanu[k].rcAtk[2], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
			IntersectRect(&temp, &vGwanu[k].rcAtk[3], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc))
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

void Gwanu::friendAnimation()
{
	if (isTurn)
	{
		if (isAtk)
		{
			switch (fDirection)
			{
			case FRIEND_LEFT:
				ANIMATIONMANAGER->addAnimation("leftAtk", "관우ATK", 12, 15, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("leftAtk");
				ANIMATIONMANAGER->resume("leftAtk");
				break;
			case FRIEND_RIGHT:
				ANIMATIONMANAGER->addAnimation("rightAtk", "관우ATK", 8, 11, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("rightAtk");
				ANIMATIONMANAGER->resume("rightAtk");
				break;
			case FRIEND_UP:
				ANIMATIONMANAGER->addAnimation("upAtk", "관우ATK", 4, 7, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("upAtk");
				ANIMATIONMANAGER->resume("upAtk");
				break;
			case FRIEND_DOWN:
				ANIMATIONMANAGER->addAnimation("downAtk", "관우ATK", 0, 3, 5, false, false);
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
				ANIMATIONMANAGER->addAnimation("friendLeft", "관우", 4, 5, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendLeft");
				ANIMATIONMANAGER->resume("friendLeft");
				break;
			case FRIEND_RIGHT:
				ANIMATIONMANAGER->addAnimation("friendRight", "관우", 6, 7, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendRight");
				ANIMATIONMANAGER->resume("friendRight");
				break;
			case FRIEND_UP:
				ANIMATIONMANAGER->addAnimation("friendUp", "관우", 2, 3, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendUp");
				ANIMATIONMANAGER->resume("friendUp");
				break;
			case FRIEND_DOWN:
				ANIMATIONMANAGER->addAnimation("friendDown", "관우", 0, 1, 2, false, true);
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

void Gwanu::friendState()
{
	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);
}

void Gwanu::setPosition(RECT rc)
{
	for (int k = 0; k < vGwanu.size(); k++)
	{
		vGwanu[k].rc = rc;
		friendX = vGwanu[k].rc.left + (vGwanu[k].rc.right - vGwanu[k].rc.left) / 2;
		friendY = vGwanu[k].rc.top + (vGwanu[k].rc.bottom - vGwanu[k].rc.top) / 2;
	}
}
