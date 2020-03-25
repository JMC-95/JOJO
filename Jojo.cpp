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
	//이름 및 얼굴
	jojo.name = "조조";
	jojo.face = "조조Face";
	jojo.className = "군웅";
	//이미지 및 애니메이션
	jojo.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//캐릭터 클릭시 이동범위 이미지
	jojo.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//캐릭터 클릭시 공격범위 이미지
	jojo.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//공격버튼 클릭시 공격범위 이미지
	jojo.img = IMAGEMANAGER->findImage(playerImg);			//캐릭터 이미지
	jojo.atkImg = IMAGEMANAGER->findImage(atkImg);			//공격 이미지
	jojo.blockImg = IMAGEMANAGER->findImage(blockImg);		//방어 및 피격 이미지
	//스테이터스
	jojo.level = 6;			//레벨
	jojo.hp = 132;			//체력
	jojo.mp = 36;			//마력
	jojo.atk = 59;			//공격력
	jojo.will = 64;			//정신력
	jojo.def = 67;			//방어력
	jojo.agi = 58;			//순발력
	jojo.ten = 60;			//사기
	jojo.movingCount = 6;	//이동력

	//HP ProgressBar
	_Hp = new progressBar;
	_Hp->init("images/UI/Info/HP.bmp", "images/UI/Info/Back_P.bmp", 1056, 289, 84, 12);
	_Hp->setGauge(currentHp, maxHp);
	currentHp = maxHp = 132;

	//MP ProgressBar
	_Mp = new progressBar;
	_Mp->init("images/UI/Info/MP.bmp", "images/UI/Info/Back_P.bmp", 1056, 307, 84, 12);
	_Mp->setGauge(currentMp, maxMp);
	currentMp = maxMp = 36;

	//EXP ProgressBar
	_Exp = new progressBar;
	_Exp->init("images/UI/Info/EXP.bmp", "images/UI/Info/Back_EXP.bmp", 1095, 243, 45, 12);
	_Exp->setGauge(currentExp, maxExp);
	currentExp = 11;
	maxExp = 100;

	//캐릭터 방향 및 위치
	pDirection = PLAYER_LEFT;
	startTile = endTile = -1;
	speed = 6;	//속도

	isTurn = true;
	isSelect = false;

	return S_OK;
}

void Jojo::release()
{
	SAFE_DELETE(_Hp);
	SAFE_DELETE(_Mp);
	SAFE_DELETE(_Exp);
}

void Jojo::update()
{
	if (isTurn)
	{
		if (!PLAYERMANAGER->getPlayer()[1]->getIsSelect() &&
			!PLAYERMANAGER->getPlayer()[2]->getIsSelect() &&
			!PLAYERMANAGER->getPlayer()[3]->getIsSelect() &&
			!PLAYERMANAGER->getPlayer()[4]->getIsSelect() &&
			!PLAYERMANAGER->getPlayer()[5]->getIsSelect() &&
			!PLAYERMANAGER->getPlayer()[6]->getIsSelect())
		{
			mouseMove();
		}
	}
	else
	{
		isHit = false;
	}

	playerAnimation();
	playerState();
}

