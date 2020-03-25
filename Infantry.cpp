#include "stdafx.h"
#include "Infantry.h"

Infantry::Infantry(int num)
{
	number = num;
}

Infantry::~Infantry()
{
}

HRESULT Infantry::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * enemyImg, const char * atkImg, const char * blockImg)
{
	//이름 및 얼굴
	infantry.name = "적보병";
	infantry.face = "적보병Face";
	infantry.className = "경보병";
	//이미지 및 애니메이션
	infantry.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//캐릭터 클릭시 이동범위 이미지
	infantry.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//캐릭터 클릭시 공격범위 이미지
	infantry.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//공격버튼 클릭시 공격범위 이미지
	infantry.img = IMAGEMANAGER->findImage(enemyImg);			//캐릭터 이미지
	infantry.atkImg = IMAGEMANAGER->findImage(atkImg);			//공격 이미지
	infantry.blockImg = IMAGEMANAGER->findImage(blockImg);		//방어 및 피격 이미지
	//스테이터스
	infantry.level = 2;			//레벨
	infantry.hp = 116;			//체력
	infantry.mp = 11;			//마력
	infantry.atk = 36;			//공격력
	infantry.will = 38;			//정신력
	infantry.def = 43;			//방어력
	infantry.agi = 32;			//순발력
	infantry.ten = 32;			//사기
	infantry.movingCount = 4;	//이동력

	//HP ProgressBar
	_Hp = new progressBar;
	_Hp->init("images/UI/Info/HP.bmp", "images/UI/Info/Back_P.bmp", 1056, 289, 84, 12);
	_Hp->setGauge(currentHp, maxHp);
	currentHp = maxHp = 116;

	//MP ProgressBar
	_Mp = new progressBar;
	_Mp->init("images/UI/Info/MP.bmp", "images/UI/Info/Back_P.bmp", 1056, 307, 84, 12);
	_Mp->setGauge(currentMp, maxMp);
	currentMp = maxMp = 11;

	//EXP ProgressBar
	_Exp = new progressBar;
	_Exp->init("images/UI/Info/EXP.bmp", "images/UI/Info/Back_EXP.bmp", 1095, 243, 45, 12);
	_Exp->setGauge(currentExp, maxExp);
	currentExp = 0;
	maxExp = 100;

	//캐릭터 방향 및 위치
	eDirection = ENEMY_RIGHT;
	startTile = endTile = -1;
	speed = 6;	//속도

	isTurn = true;
	isSelect = false;

	return S_OK;
}

void Infantry::release()
{
	SAFE_DELETE(_Hp);
	SAFE_DELETE(_Mp);
	SAFE_DELETE(_Exp);
}

void Infantry::update()
{
	if (isTurn)
	{
		mouseMove();
	}

	enemyAnimation();
	enemyState();
}

