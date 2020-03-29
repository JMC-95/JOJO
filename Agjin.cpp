#include "stdafx.h"
#include "Agjin.h"

Agjin::Agjin()
{
}

Agjin::~Agjin()
{
}

HRESULT Agjin::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * playerImg, const char * atkImg, const char * blockImg)
{
	//이름 및 얼굴
	agjin.name = "악진";
	agjin.face = "악진Face";
	agjin.className = "경보병";
	//이미지 및 애니메이션
	agjin.moveRngImg = IMAGEMANAGER->findImage(moveImg);	//캐릭터 클릭시 이동범위 이미지
	agjin.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//캐릭터 클릭시 공격범위 이미지
	agjin.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//공격버튼 클릭시 공격범위 이미지
	agjin.img = IMAGEMANAGER->findImage(playerImg);			//캐릭터 이미지
	agjin.atkImg = IMAGEMANAGER->findImage(atkImg);			//공격 이미지
	agjin.blockImg = IMAGEMANAGER->findImage(blockImg);		//방어 및 피격 이미지
	//스테이터스
	agjin.level = 4;		//레벨
	agjin.hp = 142;			//체력
	agjin.mp = 19;			//마력
	agjin.atk = 47;			//공격력
	agjin.will = 50;		//정신력
	agjin.def = 62;			//방어력
	agjin.agi = 35;			//순발력
	agjin.ten = 51;			//사기
	agjin.movingCount = 4;	//이동력

	//HP ProgressBar
	_Hp = new progressBar;
	_Hp->init("images/UI/Info/HP.bmp", "images/UI/Info/Back_P.bmp", 1056, 289, 84, 12);
	_Hp->setGauge(currentHp, maxHp);
	currentHp = maxHp = 142;

	//MP ProgressBar
	_Mp = new progressBar;
	_Mp->init("images/UI/Info/MP.bmp", "images/UI/Info/Back_P.bmp", 1056, 307, 84, 12);
	_Mp->setGauge(currentMp, maxMp);
	currentMp = maxMp = 19;

	//EXP ProgressBar
	_Exp = new progressBar;
	_Exp->init("images/UI/Info/EXP.bmp", "images/UI/Info/Back_EXP.bmp", 1095, 243, 45, 12);
	_Exp->setGauge(currentExp, maxExp);
	currentExp = 52;
	maxExp = 100;

	//캐릭터 방향 및 위치
	pDirection = PLAYER_LEFT;
	startTile = endTile = -1;
	speed = 12;	//속도

	isTurn = true;
	isMove = true;
	isSelect = false;

	return S_OK;
}

void Agjin::release()
{
}

void Agjin::update()
{
	if (isTurn)
	{
		if (!PLAYERMANAGER->getPlayer()[0]->getIsSelect() &&
			!PLAYERMANAGER->getPlayer()[1]->getIsSelect() &&
			!PLAYERMANAGER->getPlayer()[2]->getIsSelect() &&
			!PLAYERMANAGER->getPlayer()[3]->getIsSelect() &&
			!PLAYERMANAGER->getPlayer()[5]->getIsSelect() &&
			!PLAYERMANAGER->getPlayer()[6]->getIsSelect())
		{
			mouseMove();
		}
	}

	playerAnimation();
	playerState();
}