void Jojo::render(HDC hdc)
{
	if (isTurn)
	{
		if (isAtk)
		{
			jojo.atkImg->aniRender(hdc, jojo.rc.left - 8, jojo.rc.top - 8, playerAni);
		}
		else if (isHit)
		{
			jojo.blockImg->frameRender(hdc, jojo.rc.left, jojo.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, jojo.rc.left, jojo.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			jojo.img->aniRender(hdc, jojo.rc.left, jojo.rc.top, playerAni);
		}
	}
	else
	{
		if (isHit)
		{
			jojo.blockImg->frameRender(hdc, jojo.rc.left, jojo.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, jojo.rc.left, jojo.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			jojo.img->frameAlphaRender(hdc, jojo.rc.left, jojo.rc.top, 0, frameY, 100);
		}
	}
}

void Jojo::mouseMove()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&jojo.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				//선택한 타일 (캐릭터)
				startTile = i;
				saveTile = startTile;
				sDirection = pDirection;

				isSelect = true;
				isFind = false;
				noPath = false;
				startAstar = false;

				//공격범위
				for (int j = 0; j < 4; j++)
				{
					jojo.rcAtk[0] = RectMake(jojo.rc.left - 48, jojo.rc.top, TILE_WIDTH, TILE_HEIGHT);
					jojo.rcAtk[1] = RectMake(jojo.rc.left + 48, jojo.rc.top, TILE_WIDTH, TILE_HEIGHT);
					jojo.rcAtk[2] = RectMake(jojo.rc.left, jojo.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					jojo.rcAtk[3] = RectMake(jojo.rc.left, jojo.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(jojo.rcAtk[j]);
				}

				//이동범위
				if (!isStop)
				{
					floodFill(startTile, jojo.movingCount);
				}
			}
		}

		if (!PtInRect(&jojo.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
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

	if (isSelect && KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		isStop = true;
		isClick = true;
	}

	playerAstar();
	playerMenu();
	playerCollision();
}

void Jojo::playerMove()
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

	if (jojo.rc.left > 0 || jojo.rc.right < WINSIZEY ||
		jojo.rc.top > 0 || jojo.rc.bottom < WINSIZEY)
	{
		switch (pDirection)
		{
		case PLAYER_LEFT:
			playerX -= speed;
			jojo.rc = RectMakeCenter(playerX, playerY, jojo.img->getFrameWidth(), jojo.img->getFrameHeight());
			break;
		case PLAYER_RIGHT:
			playerX += speed;
			jojo.rc = RectMakeCenter(playerX, playerY, jojo.img->getFrameWidth(), jojo.img->getFrameHeight());
			break;
		case PLAYER_UP:
			playerY -= speed;
			jojo.rc = RectMakeCenter(playerX, playerY, jojo.img->getFrameWidth(), jojo.img->getFrameHeight());
			break;
		case PLAYER_DOWN:
			playerY += speed;
			jojo.rc = RectMakeCenter(playerX, playerY, jojo.img->getFrameWidth(), jojo.img->getFrameHeight());
			break;
		}

		if (playerX == stackX && playerY == stackY)
		{
			isMove = false;
			optimalPath.pop();
		}
	}
}

void Jojo::playerAstar()
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
			playerMove();
		}

		if (playerX == mapX && playerY == mapY)
		{
			isStop = true;
			isClick = true;

			//공격범위
			for (int j = 0; j < 4; j++)
			{
				jojo.rcAtk[0] = RectMake(jojo.rc.left - 48, jojo.rc.top, TILE_WIDTH, TILE_HEIGHT);
				jojo.rcAtk[1] = RectMake(jojo.rc.left + 48, jojo.rc.top, TILE_WIDTH, TILE_HEIGHT);
				jojo.rcAtk[2] = RectMake(jojo.rc.left, jojo.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				jojo.rcAtk[3] = RectMake(jojo.rc.left, jojo.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				atkList.push_back(jojo.rcAtk[j]);
			}
		}

		if (isClick)
		{
			//메뉴선택 렉트
			for (int j = 0; j < 5; j++)
			{
				rcMenu[0] = RectMake(jojo.rc.left - 97, jojo.rc.top - 30, 82, 20);
				rcMenu[1] = RectMake(jojo.rc.left - 97, jojo.rc.top - 9, 82, 20);
				rcMenu[2] = RectMake(jojo.rc.left - 97, jojo.rc.top + 12, 82, 20);
				rcMenu[3] = RectMake(jojo.rc.left - 97, jojo.rc.top + 38, 82, 20);
				rcMenu[4] = RectMake(jojo.rc.left - 97, jojo.rc.top + 63, 82, 20);
				menuList.push_back(rcMenu[j]);
			}
		}
	}
}

void Jojo::playerMenu()
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

				auto& prevTile = mainMap->getMap()[saveTile];

				jojo.rc.left = prevTile.rc.left;
				jojo.rc.right = prevTile.rc.right;
				jojo.rc.top = prevTile.rc.top;
				jojo.rc.bottom = prevTile.rc.bottom;
				pDirection = sDirection;

				playerX = prevTile.rc.left + TILE_WIDTH * 0.5;
				playerY = prevTile.rc.top + TILE_HEIGHT * 0.5;

				isSelect = false;
				isStop = false;
				isClick = false;
			}
		}
	}
}

