#include "stdafx.h"
#include "Join.h"

Join::Join()
{
}

Join::~Join()
{
}

HRESULT Join::init(const char* skillImg, const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * playerImg, const char * atkImg, const char * blockImg)
{
	//이름 및 얼굴
	join.name = "조인";
	join.face = "조인Face";
	join.className = "경기병";
	//이미지 및 애니메이션
	join.skillImg = IMAGEMANAGER->findImage(skillImg);		//스킬 이미지
	ANIMATIONMANAGER->addDefAnimation("healImg", "heal", 30, false, false);
	skillAni = ANIMATIONMANAGER->findAnimation("healImg");
	join.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//캐릭터 클릭시 이동범위 이미지
	join.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//캐릭터 클릭시 공격범위 이미지
	join.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//공격버튼 클릭시 공격범위 이미지
	join.img = IMAGEMANAGER->findImage(playerImg);			//캐릭터 이미지
	join.atkImg = IMAGEMANAGER->findImage(atkImg);			//공격 이미지
	join.blockImg = IMAGEMANAGER->findImage(blockImg);		//방어 및 피격 이미지
	ANIMATIONMANAGER->addAnimation("playerLeft", "조인", 4, 5, 2, false, true);
	playerAni = ANIMATIONMANAGER->findAnimation("playerLeft");
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
	//아이템
	join.weaponName = ITEMMANAGER->getWeapon()[1].name;
	join.armorName = ITEMMANAGER->getArmor()[0].name;
	join.weaponImg = ITEMMANAGER->getWeapon()[1].itemImage;
	join.armorImg = ITEMMANAGER->getArmor()[0].itemImage;
	join.addAtk = ITEMMANAGER->getWeapon()[1].power;
	join.addDef = ITEMMANAGER->getArmor()[0].power;

	//HP ProgressBar
	_Hp = new progressBar;
	_Hp->init("images/UI/Info/HP.bmp", "images/UI/Info/Back_P.bmp", 1056, 289, 84, 12);
	_Hp->setGauge(currentHp, maxHp);
	currentHp = maxHp = 120;

	//MP ProgressBar
	_Mp = new progressBar;
	_Mp->init("images/UI/Info/MP.bmp", "images/UI/Info/Back_P.bmp", 1056, 307, 84, 12);
	_Mp->setGauge(currentMp, maxMp);
	currentMp = maxMp = 18;

	//EXP ProgressBar
	_Exp = new progressBar;
	_Exp->init("images/UI/Info/EXP.bmp", "images/UI/Info/Back_EXP.bmp", 1095, 243, 45, 12);
	_Exp->setGauge(currentExp, maxExp);
	currentExp = 0;
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

void Join::release()
{
}

void Join::update()
{
	if (isTurn)
	{
		if (PLAYERMANAGER->getPturn())
		{
			auto& playerVector = PLAYERMANAGER->getPlayer();

			bool isMouseMove = true;

			for (int i = 0; i < playerVector.size(); ++i)
			{
				if (playerVector[i]->getPlayerInfo().name == "조인")
				{
					continue;
				}

				if (playerVector[i]->getIsSelect() || playerVector[i]->getIsClick())
				{
					isMouseMove = false;
					break;
				}
			}

			if (isMouseMove)
			{
				if (!playerVector[0]->getIsSkill() && !playerVector[0]->getIsSkillCheck()) mouseMove();
			}
		}
	}

	playerAnimation();
	playerState();
}

void Join::render(HDC hdc)
{
	if (isTurn)
	{
		if (isAtk)
		{
			join.atkImg->aniRender(hdc, join.rc.left - 8, join.rc.top - 8, playerAni);
		}
		else if (isHit)
		{
			join.blockImg->frameRender(hdc, join.rc.left, join.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, join.rc.left, join.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			if (isHeal)
			{
				join.blockImg->frameRender(hdc, join.rc.left, join.rc.top, 0, 5);
				join.skillImg->aniRender(hdc, join.rc.left - 8, join.rc.top - 8, skillAni);

				if (maxHp - currentHp < 30)
				{
					sprintf_s(str, "%d", maxHp - currentHp);
					TextOut(hdc, join.rc.left, join.rc.top, str, strlen(str));
				}
				else
				{
					sprintf_s(str, "%d", 30);
					TextOut(hdc, join.rc.left, join.rc.top, str, strlen(str));
				}
			}
			else
			{
				join.img->aniRender(hdc, join.rc.left, join.rc.top, playerAni);
			}
		}
	}
	else
	{
		if (isHit)
		{
			join.blockImg->frameRender(hdc, join.rc.left, join.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, join.rc.left, join.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			if (isHeal)
			{
				join.blockImg->frameRender(hdc, join.rc.left, join.rc.top, 0, 5);
				join.skillImg->aniRender(hdc, join.rc.left - 8, join.rc.top - 8, skillAni);

				if (maxHp - currentHp < 30)
				{
					sprintf_s(str, "%d", maxHp - currentHp);
					TextOut(hdc, join.rc.left, join.rc.top, str, strlen(str));
				}
				else
				{
					sprintf_s(str, "%d", 30);
					TextOut(hdc, join.rc.left, join.rc.top, str, strlen(str));
				}
			}
			else
			{
				join.img->frameAlphaRender(hdc, join.rc.left, join.rc.top, 0, frameY, 100);
			}
		}
	}
}

void Join::mouseMove()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&join.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
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
				for (int j = 0; j < 4; j++)
				{
					join.rcAtk[0] = RectMake(join.rc.left - 48, join.rc.top, TILE_WIDTH, TILE_HEIGHT);
					join.rcAtk[1] = RectMake(join.rc.left + 48, join.rc.top, TILE_WIDTH, TILE_HEIGHT);
					join.rcAtk[2] = RectMake(join.rc.left, join.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					join.rcAtk[3] = RectMake(join.rc.left, join.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(join.rcAtk[j]);
				}

				//이동범위
				if (isMove)
				{
					floodFill(startTile, join.movingCount);

					for (int j = 0; j < TILE_X * TILE_Y; j++)
					{
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

		if (!PtInRect(&join.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
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
					if (!isClick) SOUNDMANAGER->play("clickMiss", 1.0f);
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

void Join::playerMove()
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

	if (join.rc.left > 0 || join.rc.right < WINSIZEY ||
		join.rc.top > 0 || join.rc.bottom < WINSIZEY)
	{
		switch (pDirection)
		{
		case PLAYER_LEFT:
			playerX -= speed;
			join.rc = RectMakeCenter(playerX, playerY, join.img->getFrameWidth(), join.img->getFrameHeight());
			break;
		case PLAYER_RIGHT:
			playerX += speed;
			join.rc = RectMakeCenter(playerX, playerY, join.img->getFrameWidth(), join.img->getFrameHeight());
			break;
		case PLAYER_UP:
			playerY -= speed;
			join.rc = RectMakeCenter(playerX, playerY, join.img->getFrameWidth(), join.img->getFrameHeight());
			break;
		case PLAYER_DOWN:
			playerY += speed;
			join.rc = RectMakeCenter(playerX, playerY, join.img->getFrameWidth(), join.img->getFrameHeight());
			break;
		}

		if (playerX == stackX && playerY == stackY)
		{
			optimalPath.pop();
		}
	}
}

void Join::playerAstar()
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
			atkList.clear();

			isMove = false;
			isClick = true;

			//공격범위
			for (int j = 0; j < 4; j++)
			{
				join.rcAtk[0] = RectMake(join.rc.left - 48, join.rc.top, TILE_WIDTH, TILE_HEIGHT);
				join.rcAtk[1] = RectMake(join.rc.left + 48, join.rc.top, TILE_WIDTH, TILE_HEIGHT);
				join.rcAtk[2] = RectMake(join.rc.left, join.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				join.rcAtk[3] = RectMake(join.rc.left, join.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				atkList.push_back(join.rcAtk[j]);
			}
		}
	}

	if (isClick)
	{
		//메뉴선택 렉트
		for (int j = 0; j < 5; j++)
		{
			rcMenu[0] = RectMake(join.rc.left - 97, join.rc.top - 30, 82, 20);
			rcMenu[1] = RectMake(join.rc.left - 97, join.rc.top - 9, 82, 20);
			rcMenu[2] = RectMake(join.rc.left - 97, join.rc.top + 12, 82, 20);
			rcMenu[3] = RectMake(join.rc.left - 97, join.rc.top + 38, 82, 20);
			rcMenu[4] = RectMake(join.rc.left - 97, join.rc.top + 63, 82, 20);
			menuList.push_back(rcMenu[j]);
		}
	}
}

void Join::playerMenu()
{
	//메뉴
	if (isClick)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
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

				auto& prevTile = mainMap->getMap()[saveTile];

				join.rc.left = prevTile.rc.left;
				join.rc.right = prevTile.rc.right;
				join.rc.top = prevTile.rc.top;
				join.rc.bottom = prevTile.rc.bottom;
				pDirection = sDirection;

				playerX = prevTile.rc.left + TILE_WIDTH * 0.5;
				playerY = prevTile.rc.top + TILE_HEIGHT * 0.5;

				isSelect = false;
				isMove = true;
				isClick = false;
			}
		}
	}
}

void Join::playerCollision()
{
	isTarget = false;
	frameX = 0;

	for (int j = 0; j < ENEMYMANAGER->getEnemy().size(); j++)
	{
		auto enemyX = ENEMYMANAGER->getEnemy()[j]->getEnemyX();
		auto enemyY = ENEMYMANAGER->getEnemy()[j]->getEnemyY();
		auto enemyHit = ENEMYMANAGER->getEnemy()[j]->getIsHit();
		auto& enemyRect = ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rc;

		for (int k = 0; k < 4; k++)
		{
			if (IntersectRect(&temp, &join.rcAtk[k], &enemyRect))
			{
				isTarget = true;
				frameX = 1;

				if (isAtk && enemyHit)
				{
					if (playerX > enemyX) pDirection = PLAYER_LEFT;
					else if (playerX < enemyX) pDirection = PLAYER_RIGHT;
					else if (playerY > enemyY) pDirection = PLAYER_UP;
					else if (playerY < enemyY) pDirection = PLAYER_DOWN;
				}
			}
		}
	}
}

void Join::playerAnimation()
{
	if (isTurn)
	{
		if (isAtk)
		{
			switch (pDirection)
			{
			case PLAYER_LEFT:
				ANIMATIONMANAGER->addAnimation("leftAtk", "조인ATK", 12, 15, 5, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("leftAtk");
				ANIMATIONMANAGER->resume("leftAtk");
				break;
			case PLAYER_RIGHT:
				ANIMATIONMANAGER->addAnimation("rightAtk", "조인ATK", 8, 11, 5, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("rightAtk");
				ANIMATIONMANAGER->resume("rightAtk");
				break;
			case PLAYER_UP:
				ANIMATIONMANAGER->addAnimation("upAtk", "조인ATK", 4, 7, 5, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("upAtk");
				ANIMATIONMANAGER->resume("upAtk");
				break;
			case PLAYER_DOWN:
				ANIMATIONMANAGER->addAnimation("downAtk", "조인ATK", 0, 3, 5, false, false);
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

			if (currentHp < 40 && !isSelect && isMove)
			{
				ANIMATIONMANAGER->addAnimation("playerHp", "조인", 12, 13, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerHp");
				ANIMATIONMANAGER->resume("playerHp");
			}

			if (isHeal)
			{
				ANIMATIONMANAGER->addDefAnimation("healImg", "heal", 30, false, false);
				skillAni = ANIMATIONMANAGER->findAnimation("healImg");
				ANIMATIONMANAGER->resume("healImg");
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

		if (isHeal)
		{
			ANIMATIONMANAGER->addDefAnimation("healImg", "heal", 30, false, false);
			skillAni = ANIMATIONMANAGER->findAnimation("healImg");
			ANIMATIONMANAGER->resume("healImg");
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

	//HP
	if (currentHp < 0) currentHp = 0;
	if (currentHp > maxHp) currentHp = maxHp;

	//MP
	if (currentMp < 0) currentMp = 0;

	//EXP
	if (currentExp >= 100)
	{
		currentExp -= 100;

		join.level += 1;		//레벨
		maxHp += 3;				//체력
		maxMp += 3;				//마력
		join.atk += 3;			//공격력
		join.will += 3;			//정신력
		join.def += 3;			//방어력
		join.agi += 3;			//순발력

		isLevelUp = true;
	}

	//Level
	if (isLevelUp)
	{
		levelCount++;

		if (levelCount > 50)
		{
			levelCount = 0;
			isLevelUp = false;
		}
	}
}

void Join::setPosition(RECT rc)
{
	join.rc = rc;
	playerX = join.rc.left + (join.rc.right - join.rc.left) / 2;
	playerY = join.rc.top + (join.rc.bottom - join.rc.top) / 2;
}
