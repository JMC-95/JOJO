#include "stdafx.h"
#include "Archer.h"

Archer::Archer(int num)
{
	number = num;
}

Archer::~Archer()
{
}

HRESULT Archer::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * enemyImg, const char * atkImg, const char * blockImg)
{
	//이름 및 얼굴
	archer.name = "궁병";
	archer.face = "적궁병Face";
	archer.className = "궁병";
	//이미지 및 애니메이션
	archer.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//캐릭터 클릭시 이동범위 이미지
	archer.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//캐릭터 클릭시 공격범위 이미지
	archer.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//공격버튼 클릭시 공격범위 이미지
	archer.img = IMAGEMANAGER->findImage(enemyImg);				//캐릭터 이미지
	archer.atkImg = IMAGEMANAGER->findImage(atkImg);			//공격 이미지
	archer.blockImg = IMAGEMANAGER->findImage(blockImg);		//방어 및 피격 이미지
	ANIMATIONMANAGER->addAnimation("enemyRight", "적궁병", 6, 7, 2, false, true);
	enemyAni = ANIMATIONMANAGER->findAnimation("enemyRight");
	//스테이터스
	archer.level = 2;			//레벨
	archer.hp = 94;				//체력
	archer.mp = 11;				//마력
	archer.atk = 42;			//공격력
	archer.will = 32;			//정신력
	archer.def = 32;			//방어력
	archer.agi = 32;			//순발력
	archer.ten = 43;			//사기
	archer.movingCount = 4;		//이동력
	//아이템
	archer.weaponName = ITEMMANAGER->getWeapon()[2].name;
	archer.armorName = ITEMMANAGER->getArmor()[0].name;
	archer.weaponImg = ITEMMANAGER->getWeapon()[2].itemImage;
	archer.armorImg = ITEMMANAGER->getArmor()[0].itemImage;
	archer.addAtk = ITEMMANAGER->getWeapon()[2].power;
	archer.addDef = ITEMMANAGER->getArmor()[0].power;

	//HP ProgressBar
	_Hp = new progressBar;
	_Hp->init("images/UI/Info/HP.bmp", "images/UI/Info/Back_P.bmp", 1056, 289, 84, 12);
	_Hp->setGauge(currentHp, maxHp);
	currentHp = maxHp = 94;

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
	speed = 12;	//속도

	isTurn = true;
	isMove = true;
	isSelect = false;

	return S_OK;
}

void Archer::release()
{
	SAFE_DELETE(_Hp);
	SAFE_DELETE(_Mp);
	SAFE_DELETE(_Exp);
}

void Archer::update()
{
	if (isTurn)
	{
		if (ENEMYMANAGER->getEturn()) enemyAi();
	}

	enemyAnimation();
	enemyState();
	mouseMove();
}