void Jojo::playerCollision()
{
	RECT temp;
	frameX = 0;

	for (int j = 0; j < ENEMYMANAGER->getEnemy().size(); j++)
	{
		auto enemyX = ENEMYMANAGER->getEnemy()[j]->getEnemyX();
		auto enemyY = ENEMYMANAGER->getEnemy()[j]->getEnemyY();
		auto& enemyRect = ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rc;

		bool isInterSect = false;

		for (int k = 0; k < 8; k++)
		{
			if (IntersectRect(&temp, &jojo.rcAtk[k], &enemyRect))
			{
				isInterSect = true;
				break;
			}
		}

		if (isInterSect)
		{
			isTarget = true;
			frameX = 1;

			if (PtInRect(&enemyRect, m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON) && isAtkRng)
			{
				isAtkRng = false;
				isAtk = true;
				isDamage = true;

				if (playerX > enemyX) pDirection = PLAYER_LEFT;
				else if (playerX < enemyX) pDirection = PLAYER_RIGHT;
				else if (playerY > enemyY) pDirection = PLAYER_UP;
				else if (playerY < enemyY) pDirection = PLAYER_DOWN;
			}
		}
	}
}

void Jojo::playerAnimation()
{
	if (isTurn)
	{
		if (isAtk)
		{
			switch (pDirection)
			{
			case PLAYER_LEFT:
				ANIMATIONMANAGER->addAnimation("leftAtk", "조조ATK", 12, 15, 5, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("leftAtk");
				ANIMATIONMANAGER->resume("leftAtk");
				break;
			case PLAYER_RIGHT:
				ANIMATIONMANAGER->addAnimation("rightAtk", "조조ATK", 8, 11, 5, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("rightAtk");
				ANIMATIONMANAGER->resume("rightAtk");
				break;
			case PLAYER_UP:
				ANIMATIONMANAGER->addAnimation("upAtk", "조조ATK", 4, 7, 5, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("upAtk");
				ANIMATIONMANAGER->resume("upAtk");
				break;
			case PLAYER_DOWN:
				ANIMATIONMANAGER->addAnimation("downAtk", "조조ATK", 0, 3, 5, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("downAtk");
				ANIMATIONMANAGER->resume("downAtk");
				break;
			}
		}
		else
		{
			switch (pDirection)
			{
			case PLAYER_LEFT:
				ANIMATIONMANAGER->addAnimation("playerLeft", "조조", 4, 5, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerLeft");
				ANIMATIONMANAGER->resume("playerLeft");
				break;
			case PLAYER_RIGHT:
				ANIMATIONMANAGER->addAnimation("playerRight", "조조", 6, 7, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerRight");
				ANIMATIONMANAGER->resume("playerRight");
				break;
			case PLAYER_UP:
				ANIMATIONMANAGER->addAnimation("playerUp", "조조", 2, 3, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerUp");
				ANIMATIONMANAGER->resume("playerUp");
				break;
			case PLAYER_DOWN:
				ANIMATIONMANAGER->addAnimation("playerDown", "조조", 0, 1, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerDown");
				ANIMATIONMANAGER->resume("playerDown");
				break;
			}
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

void Jojo::playerState()
{
	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);
}

void Jojo::setPosition(RECT rc)
{
	jojo.rc = rc;
	playerX = jojo.rc.left + (jojo.rc.right - jojo.rc.left) / 2;
	playerY = jojo.rc.top + (jojo.rc.bottom - jojo.rc.top) / 2;
}