void Infantry::render(HDC hdc)
{
	if (isTurn)
	{
		if (isAtk)
		{
			infantry.atkImg->aniRender(hdc, infantry.rc.left - 8, infantry.rc.top - 8, enemyAni);
		}
		else if (isHit)
		{
			infantry.blockImg->frameRender(hdc, infantry.rc.left, infantry.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, infantry.rc.left, infantry.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			infantry.img->aniRender(hdc, infantry.rc.left, infantry.rc.top, enemyAni);
		}
	}
	else
	{
		if (isHit)
		{
			infantry.blockImg->frameRender(hdc, infantry.rc.left, infantry.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, infantry.rc.left, infantry.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			infantry.img->frameAlphaRender(hdc, infantry.rc.left, infantry.rc.top, 0, frameY, 100);
		}
	}
}

void Infantry::mouseMove()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&infantry.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
		{
			if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
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
					infantry.rcAtk[0] = RectMake(infantry.rc.left - 48, infantry.rc.top, TILE_WIDTH, TILE_HEIGHT);
					infantry.rcAtk[1] = RectMake(infantry.rc.left - 48, infantry.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					infantry.rcAtk[2] = RectMake(infantry.rc.left - 48, infantry.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					infantry.rcAtk[3] = RectMake(infantry.rc.left + 48, infantry.rc.top, TILE_WIDTH, TILE_HEIGHT);
					infantry.rcAtk[4] = RectMake(infantry.rc.left + 48, infantry.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					infantry.rcAtk[5] = RectMake(infantry.rc.left + 48, infantry.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					infantry.rcAtk[6] = RectMake(infantry.rc.left, infantry.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					infantry.rcAtk[7] = RectMake(infantry.rc.left, infantry.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(infantry.rcAtk[j]);
				}

				//이동범위
				if (!isStop)
				{
					floodFill(startTile, infantry.movingCount);
				}
			}
		}

		if (!PtInRect(&infantry.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
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

	enemyAstar();
	enemyMenu();
	enemyCollision();
}

void Infantry::enemyMove()
{
	stackX = optimalPath.top().rc.left + (optimalPath.top().rc.right - optimalPath.top().rc.left) / 2;
	stackY = optimalPath.top().rc.top + (optimalPath.top().rc.bottom - optimalPath.top().rc.top) / 2;

	if (!isMove)
	{
		if (enemyX > stackX)
		{
			eDirection = ENEMY_LEFT;
		}
		else if (enemyX < stackX)
		{
			eDirection = ENEMY_RIGHT;
		}
		else if (enemyY > stackY)
		{
			eDirection = ENEMY_UP;
		}
		else if (enemyY < stackY)
		{
			eDirection = ENEMY_DOWN;
		}

		isMove = true;
	}

	if (infantry.rc.left > 0 || infantry.rc.right < WINSIZEY ||
		infantry.rc.top > 0 || infantry.rc.bottom < WINSIZEY)
	{
		switch (eDirection)
		{
		case ENEMY_LEFT:
			enemyX -= speed;
			infantry.rc = RectMakeCenter(enemyX, enemyY, infantry.img->getFrameWidth(), infantry.img->getFrameHeight());
			break;
		case ENEMY_RIGHT:
			enemyX += speed;
			infantry.rc = RectMakeCenter(enemyX, enemyY, infantry.img->getFrameWidth(), infantry.img->getFrameHeight());
			break;
		case ENEMY_UP:
			enemyY -= speed;
			infantry.rc = RectMakeCenter(enemyX, enemyY, infantry.img->getFrameWidth(), infantry.img->getFrameHeight());
			break;
		case ENEMY_DOWN:
			enemyY += speed;
			infantry.rc = RectMakeCenter(enemyX, enemyY, infantry.img->getFrameWidth(), infantry.img->getFrameHeight());
			break;
		}

		if (enemyX == stackX && enemyY == stackY)
		{
			isMove = false;
			optimalPath.pop();
		}
	}
}

void Infantry::enemyAstar()
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
		{
			enemyMove();
		}

		if (enemyX == mapX && enemyY == mapY)
		{
			isStop = true;
			isClick = true;

			//공격범위
			for (int j = 0; j < 8; j++)
			{
				infantry.rcAtk[0] = RectMake(infantry.rc.left - 48, infantry.rc.top, TILE_WIDTH, TILE_HEIGHT);
				infantry.rcAtk[1] = RectMake(infantry.rc.left - 48, infantry.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				infantry.rcAtk[2] = RectMake(infantry.rc.left - 48, infantry.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				infantry.rcAtk[3] = RectMake(infantry.rc.left + 48, infantry.rc.top, TILE_WIDTH, TILE_HEIGHT);
				infantry.rcAtk[4] = RectMake(infantry.rc.left + 48, infantry.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				infantry.rcAtk[5] = RectMake(infantry.rc.left + 48, infantry.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				infantry.rcAtk[6] = RectMake(infantry.rc.left, infantry.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				infantry.rcAtk[7] = RectMake(infantry.rc.left, infantry.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				atkList.push_back(infantry.rcAtk[j]);
			}

			//메뉴선택 렉트
			for (int j = 0; j < 5; j++)
			{
				rcMenu[0] = RectMake(infantry.rc.left - 97, infantry.rc.top - 30, 82, 20);
				rcMenu[1] = RectMake(infantry.rc.left - 97, infantry.rc.top - 9, 82, 20);
				rcMenu[2] = RectMake(infantry.rc.left - 97, infantry.rc.top + 12, 82, 20);
				rcMenu[3] = RectMake(infantry.rc.left - 97, infantry.rc.top + 38, 82, 20);
				rcMenu[4] = RectMake(infantry.rc.left - 97, infantry.rc.top + 63, 82, 20);
				menuList.push_back(rcMenu[j]);
			}
		}
	}
}

void Infantry::enemyAnimation()
{
	if (isTurn)
	{
		if (isAtk)
		{
			switch (eDirection)
			{
			case ENEMY_LEFT:
				ANIMATIONMANAGER->addAnimation("leftAtk", "적보병ATK", 12, 15, 5, false, false);
				enemyAni = ANIMATIONMANAGER->findAnimation("leftAtk");
				ANIMATIONMANAGER->resume("leftAtk");
				break;
			case ENEMY_RIGHT:
				ANIMATIONMANAGER->addAnimation("rightAtk", "적보병ATK", 8, 11, 5, false, false);
				enemyAni = ANIMATIONMANAGER->findAnimation("rightAtk");
				ANIMATIONMANAGER->resume("rightAtk");
				break;
			case ENEMY_UP:
				ANIMATIONMANAGER->addAnimation("upAtk", "적보병ATK", 4, 7, 5, false, false);
				enemyAni = ANIMATIONMANAGER->findAnimation("upAtk");
				ANIMATIONMANAGER->resume("upAtk");
				break;
			case ENEMY_DOWN:
				ANIMATIONMANAGER->addAnimation("downAtk", "적보병ATK", 0, 3, 5, false, false);
				enemyAni = ANIMATIONMANAGER->findAnimation("downAtk");
				ANIMATIONMANAGER->resume("downAtk");
				break;
			}
		}
		else
		{
			switch (eDirection)
			{
			case ENEMY_LEFT:
				ANIMATIONMANAGER->addAnimation("enemyLeft", "적보병", 4, 5, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyLeft");
				ANIMATIONMANAGER->resume("enemyLeft");
				break;
			case ENEMY_RIGHT:
				ANIMATIONMANAGER->addAnimation("enemyRight", "적보병", 6, 7, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyRight");
				ANIMATIONMANAGER->resume("enemyRight");
				break;
			case ENEMY_UP:
				ANIMATIONMANAGER->addAnimation("enemyUp", "적보병", 2, 3, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyUp");
				ANIMATIONMANAGER->resume("enemyUp");
				break;
			case ENEMY_DOWN:
				ANIMATIONMANAGER->addAnimation("enemyDown", "적보병", 0, 1, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyDown");
				ANIMATIONMANAGER->resume("enemyDown");
				break;
			}
		}
	}
	else
	{
		switch (eDirection)
		{
		case ENEMY_LEFT:
			frameY = 10;
			break;
		case ENEMY_RIGHT:
			frameY = 11;
			break;
		case ENEMY_UP:
			frameY = 9;
			break;
		case ENEMY_DOWN:
			frameY = 8;
			break;
		}
	}
}

void Infantry::enemyState()
{
	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);
}

void Infantry::enemyMenu()
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

void Infantry::enemyCollision()
{
}

void Infantry::setPosition(RECT rc)
{
	infantry.rc = rc;
	enemyX = infantry.rc.left + (infantry.rc.right - infantry.rc.left) / 2;
	enemyY = infantry.rc.top + (infantry.rc.bottom - infantry.rc.top) / 2;
}
