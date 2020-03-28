#include "stdafx.h"
#include "Johong.h"

Johong::Johong()
{
}

Johong::~Johong()
{
}

HRESULT Johong::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * playerImg, const char * atkImg, const char * blockImg)
{
	//이름 및 얼굴
	johong.name = "조홍";
	johong.face = "조홍Face";
	johong.className = "경보병";
	//이미지 및 애니메이션
	johong.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//캐릭터 클릭시 이동범위 이미지
	johong.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//캐릭터 클릭시 공격범위 이미지
	johong.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//공격버튼 클릭시 공격범위 이미지
	johong.img = IMAGEMANAGER->findImage(playerImg);			//캐릭터 이미지
	johong.atkImg = IMAGEMANAGER->findImage(atkImg);			//공격 이미지
	johong.blockImg = IMAGEMANAGER->findImage(blockImg);		//방어 및 피격 이미지
	//스테이터스
	johong.level = 4;			//레벨
	johong.hp = 134;			//체력
	johong.mp = 17;				//마력
	johong.atk = 40;			//공격력
	johong.will = 53;			//정신력
	johong.def = 51;			//방어력
	johong.agi = 41;			//순발력
	johong.ten = 43;			//사기
	johong.movingCount = 4;		//이동력

	//HP ProgressBar
	_Hp = new progressBar;
	_Hp->init("images/UI/Info/HP.bmp", "images/UI/Info/Back_P.bmp", 1056, 289, 84, 12);
	_Hp->setGauge(currentHp, maxHp);
	currentHp = maxHp = 134;

	//MP ProgressBar
	_Mp = new progressBar;
	_Mp->init("images/UI/Info/MP.bmp", "images/UI/Info/Back_P.bmp", 1056, 307, 84, 12);
	_Mp->setGauge(currentMp, maxMp);
	currentMp = maxMp = 17;

	//EXP ProgressBar
	_Exp = new progressBar;
	_Exp->init("images/UI/Info/EXP.bmp", "images/UI/Info/Back_EXP.bmp", 1095, 243, 45, 12);
	_Exp->setGauge(currentExp, maxExp);
	currentExp = 47;
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

void Johong::release()
{
}

void Johong::update()
{
	if (isTurn)
	{
		if (!PLAYERMANAGER->getPlayer()[0]->getIsSelect() &&
			!PLAYERMANAGER->getPlayer()[1]->getIsSelect() &&
			!PLAYERMANAGER->getPlayer()[2]->getIsSelect() &&
			!PLAYERMANAGER->getPlayer()[3]->getIsSelect() &&
			!PLAYERMANAGER->getPlayer()[4]->getIsSelect() &&
			!PLAYERMANAGER->getPlayer()[5]->getIsSelect())
		{
			mouseMove();
		}
	}

	playerAnimation();
	playerState();

	if (KEYMANAGER->isOnceKeyDown('5')) isTurn = true;
}

void Johong::render(HDC hdc)
{
	if (isTurn)
	{
		if (isAtk)
		{
			johong.atkImg->aniRender(hdc, johong.rc.left - 8, johong.rc.top - 8, playerAni);
		}
		else if (isHit)
		{
			johong.blockImg->frameRender(hdc, johong.rc.left, johong.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, johong.rc.left, johong.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			johong.img->aniRender(hdc, johong.rc.left, johong.rc.top, playerAni);
		}
	}
	else
	{
		if (isHit)
		{
			johong.blockImg->frameRender(hdc, johong.rc.left, johong.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, johong.rc.left, johong.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			johong.img->frameAlphaRender(hdc, johong.rc.left, johong.rc.top, 0, frameY, 100);
		}
	}
}

void Johong::mouseMove()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&johong.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
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
					johong.rcAtk[0] = RectMake(johong.rc.left - 48, johong.rc.top, TILE_WIDTH, TILE_HEIGHT);
					johong.rcAtk[1] = RectMake(johong.rc.left - 48, johong.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					johong.rcAtk[2] = RectMake(johong.rc.left - 48, johong.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					johong.rcAtk[3] = RectMake(johong.rc.left + 48, johong.rc.top, TILE_WIDTH, TILE_HEIGHT);
					johong.rcAtk[4] = RectMake(johong.rc.left + 48, johong.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					johong.rcAtk[5] = RectMake(johong.rc.left + 48, johong.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					johong.rcAtk[6] = RectMake(johong.rc.left, johong.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					johong.rcAtk[7] = RectMake(johong.rc.left, johong.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(johong.rcAtk[j]);
				}

				//이동범위
				if (isMove)
				{
					floodFill(startTile, johong.movingCount);

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

		if (!PtInRect(&johong.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
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
	}

	playerAstar();
	playerCollision();
	playerMenu();
}

void Johong::playerMove()
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

	if (johong.rc.left > 0 || johong.rc.right < WINSIZEY ||
		johong.rc.top > 0 || johong.rc.bottom < WINSIZEY)
	{
		switch (pDirection)
		{
		case PLAYER_LEFT:
			playerX -= speed;
			johong.rc = RectMakeCenter(playerX, playerY, johong.img->getFrameWidth(), johong.img->getFrameHeight());
			break;
		case PLAYER_RIGHT:
			playerX += speed;
			johong.rc = RectMakeCenter(playerX, playerY, johong.img->getFrameWidth(), johong.img->getFrameHeight());
			break;
		case PLAYER_UP:
			playerY -= speed;
			johong.rc = RectMakeCenter(playerX, playerY, johong.img->getFrameWidth(), johong.img->getFrameHeight());
			break;
		case PLAYER_DOWN:
			playerY += speed;
			johong.rc = RectMakeCenter(playerX, playerY, johong.img->getFrameWidth(), johong.img->getFrameHeight());
			break;
		}

		if (playerX == stackX && playerY == stackY)
		{
			optimalPath.pop();
		}
	}
}

void Johong::playerAstar()
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
				johong.rcAtk[0] = RectMake(johong.rc.left - 48, johong.rc.top, TILE_WIDTH, TILE_HEIGHT);
				johong.rcAtk[1] = RectMake(johong.rc.left - 48, johong.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				johong.rcAtk[2] = RectMake(johong.rc.left - 48, johong.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				johong.rcAtk[3] = RectMake(johong.rc.left + 48, johong.rc.top, TILE_WIDTH, TILE_HEIGHT);
				johong.rcAtk[4] = RectMake(johong.rc.left + 48, johong.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				johong.rcAtk[5] = RectMake(johong.rc.left + 48, johong.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				johong.rcAtk[6] = RectMake(johong.rc.left, johong.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				johong.rcAtk[7] = RectMake(johong.rc.left, johong.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				atkList.push_back(johong.rcAtk[j]);
			}
		}

		if (isClick)
		{
			//메뉴선택 렉트
			for (int j = 0; j < 5; j++)
			{
				rcMenu[0] = RectMake(johong.rc.left - 97, johong.rc.top - 30, 82, 20);
				rcMenu[1] = RectMake(johong.rc.left - 97, johong.rc.top - 9, 82, 20);
				rcMenu[2] = RectMake(johong.rc.left - 97, johong.rc.top + 12, 82, 20);
				rcMenu[3] = RectMake(johong.rc.left - 97, johong.rc.top + 38, 82, 20);
				rcMenu[4] = RectMake(johong.rc.left - 97, johong.rc.top + 63, 82, 20);
				menuList.push_back(rcMenu[j]);
			}
		}
	}
}

void Johong::playerMenu()
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

				johong.rc.left = prevTile.rc.left;
				johong.rc.right = prevTile.rc.right;
				johong.rc.top = prevTile.rc.top;
				johong.rc.bottom = prevTile.rc.bottom;
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

void Johong::playerCollision()
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
			if (IntersectRect(&temp, &johong.rcAtk[k], &enemyRect))
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

void Johong::playerAnimation()
{
	if (isTurn)
	{
		if (isAtk)
		{
			switch (pDirection)
			{
			case PLAYER_LEFT:
				ANIMATIONMANAGER->addAnimation("leftAtk", "조홍ATK", 12, 15, 5, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("leftAtk");
				ANIMATIONMANAGER->resume("leftAtk");
				break;
			case PLAYER_RIGHT:
				ANIMATIONMANAGER->addAnimation("rightAtk", "조홍ATK", 8, 11, 5, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("rightAtk");
				ANIMATIONMANAGER->resume("rightAtk");
				break;
			case PLAYER_UP:
				ANIMATIONMANAGER->addAnimation("upAtk", "조홍ATK", 4, 7, 5, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("upAtk");
				ANIMATIONMANAGER->resume("upAtk");
				break;
			case PLAYER_DOWN:
				ANIMATIONMANAGER->addAnimation("downAtk", "조홍ATK", 0, 3, 5, false, false);
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
				ANIMATIONMANAGER->addAnimation("playerLeft", "조홍", 4, 5, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerLeft");
				ANIMATIONMANAGER->resume("playerLeft");
				break;
			case PLAYER_RIGHT:
				ANIMATIONMANAGER->addAnimation("playerRight", "조홍", 6, 7, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerRight");
				ANIMATIONMANAGER->resume("playerRight");
				break;
			case PLAYER_UP:
				ANIMATIONMANAGER->addAnimation("playerUp", "조홍", 2, 3, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerUp");
				ANIMATIONMANAGER->resume("playerUp");
				break;
			case PLAYER_DOWN:
				ANIMATIONMANAGER->addAnimation("playerDown", "조홍", 0, 1, 2, false, true);
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

void Johong::playerState()
{
	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);
}

void Johong::setPosition(RECT rc)
{
	johong.rc = rc;
	playerX = johong.rc.left + (johong.rc.right - johong.rc.left) / 2;
	playerY = johong.rc.top + (johong.rc.bottom - johong.rc.top) / 2;
}