void Agjin::render(HDC hdc)
{
	if (isTurn)
	{
		if (isAtk)
		{
			agjin.atkImg->aniRender(hdc, agjin.rc.left - 8, agjin.rc.top - 8, playerAni);
		}
		else if (isHit)
		{
			agjin.blockImg->frameRender(hdc, agjin.rc.left, agjin.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, agjin.rc.left, agjin.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			agjin.img->aniRender(hdc, agjin.rc.left, agjin.rc.top, playerAni);
		}
	}
	else
	{
		if (isHit)
		{
			agjin.blockImg->frameRender(hdc, agjin.rc.left, agjin.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, agjin.rc.left, agjin.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			agjin.img->frameAlphaRender(hdc, agjin.rc.left, agjin.rc.top, 0, frameY, 100);
		}
	}
}

void Agjin::mouseMove()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&agjin.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				SOUNDMANAGER->play("click", 1.0f);

				//선택한 타일 (캐릭터)
				startTile = i;
				saveTile = startTile;
				sDirection = pDirection;

				isSelect = true;
				isFind = false;
				noPath = false;
				startAstar = false;

				//공격범위
				for (int j = 0; j < 8; j++)
				{
					agjin.rcAtk[0] = RectMake(agjin.rc.left - 48, agjin.rc.top, TILE_WIDTH, TILE_HEIGHT);
					agjin.rcAtk[1] = RectMake(agjin.rc.left - 48, agjin.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					agjin.rcAtk[2] = RectMake(agjin.rc.left - 48, agjin.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					agjin.rcAtk[3] = RectMake(agjin.rc.left + 48, agjin.rc.top, TILE_WIDTH, TILE_HEIGHT);
					agjin.rcAtk[4] = RectMake(agjin.rc.left + 48, agjin.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					agjin.rcAtk[5] = RectMake(agjin.rc.left + 48, agjin.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					agjin.rcAtk[6] = RectMake(agjin.rc.left, agjin.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					agjin.rcAtk[7] = RectMake(agjin.rc.left, agjin.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(agjin.rcAtk[j]);
				}

				//이동범위
				if (isMove)
				{
					floodFill(startTile, agjin.movingCount);

					for (int j = 0; j < TILE_X * TILE_Y; j++)
					{
						RECT temp;
						auto& rc = mainMap->getMap()[j].rc;

						if (mainMap->getMap()[j].flood)
						{
							for (int k = 0; k < ENEMYMANAGER->getEnemy().size(); ++k)
							{
								auto& enemyRect = ENEMYMANAGER->getEnemy()[k]->getEnemyInfo().rc;

								if (IntersectRect(&temp, &enemyRect, &rc))
								{
									mainMap->getMap()[j].flood = false;
								}
							}
						}
					}
				}
			}
		}

		if (!PtInRect(&agjin.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				if (mainMap->getMap()[i].flood)
				{
					SOUNDMANAGER->play("move", 1.0f);

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
					SOUNDMANAGER->play("clickMiss", 1.0f);
					isSelect = false;
				}

				for (int j = 0; j < TILE_X * TILE_Y; j++)
				{
					if (mainMap->getMap()[j].flood)
					{
						mainMap->getMap()[j].flood = false;
					}
				}
			}
		}
	}

	if (isSelect && KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		isClick = true;
		isMove = false;
		isSelect = false;
	}

	playerAstar();
	playerCollision();
	playerMenu();
}

void Agjin::playerMove()
{
	stackX = optimalPath.top().rc.left + (optimalPath.top().rc.right - optimalPath.top().rc.left) / 2;
	stackY = optimalPath.top().rc.top + (optimalPath.top().rc.bottom - optimalPath.top().rc.top) / 2;

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

	if (agjin.rc.left > 0 || agjin.rc.right < WINSIZEY ||
		agjin.rc.top > 0 || agjin.rc.bottom < WINSIZEY)
	{
		switch (pDirection)
		{
		case PLAYER_LEFT:
			playerX -= speed;
			agjin.rc = RectMakeCenter(playerX, playerY, agjin.img->getFrameWidth(), agjin.img->getFrameHeight());
			break;
		case PLAYER_RIGHT:
			playerX += speed;
			agjin.rc = RectMakeCenter(playerX, playerY, agjin.img->getFrameWidth(), agjin.img->getFrameHeight());
			break;
		case PLAYER_UP:
			playerY -= speed;
			agjin.rc = RectMakeCenter(playerX, playerY, agjin.img->getFrameWidth(), agjin.img->getFrameHeight());
			break;
		case PLAYER_DOWN:
			playerY += speed;
			agjin.rc = RectMakeCenter(playerX, playerY, agjin.img->getFrameWidth(), agjin.img->getFrameHeight());
			break;
		}

		if (playerX == stackX && playerY == stackY)
		{
			optimalPath.pop();
		}
	}
}

void Agjin::playerAstar()
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
		if (isMove)
		{
			playerMove();
		}

		if (playerX == mapX && playerY == mapY)
		{
			isMove = false;
			isClick = true;

			//공격범위
			for (int j = 0; j < 8; j++)
			{
				agjin.rcAtk[0] = RectMake(agjin.rc.left - 48, agjin.rc.top, TILE_WIDTH, TILE_HEIGHT);
				agjin.rcAtk[1] = RectMake(agjin.rc.left - 48, agjin.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				agjin.rcAtk[2] = RectMake(agjin.rc.left - 48, agjin.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				agjin.rcAtk[3] = RectMake(agjin.rc.left + 48, agjin.rc.top, TILE_WIDTH, TILE_HEIGHT);
				agjin.rcAtk[4] = RectMake(agjin.rc.left + 48, agjin.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				agjin.rcAtk[5] = RectMake(agjin.rc.left + 48, agjin.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				agjin.rcAtk[6] = RectMake(agjin.rc.left, agjin.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				agjin.rcAtk[7] = RectMake(agjin.rc.left, agjin.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				atkList.push_back(agjin.rcAtk[j]);
			}
		}
	}

	if (isClick)
	{
		//메뉴선택 렉트
		for (int j = 0; j < 5; j++)
		{
			rcMenu[0] = RectMake(agjin.rc.left - 97, agjin.rc.top - 30, 82, 20);
			rcMenu[1] = RectMake(agjin.rc.left - 97, agjin.rc.top - 9, 82, 20);
			rcMenu[2] = RectMake(agjin.rc.left - 97, agjin.rc.top + 12, 82, 20);
			rcMenu[3] = RectMake(agjin.rc.left - 97, agjin.rc.top + 38, 82, 20);
			rcMenu[4] = RectMake(agjin.rc.left - 97, agjin.rc.top + 63, 82, 20);
			menuList.push_back(rcMenu[j]);
		}
	}
}

void Agjin::playerCollision()
{
	RECT temp;
	frameX = 0;

	for (int j = 0; j < ENEMYMANAGER->getEnemy().size(); j++)
	{
		auto enemyX = ENEMYMANAGER->getEnemy()[j]->getEnemyX();
		auto enemyY = ENEMYMANAGER->getEnemy()[j]->getEnemyY();
		auto enemyHit = ENEMYMANAGER->getEnemy()[j]->getIsHit();
		auto& enemyRect = ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rc;

		for (int k = 0; k < 8; k++)
		{
			if (IntersectRect(&temp, &agjin.rcAtk[k], &enemyRect))
			{
				isTarget = true;
				frameX = 1;
			}
		}

		if (enemyHit)
		{
			if (playerX > enemyX) pDirection = PLAYER_LEFT;
			else if (playerX < enemyX) pDirection = PLAYER_RIGHT;
			else if (playerY > enemyY) pDirection = PLAYER_UP;
			else if (playerY < enemyY) pDirection = PLAYER_DOWN;
		}
	}
}

void Agjin::playerMenu()
{
	//메뉴
	if (isClick)
	{
		if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->stop("move");
			SOUNDMANAGER->stop("clickMiss");
			SOUNDMANAGER->play("click", 1.0f);

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

				isMove = true;
				isTurn = false;
				isSelect = false;
				isClick = false;
			}
			if (PtInRect(&rcMenu[4], m_ptMouse))	//취소
			{
				atkList.clear();
				menuList.clear();

				auto& prevTile = mainMap->getMap()[saveTile];

				agjin.rc.left = prevTile.rc.left;
				agjin.rc.right = prevTile.rc.right;
				agjin.rc.top = prevTile.rc.top;
				agjin.rc.bottom = prevTile.rc.bottom;
				pDirection = sDirection;

				playerX = prevTile.rc.left + TILE_WIDTH * 0.5;
				playerY = prevTile.rc.top + TILE_HEIGHT * 0.5;

				isMove = true;
				isSelect = false;
				isClick = false;
			}
		}
	}
}

void Agjin::playerAnimation()
{
	if (isTurn)
	{
		if (isAtk)
		{
			switch (pDirection)
			{
			case PLAYER_LEFT:
				ANIMATIONMANAGER->addAnimation("leftAtk", "악진ATK", 12, 15, 5, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("leftAtk");
				ANIMATIONMANAGER->resume("leftAtk");
				break;
			case PLAYER_RIGHT:
				ANIMATIONMANAGER->addAnimation("rightAtk", "악진ATK", 8, 11, 5, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("rightAtk");
				ANIMATIONMANAGER->resume("rightAtk");
				break;
			case PLAYER_UP:
				ANIMATIONMANAGER->addAnimation("upAtk", "악진ATK", 4, 7, 5, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("upAtk");
				ANIMATIONMANAGER->resume("upAtk");
				break;
			case PLAYER_DOWN:
				ANIMATIONMANAGER->addAnimation("downAtk", "악진ATK", 0, 3, 5, false, false);
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
				ANIMATIONMANAGER->addAnimation("playerLeft", "악진", 4, 5, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerLeft");
				ANIMATIONMANAGER->resume("playerLeft");
				break;
			case PLAYER_RIGHT:
				ANIMATIONMANAGER->addAnimation("playerRight", "악진", 6, 7, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerRight");
				ANIMATIONMANAGER->resume("playerRight");
				break;
			case PLAYER_UP:
				ANIMATIONMANAGER->addAnimation("playerUp", "악진", 2, 3, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerUp");
				ANIMATIONMANAGER->resume("playerUp");
				break;
			case PLAYER_DOWN:
				ANIMATIONMANAGER->addAnimation("playerDown", "악진", 0, 1, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerDown");
				ANIMATIONMANAGER->resume("playerDown");
				break;
			}

			if (currentHp < 40 && !isSelect && isMove)
			{
				ANIMATIONMANAGER->addAnimation("playerHp", "악진", 12, 13, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerHp");
				ANIMATIONMANAGER->resume("playerHp");
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

void Agjin::playerState()
{
	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);
}

void Agjin::setPosition(RECT rc)
{
	agjin.rc = rc;
	playerX = agjin.rc.left + (agjin.rc.right - agjin.rc.left) / 2;
	playerY = agjin.rc.top + (agjin.rc.bottom - agjin.rc.top) / 2;
}