void Archer::render(HDC hdc)
{
	if (isTurn)
	{
		if (isAtk)
		{
			archer.atkImg->aniRender(hdc, archer.rc.left - 8, archer.rc.top - 8, enemyAni);
		}
		else if (isHit)
		{
			archer.blockImg->frameRender(hdc, archer.rc.left, archer.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, archer.rc.left, archer.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			archer.img->aniRender(hdc, archer.rc.left, archer.rc.top, enemyAni);
		}
	}
	else
	{
		if (isHit)
		{
			archer.blockImg->frameRender(hdc, archer.rc.left, archer.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, archer.rc.left, archer.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			archer.img->frameAlphaRender(hdc, archer.rc.left, archer.rc.top, 0, frameY, 100);
		}
	}
}

void Archer::enemyAi()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&archer.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
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
					archer.rcAtk[0] = RectMake(archer.rc.left - 96, archer.rc.top, TILE_WIDTH, TILE_HEIGHT);
					archer.rcAtk[1] = RectMake(archer.rc.left, archer.rc.top - 96, TILE_WIDTH, TILE_HEIGHT);
					archer.rcAtk[2] = RectMake(archer.rc.left - 48, archer.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					archer.rcAtk[3] = RectMake(archer.rc.left - 48, archer.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					archer.rcAtk[4] = RectMake(archer.rc.left + 96, archer.rc.top, TILE_WIDTH, TILE_HEIGHT);
					archer.rcAtk[5] = RectMake(archer.rc.left, archer.rc.top + 96, TILE_WIDTH, TILE_HEIGHT);
					archer.rcAtk[6] = RectMake(archer.rc.left + 48, archer.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					archer.rcAtk[7] = RectMake(archer.rc.left + 48, archer.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(archer.rcAtk[j]);
				}

				//이동범위
				if (isMove)
				{
					floodFill(startTile, archer.movingCount);

					for (int i = 0; i < TILE_X * TILE_Y; i++)
					{
						for (int k = 0; k < PLAYERMANAGER->getPlayer().size(); ++k)
						{
							auto& rc = mainMap->getMap()[i].rc;
							auto& playerRect = PLAYERMANAGER->getPlayer()[k]->getPlayerInfo().rc;

							if (IntersectRect(&temp, &rc, &playerRect))
							{
								mainMap->getMap()[i].flood = false;
							}
						}

						for (int k = 0; k < FRIENDMANAGER->getFriend().size(); ++k)
						{
							auto& rc = mainMap->getMap()[i].rc;
							auto& friendRect = FRIENDMANAGER->getFriend()[k]->getFriendInfo().rc;

							if (IntersectRect(&temp, &rc, &friendRect))
							{
								mainMap->getMap()[i].flood = false;
							}
						}

						for (int k = 0; k < ENEMYMANAGER->getEnemy().size(); ++k)
						{
							auto& rc = mainMap->getMap()[i].rc;
							auto& enemyRect = ENEMYMANAGER->getEnemy()[k]->getEnemyInfo().rc;

							if (IntersectRect(&temp, &rc, &enemyRect))
							{
								mainMap->getMap()[i].flood = false;
							}
						}
					}
				}
			}
		}

		if (!PtInRect(&archer.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				if (mainMap->getMap()[i].flood)
				{
					SOUNDMANAGER->play("cMove", 1.0f);

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

	if (isSelect && PtInRect(&archer.rc, m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		isClick = true;
		isMove = false;
		isSelect = false;

		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			if (mainMap->getMap()[i].flood) mainMap->getMap()[i].flood = false;
		}
	}

	enemyAstar();
	enemyMenu();
	enemyCollision();
}

void Archer::mouseMove()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && PtInRect(&archer.rc, m_ptMouse))
		{
			if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
			{
				//선택한 타일 (캐릭터)
				startTile = i;

				//캐릭터 클릭
				isSelect = true;

				//공격범위
				for (int j = 0; j < 8; j++)
				{
					archer.rcAtk[0] = RectMake(archer.rc.left - 96, archer.rc.top, TILE_WIDTH, TILE_HEIGHT);
					archer.rcAtk[1] = RectMake(archer.rc.left, archer.rc.top - 96, TILE_WIDTH, TILE_HEIGHT);
					archer.rcAtk[2] = RectMake(archer.rc.left - 48, archer.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					archer.rcAtk[3] = RectMake(archer.rc.left - 48, archer.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					archer.rcAtk[4] = RectMake(archer.rc.left + 96, archer.rc.top, TILE_WIDTH, TILE_HEIGHT);
					archer.rcAtk[5] = RectMake(archer.rc.left, archer.rc.top + 96, TILE_WIDTH, TILE_HEIGHT);
					archer.rcAtk[6] = RectMake(archer.rc.left + 48, archer.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					archer.rcAtk[7] = RectMake(archer.rc.left + 48, archer.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(archer.rcAtk[j]);
				}

				//이동범위
				if (isMove) floodFill(startTile, archer.movingCount);
			}
		}

		if (PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				//캐릭터 클릭
				isSelect = false;

				//공격범위
				atkList.clear();

				//이동범위
				for (int i = 0; i < TILE_X * TILE_Y; i++)
				{
					if (mainMap->getMap()[i].flood) mainMap->getMap()[i].flood = false;
				}
			}
		}
	}
}

void Archer::enemyMove()
{
	stackX = optimalPath.top().rc.left + (optimalPath.top().rc.right - optimalPath.top().rc.left) / 2;
	stackY = optimalPath.top().rc.top + (optimalPath.top().rc.bottom - optimalPath.top().rc.top) / 2;

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

	if (archer.rc.left > 0 || archer.rc.right < WINSIZEY ||
		archer.rc.top > 0 || archer.rc.bottom < WINSIZEY)
	{
		switch (eDirection)
		{
		case ENEMY_LEFT:
			enemyX -= speed;
			archer.rc = RectMakeCenter(enemyX, enemyY, archer.img->getFrameWidth(), archer.img->getFrameHeight());
			break;
		case ENEMY_RIGHT:
			enemyX += speed;
			archer.rc = RectMakeCenter(enemyX, enemyY, archer.img->getFrameWidth(), archer.img->getFrameHeight());
			break;
		case ENEMY_UP:
			enemyY -= speed;
			archer.rc = RectMakeCenter(enemyX, enemyY, archer.img->getFrameWidth(), archer.img->getFrameHeight());
			break;
		case ENEMY_DOWN:
			enemyY += speed;
			archer.rc = RectMakeCenter(enemyX, enemyY, archer.img->getFrameWidth(), archer.img->getFrameHeight());
			break;
		}

		if (enemyX == stackX && enemyY == stackY)
		{
			optimalPath.pop();
		}
	}
}

void Archer::enemyAstar()
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
			enemyMove();
		}

		if (enemyX == mapX && enemyY == mapY)
		{
			isMove = false;
			isClick = true;

			//공격범위
			for (int j = 0; j < 8; j++)
			{
				archer.rcAtk[0] = RectMake(archer.rc.left - 96, archer.rc.top, TILE_WIDTH, TILE_HEIGHT);
				archer.rcAtk[1] = RectMake(archer.rc.left, archer.rc.top - 96, TILE_WIDTH, TILE_HEIGHT);
				archer.rcAtk[2] = RectMake(archer.rc.left - 48, archer.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				archer.rcAtk[3] = RectMake(archer.rc.left - 48, archer.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				archer.rcAtk[4] = RectMake(archer.rc.left + 96, archer.rc.top, TILE_WIDTH, TILE_HEIGHT);
				archer.rcAtk[5] = RectMake(archer.rc.left, archer.rc.top + 96, TILE_WIDTH, TILE_HEIGHT);
				archer.rcAtk[6] = RectMake(archer.rc.left + 48, archer.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				archer.rcAtk[7] = RectMake(archer.rc.left + 48, archer.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				atkList.push_back(archer.rcAtk[j]);
			}
		}
	}

	if (isClick)
	{
		//메뉴선택 렉트
		for (int j = 0; j < 5; j++)
		{
			rcMenu[0] = RectMake(archer.rc.left - 97, archer.rc.top - 30, 82, 20);
			rcMenu[1] = RectMake(archer.rc.left - 97, archer.rc.top - 9, 82, 20);
			rcMenu[2] = RectMake(archer.rc.left - 97, archer.rc.top + 12, 82, 20);
			rcMenu[3] = RectMake(archer.rc.left - 97, archer.rc.top + 38, 82, 20);
			rcMenu[4] = RectMake(archer.rc.left - 97, archer.rc.top + 63, 82, 20);
			menuList.push_back(rcMenu[j]);
		}
	}
}

void Archer::enemyAnimation()
{
	if (isTurn)
	{
		if (isAtk)
		{
			switch (eDirection)
			{
			case ENEMY_LEFT:
				ANIMATIONMANAGER->addAnimation("leftAtk", "적궁병ATK", 12, 15, 5, false, false);
				enemyAni = ANIMATIONMANAGER->findAnimation("leftAtk");
				ANIMATIONMANAGER->resume("leftAtk");
				break;
			case ENEMY_RIGHT:
				ANIMATIONMANAGER->addAnimation("rightAtk", "적궁병ATK", 8, 11, 5, false, false);
				enemyAni = ANIMATIONMANAGER->findAnimation("rightAtk");
				ANIMATIONMANAGER->resume("rightAtk");
				break;
			case ENEMY_UP:
				ANIMATIONMANAGER->addAnimation("upAtk", "적궁병ATK", 4, 7, 5, false, false);
				enemyAni = ANIMATIONMANAGER->findAnimation("upAtk");
				ANIMATIONMANAGER->resume("upAtk");
				break;
			case ENEMY_DOWN:
				ANIMATIONMANAGER->addAnimation("downAtk", "적궁병ATK", 0, 3, 5, false, false);
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
				ANIMATIONMANAGER->addAnimation("enemyLeft", "적궁병", 4, 5, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyLeft");
				ANIMATIONMANAGER->resume("enemyLeft");
				break;
			case ENEMY_RIGHT:
				ANIMATIONMANAGER->addAnimation("enemyRight", "적궁병", 6, 7, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyRight");
				ANIMATIONMANAGER->resume("enemyRight");
				break;
			case ENEMY_UP:
				ANIMATIONMANAGER->addAnimation("enemyUp", "적궁병", 2, 3, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyUp");
				ANIMATIONMANAGER->resume("enemyUp");
				break;
			case ENEMY_DOWN:
				ANIMATIONMANAGER->addAnimation("enemyDown", "적궁병", 0, 1, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyDown");
				ANIMATIONMANAGER->resume("enemyDown");
				break;
			}

			if (currentHp < 40 && !isSelect && isMove)
			{
				ANIMATIONMANAGER->addAnimation("enemyHp", "적궁병", 12, 13, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyHp");
				ANIMATIONMANAGER->resume("enemyHp");
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

void Archer::enemyState()
{
	//적군 위치
	positionX = archer.rc.left / TILE_WIDTH;
	positionY = archer.rc.top / TILE_HEIGHT;
	enemyTile = positionX + (positionY * TILE_Y);

	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);

	if (currentHp < 0) currentHp = 0;
}

void Archer::enemyMenu()
{
	//메뉴
	if (isClick)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
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
			}
			if (PtInRect(&rcMenu[2], m_ptMouse))	//도구
			{
				atkList.clear();
				menuList.clear();
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

				isMove = true;
				isSelect = false;
				isClick = false;
			}
		}
	}
}

void Archer::enemyCollision()
{
	isTarget = false;
	frameX = 0;

	for (int j = 0; j < PLAYERMANAGER->getPlayer().size(); j++)
	{
		auto playerX = PLAYERMANAGER->getPlayer()[j]->getPlayerX();
		auto playerY = PLAYERMANAGER->getPlayer()[j]->getPlayerY();
		auto playerHit = PLAYERMANAGER->getPlayer()[j]->getIsHit();
		auto& playerRect = PLAYERMANAGER->getPlayer()[j]->getPlayerInfo().rc;

		for (int k = 0; k < 8; k++)
		{
			if (IntersectRect(&temp, &archer.rcAtk[k], &playerRect))
			{
				isTarget = true;
				frameX = 1;

				if (isAtk &&playerHit)
				{
					if (enemyX > playerX) eDirection = ENEMY_LEFT;
					else if (enemyX < playerX) eDirection = ENEMY_RIGHT;
					else if (enemyY > playerY) eDirection = ENEMY_UP;
					else if (enemyY < playerY) eDirection = ENEMY_DOWN;
				}
			}
		}
	}

	for (int j = 0; j < FRIENDMANAGER->getFriend().size(); j++)
	{
		auto friendX = FRIENDMANAGER->getFriend()[j]->getFriendX();
		auto friendY = FRIENDMANAGER->getFriend()[j]->getFriendY();
		auto friendHit = FRIENDMANAGER->getFriend()[j]->getIsHit();
		auto& friendRect = FRIENDMANAGER->getFriend()[j]->getFriendInfo().rc;

		for (int k = 0; k < 8; k++)
		{
			if (IntersectRect(&temp, &archer.rcAtk[k], &friendRect))
			{
				isTarget = true;
				frameX = 1;
			}
		}

		if (friendHit)
		{
			if (enemyX > friendX) eDirection = ENEMY_LEFT;
			else if (enemyX < friendX) eDirection = ENEMY_RIGHT;
			else if (enemyY > friendY) eDirection = ENEMY_UP;
			else if (enemyY < friendY) eDirection = ENEMY_DOWN;
		}
	}
}

void Archer::setPosition(RECT rc)
{
	archer.rc = rc;
	enemyX = archer.rc.left + (archer.rc.right - archer.rc.left) / 2;
	enemyY = archer.rc.top + (archer.rc.bottom - archer.rc.top) / 2;
}
