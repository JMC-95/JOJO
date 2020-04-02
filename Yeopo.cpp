#include "stdafx.h"
#include "Yeopo.h"

Yeopo::Yeopo()
{
}

Yeopo::~Yeopo()
{
}

HRESULT Yeopo::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * enemyImg, const char * atkImg, const char * blockImg)
{
	//이름 및 얼굴
	yeopo.name = "여포";
	yeopo.face = "여포Face";
	yeopo.className = "서량기병";
	//이미지 및 애니메이션
	yeopo.moveRngImg = IMAGEMANAGER->findImage(moveImg);	//캐릭터 클릭시 이동범위 이미지
	yeopo.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//캐릭터 클릭시 공격범위 이미지
	yeopo.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//공격버튼 클릭시 공격범위 이미지
	yeopo.img = IMAGEMANAGER->findImage(enemyImg);			//캐릭터 이미지
	yeopo.atkImg = IMAGEMANAGER->findImage(atkImg);			//공격 이미지
	yeopo.blockImg = IMAGEMANAGER->findImage(blockImg);		//방어 및 피격 이미지
	ANIMATIONMANAGER->addAnimation("enemyRight", "여포", 6, 7, 2, false, true);
	enemyAni = ANIMATIONMANAGER->findAnimation("enemyRight");
	//스테이터스
	yeopo.level = 6;			//레벨
	yeopo.hp = 180;				//체력
	yeopo.mp = 10;				//마력
	yeopo.atk = 70;				//공격력
	yeopo.will = 22;			//정신력
	yeopo.def = 54;				//방어력
	yeopo.agi = 65;				//순발력
	yeopo.ten = 65;				//사기
	yeopo.movingCount = 6;		//이동력
	//아이템
	yeopo.weaponName = ITEMMANAGER->getWeapon()[4].name;
	yeopo.armorName = ITEMMANAGER->getArmor()[1].name;
	yeopo.asstName = ITEMMANAGER->getAssistant()[0].name;
	yeopo.weaponImg = ITEMMANAGER->getWeapon()[4].itemImage;
	yeopo.armorImg = ITEMMANAGER->getArmor()[1].itemImage;
	yeopo.asstImg = ITEMMANAGER->getAssistant()[0].itemImage;
	yeopo.addAtk = ITEMMANAGER->getWeapon()[4].power;
	yeopo.addDef = ITEMMANAGER->getArmor()[1].power;

	//HP ProgressBar
	_Hp = new progressBar;
	_Hp->init("images/UI/Info/HP.bmp", "images/UI/Info/Back_P.bmp", 1056, 289, 84, 12);
	_Hp->setGauge(currentHp, maxHp);
	currentHp = maxHp = 180;

	//MP ProgressBar
	_Mp = new progressBar;
	_Mp->init("images/UI/Info/MP.bmp", "images/UI/Info/Back_P.bmp", 1056, 307, 84, 12);
	_Mp->setGauge(currentMp, maxMp);
	currentMp = maxMp = 10;

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

void Yeopo::release()
{
	SAFE_DELETE(_Hp);
	SAFE_DELETE(_Mp);
	SAFE_DELETE(_Exp);
}

void Yeopo::update()
{
	if (isTurn)
	{
		if (ENEMYMANAGER->getEturn()) enemyAi();
	}

	enemyAnimation();
	enemyState();
	mouseMove();
}

void Yeopo::render(HDC hdc)
{
	if (isTurn)
	{
		if (isAtk)
		{
			yeopo.atkImg->aniRender(hdc, yeopo.rc.left - 8, yeopo.rc.top - 8, enemyAni);
		}
		else if (isHit)
		{
			yeopo.blockImg->frameRender(hdc, yeopo.rc.left, yeopo.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, yeopo.rc.left, yeopo.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			yeopo.img->aniRender(hdc, yeopo.rc.left, yeopo.rc.top, enemyAni);
		}
	}
	else
	{
		if (isHit)
		{
			yeopo.blockImg->frameRender(hdc, yeopo.rc.left, yeopo.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, yeopo.rc.left, yeopo.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			yeopo.img->frameAlphaRender(hdc, yeopo.rc.left, yeopo.rc.top, 0, frameY, 100);
		}
	}
}

void Yeopo::enemyAi()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&yeopo.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
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
				for (int j = 0; j < 4; j++)
				{
					yeopo.rcAtk[0] = RectMake(yeopo.rc.left - 48, yeopo.rc.top, TILE_WIDTH, TILE_HEIGHT);
					yeopo.rcAtk[1] = RectMake(yeopo.rc.left + 48, yeopo.rc.top, TILE_WIDTH, TILE_HEIGHT);
					yeopo.rcAtk[2] = RectMake(yeopo.rc.left, yeopo.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					yeopo.rcAtk[3] = RectMake(yeopo.rc.left, yeopo.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(yeopo.rcAtk[j]);
				}

				//이동범위
				if (isMove)
				{
					floodFill(startTile, yeopo.movingCount);

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

		if (!PtInRect(&yeopo.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
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

	enemyAstar();
	enemyMenu();
	enemyCollision();
}

void Yeopo::mouseMove()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && PtInRect(&yeopo.rc, m_ptMouse))
		{
			if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
			{
				//선택한 타일 (캐릭터)
				startTile = i;

				//캐릭터 클릭
				isSelect = true;

				//공격범위
				for (int j = 0; j < 4; j++)
				{
					yeopo.rcAtk[0] = RectMake(yeopo.rc.left - 48, yeopo.rc.top, TILE_WIDTH, TILE_HEIGHT);
					yeopo.rcAtk[1] = RectMake(yeopo.rc.left + 48, yeopo.rc.top, TILE_WIDTH, TILE_HEIGHT);
					yeopo.rcAtk[2] = RectMake(yeopo.rc.left, yeopo.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					yeopo.rcAtk[3] = RectMake(yeopo.rc.left, yeopo.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(yeopo.rcAtk[j]);
				}

				//이동범위
				if (isMove) floodFill(startTile, yeopo.movingCount);
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

void Yeopo::enemyMove()
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

	if (yeopo.rc.left > 0 || yeopo.rc.right < WINSIZEY ||
		yeopo.rc.top > 0 || yeopo.rc.bottom < WINSIZEY)
	{
		switch (eDirection)
		{
		case ENEMY_LEFT:
			enemyX -= speed;
			yeopo.rc = RectMakeCenter(enemyX, enemyY, yeopo.img->getFrameWidth(), yeopo.img->getFrameHeight());
			break;
		case ENEMY_RIGHT:
			enemyX += speed;
			yeopo.rc = RectMakeCenter(enemyX, enemyY, yeopo.img->getFrameWidth(), yeopo.img->getFrameHeight());
			break;
		case ENEMY_UP:
			enemyY -= speed;
			yeopo.rc = RectMakeCenter(enemyX, enemyY, yeopo.img->getFrameWidth(), yeopo.img->getFrameHeight());
			break;
		case ENEMY_DOWN:
			enemyY += speed;
			yeopo.rc = RectMakeCenter(enemyX, enemyY, yeopo.img->getFrameWidth(), yeopo.img->getFrameHeight());
			break;
		}

		if (enemyX == stackX && enemyY == stackY)
		{
			optimalPath.pop();
		}
	}
}

void Yeopo::enemyAstar()
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
			for (int j = 0; j < 4; j++)
			{
				yeopo.rcAtk[0] = RectMake(yeopo.rc.left - 48, yeopo.rc.top, TILE_WIDTH, TILE_HEIGHT);
				yeopo.rcAtk[1] = RectMake(yeopo.rc.left + 48, yeopo.rc.top, TILE_WIDTH, TILE_HEIGHT);
				yeopo.rcAtk[2] = RectMake(yeopo.rc.left, yeopo.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				yeopo.rcAtk[3] = RectMake(yeopo.rc.left, yeopo.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				atkList.push_back(yeopo.rcAtk[j]);
			}
		}
	}

	if (isClick)
	{
		//메뉴선택 렉트
		for (int j = 0; j < 5; j++)
		{
			rcMenu[0] = RectMake(yeopo.rc.left - 97, yeopo.rc.top - 30, 82, 20);
			rcMenu[1] = RectMake(yeopo.rc.left - 97, yeopo.rc.top - 9, 82, 20);
			rcMenu[2] = RectMake(yeopo.rc.left - 97, yeopo.rc.top + 12, 82, 20);
			rcMenu[3] = RectMake(yeopo.rc.left - 97, yeopo.rc.top + 38, 82, 20);
			rcMenu[4] = RectMake(yeopo.rc.left - 97, yeopo.rc.top + 63, 82, 20);
			menuList.push_back(rcMenu[j]);
		}
	}
}

void Yeopo::enemyAnimation()
{
	if (isTurn)
	{
		if (isAtk)
		{
			switch (eDirection)
			{
			case ENEMY_LEFT:
				ANIMATIONMANAGER->addAnimation("leftAtk", "여포ATK", 12, 15, 5, false, false);
				enemyAni = ANIMATIONMANAGER->findAnimation("leftAtk");
				ANIMATIONMANAGER->resume("leftAtk");
				break;
			case ENEMY_RIGHT:
				ANIMATIONMANAGER->addAnimation("rightAtk", "여포ATK", 8, 11, 5, false, false);
				enemyAni = ANIMATIONMANAGER->findAnimation("rightAtk");
				ANIMATIONMANAGER->resume("rightAtk");
				break;
			case ENEMY_UP:
				ANIMATIONMANAGER->addAnimation("upAtk", "여포ATK", 4, 7, 5, false, false);
				enemyAni = ANIMATIONMANAGER->findAnimation("upAtk");
				ANIMATIONMANAGER->resume("upAtk");
				break;
			case ENEMY_DOWN:
				ANIMATIONMANAGER->addAnimation("downAtk", "여포ATK", 0, 3, 5, false, false);
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
				ANIMATIONMANAGER->addAnimation("enemyLeft", "여포", 4, 5, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyLeft");
				ANIMATIONMANAGER->resume("enemyLeft");
				break;
			case ENEMY_RIGHT:
				ANIMATIONMANAGER->addAnimation("enemyRight", "여포", 6, 7, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyRight");
				ANIMATIONMANAGER->resume("enemyRight");
				break;
			case ENEMY_UP:
				ANIMATIONMANAGER->addAnimation("enemyUp", "여포", 2, 3, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyUp");
				ANIMATIONMANAGER->resume("enemyUp");
				break;
			case ENEMY_DOWN:
				ANIMATIONMANAGER->addAnimation("enemyDown", "여포", 0, 1, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyDown");
				ANIMATIONMANAGER->resume("enemyDown");
				break;
			}

			if (currentHp < 40 && !isSelect && isMove)
			{
				ANIMATIONMANAGER->addAnimation("enemyHp", "여포", 12, 13, 2, false, true);
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

void Yeopo::enemyState()
{
	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);

	if (currentHp < 0) currentHp = 0;
}

void Yeopo::enemyMenu()
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

void Yeopo::enemyCollision()
{
	frameX = 0;

	for (int j = 0; j < PLAYERMANAGER->getPlayer().size(); j++)
	{
		auto playerX = PLAYERMANAGER->getPlayer()[j]->getPlayerX();
		auto playerY = PLAYERMANAGER->getPlayer()[j]->getPlayerY();
		auto playerHit = PLAYERMANAGER->getPlayer()[j]->getIsHit();
		auto& playerRect = PLAYERMANAGER->getPlayer()[j]->getPlayerInfo().rc;

		for (int k = 0; k < 4; k++)
		{
			if (IntersectRect(&temp, &yeopo.rcAtk[k], &playerRect))
			{
				isTarget = true;
				frameX = 1;
			}
		}

		if (playerHit)
		{
			if (enemyX > playerX) eDirection = ENEMY_LEFT;
			else if (enemyX < playerX) eDirection = ENEMY_RIGHT;
			else if (enemyY > playerY) eDirection = ENEMY_UP;
			else if (enemyY < playerY) eDirection = ENEMY_DOWN;
		}
	}

	for (int j = 0; j < FRIENDMANAGER->getFriend().size(); j++)
	{
		auto friendX = FRIENDMANAGER->getFriend()[j]->getFriendX();
		auto friendY = FRIENDMANAGER->getFriend()[j]->getFriendY();
		auto friendHit = FRIENDMANAGER->getFriend()[j]->getIsHit();
		auto& friendRect = FRIENDMANAGER->getFriend()[j]->getFriendInfo().rc;

		for (int k = 0; k < 4; k++)
		{
			if (IntersectRect(&temp, &yeopo.rcAtk[k], &friendRect))
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

void Yeopo::setPosition(RECT rc)
{
	yeopo.rc = rc;
	enemyX = yeopo.rc.left + (yeopo.rc.right - yeopo.rc.left) / 2;
	enemyY = yeopo.rc.top + (yeopo.rc.bottom - yeopo.rc.top) / 2;
}
