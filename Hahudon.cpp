#include "stdafx.h"
#include "Hahudon.h"

Hahudon::Hahudon()
{
}

Hahudon::~Hahudon()
{
}

HRESULT Hahudon::init(const char* skillImg, const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * playerImg, const char * atkImg, const char * blockImg)
{
	//이름 및 얼굴
	hahudon.name = "하후돈";
	hahudon.face = "하후돈Face";
	hahudon.className = "경기병";
	//이미지 및 애니메이션
	hahudon.skillImg = IMAGEMANAGER->findImage(skillImg);		//스킬 이미지
	ANIMATIONMANAGER->addDefAnimation("healImg", "heal", 30, false, false);
	skillAni = ANIMATIONMANAGER->findAnimation("healImg");
	hahudon.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//캐릭터 클릭시 이동범위 이미지
	hahudon.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//캐릭터 클릭시 공격범위 이미지
	hahudon.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//공격버튼 클릭시 공격범위 이미지
	hahudon.img = IMAGEMANAGER->findImage(playerImg);			//캐릭터 이미지
	hahudon.atkImg = IMAGEMANAGER->findImage(atkImg);			//공격 이미지
	hahudon.blockImg = IMAGEMANAGER->findImage(blockImg);		//방어 및 피격 이미지
	ANIMATIONMANAGER->addAnimation("playerLeft", "하후돈", 4, 5, 2, false, true);
	playerAni = ANIMATIONMANAGER->findAnimation("playerLeft");
	//스테이터스
	hahudon.level = 6;			//레벨
	hahudon.hp = 136;			//체력
	hahudon.mp = 16;			//마력
	hahudon.atk = 73;			//공격력
	hahudon.will = 44;			//정신력
	hahudon.def = 59;			//방어력
	hahudon.agi = 63;			//순발력
	hahudon.ten = 45;			//사기
	hahudon.movingCount = 6;	//이동력
	//아이템
	hahudon.weaponName = ITEMMANAGER->getWeapon()[1].name;
	hahudon.armorName = ITEMMANAGER->getArmor()[0].name;
	hahudon.weaponImg = ITEMMANAGER->getWeapon()[1].itemImage;
	hahudon.armorImg = ITEMMANAGER->getArmor()[0].itemImage;
	hahudon.addAtk = ITEMMANAGER->getWeapon()[1].power;
	hahudon.addDef = ITEMMANAGER->getArmor()[0].power;

	//HP ProgressBar
	_Hp = new progressBar;
	_Hp->init("images/UI/Info/HP.bmp", "images/UI/Info/Back_P.bmp", 1056, 289, 84, 12);
	_Hp->setGauge(currentHp, maxHp);
	currentHp = maxHp = 136;

	//MP ProgressBar
	_Mp = new progressBar;
	_Mp->init("images/UI/Info/MP.bmp", "images/UI/Info/Back_P.bmp", 1056, 307, 84, 12);
	_Mp->setGauge(currentMp, maxMp);
	currentMp = maxMp = 16;

	//EXP ProgressBar
	_Exp = new progressBar;
	_Exp->init("images/UI/Info/EXP.bmp", "images/UI/Info/Back_EXP.bmp", 1095, 243, 45, 12);
	_Exp->setGauge(currentExp, maxExp);
	currentExp = 28;
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

void Hahudon::release()
{
}

void Hahudon::update()
{
	if (isTurn)
	{
		if (PLAYERMANAGER->getPturn())
		{
			auto& playerVector = PLAYERMANAGER->getPlayer();

			bool isMouseMove = true;

			for (int i = 0; i < playerVector.size(); ++i)
			{
				if (playerVector[i]->getPlayerInfo().name == "하후돈")
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

void Hahudon::render(HDC hdc)
{
	if (isTurn)
	{
		if (isAtk)
		{
			hahudon.atkImg->aniRender(hdc, hahudon.rc.left - 8, hahudon.rc.top - 8, playerAni);
		}
		else if (isHit)
		{
			hahudon.blockImg->frameRender(hdc, hahudon.rc.left, hahudon.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, hahudon.rc.left, hahudon.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			if (isHeal)
			{
				hahudon.blockImg->frameRender(hdc, hahudon.rc.left, hahudon.rc.top, 0, 5);
				hahudon.skillImg->aniRender(hdc, hahudon.rc.left - 8, hahudon.rc.top - 8, skillAni);

				if (maxHp - currentHp < 30)
				{
					sprintf_s(str, "%d", maxHp - currentHp);
					TextOut(hdc, hahudon.rc.left, hahudon.rc.top, str, strlen(str));
				}
				else
				{
					sprintf_s(str, "%d", 30);
					TextOut(hdc, hahudon.rc.left, hahudon.rc.top, str, strlen(str));
				}
			}
			else
			{
				hahudon.img->aniRender(hdc, hahudon.rc.left, hahudon.rc.top, playerAni);
			}
		}
	}
	else
	{
		if (isHit)
		{
			hahudon.blockImg->frameRender(hdc, hahudon.rc.left, hahudon.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, hahudon.rc.left, hahudon.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			if (isHeal)
			{
				hahudon.blockImg->frameRender(hdc, hahudon.rc.left, hahudon.rc.top, 0, 5);
				hahudon.skillImg->aniRender(hdc, hahudon.rc.left - 8, hahudon.rc.top - 8, skillAni);

				if (maxHp - currentHp < 30)
				{
					sprintf_s(str, "%d", maxHp - currentHp);
					TextOut(hdc, hahudon.rc.left, hahudon.rc.top, str, strlen(str));
				}
				else
				{
					sprintf_s(str, "%d", 30);
					TextOut(hdc, hahudon.rc.left, hahudon.rc.top, str, strlen(str));
				}
			}
			else
			{
				hahudon.img->frameAlphaRender(hdc, hahudon.rc.left, hahudon.rc.top, 0, frameY, 100);
			}
		}
	}
}

void Hahudon::mouseMove()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&hahudon.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
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
					hahudon.rcAtk[0] = RectMake(hahudon.rc.left - 48, hahudon.rc.top, TILE_WIDTH, TILE_HEIGHT);
					hahudon.rcAtk[1] = RectMake(hahudon.rc.left + 48, hahudon.rc.top, TILE_WIDTH, TILE_HEIGHT);
					hahudon.rcAtk[2] = RectMake(hahudon.rc.left, hahudon.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					hahudon.rcAtk[3] = RectMake(hahudon.rc.left, hahudon.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(hahudon.rcAtk[j]);
				}

				//이동범위
				if (isMove)
				{
					floodFill(startTile, hahudon.movingCount);

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

		if (!PtInRect(&hahudon.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
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

void Hahudon::playerMove()
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

	if (hahudon.rc.left > 0 || hahudon.rc.right < WINSIZEY ||
		hahudon.rc.top > 0 || hahudon.rc.bottom < WINSIZEY)
	{
		switch (pDirection)
		{
		case PLAYER_LEFT:
			playerX -= speed;
			hahudon.rc = RectMakeCenter(playerX, playerY, hahudon.img->getFrameWidth(), hahudon.img->getFrameHeight());
			break;
		case PLAYER_RIGHT:
			playerX += speed;
			hahudon.rc = RectMakeCenter(playerX, playerY, hahudon.img->getFrameWidth(), hahudon.img->getFrameHeight());
			break;
		case PLAYER_UP:
			playerY -= speed;
			hahudon.rc = RectMakeCenter(playerX, playerY, hahudon.img->getFrameWidth(), hahudon.img->getFrameHeight());
			break;
		case PLAYER_DOWN:
			playerY += speed;
			hahudon.rc = RectMakeCenter(playerX, playerY, hahudon.img->getFrameWidth(), hahudon.img->getFrameHeight());
			break;
		}

		if (playerX == stackX && playerY == stackY)
		{
			optimalPath.pop();
		}
	}
}

void Hahudon::playerAstar()
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
				hahudon.rcAtk[0] = RectMake(hahudon.rc.left - 48, hahudon.rc.top, TILE_WIDTH, TILE_HEIGHT);
				hahudon.rcAtk[1] = RectMake(hahudon.rc.left + 48, hahudon.rc.top, TILE_WIDTH, TILE_HEIGHT);
				hahudon.rcAtk[2] = RectMake(hahudon.rc.left, hahudon.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				hahudon.rcAtk[3] = RectMake(hahudon.rc.left, hahudon.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				atkList.push_back(hahudon.rcAtk[j]);
			}
		}
	}

	if (isClick)
	{
		//메뉴선택 렉트
		for (int j = 0; j < 5; j++)
		{
			rcMenu[0] = RectMake(hahudon.rc.left - 97, hahudon.rc.top - 30, 82, 20);
			rcMenu[1] = RectMake(hahudon.rc.left - 97, hahudon.rc.top - 9, 82, 20);
			rcMenu[2] = RectMake(hahudon.rc.left - 97, hahudon.rc.top + 12, 82, 20);
			rcMenu[3] = RectMake(hahudon.rc.left - 97, hahudon.rc.top + 38, 82, 20);
			rcMenu[4] = RectMake(hahudon.rc.left - 97, hahudon.rc.top + 63, 82, 20);
			menuList.push_back(rcMenu[j]);
		}
	}
}

void Hahudon::playerMenu()
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

				hahudon.rc.left = prevTile.rc.left;
				hahudon.rc.right = prevTile.rc.right;
				hahudon.rc.top = prevTile.rc.top;
				hahudon.rc.bottom = prevTile.rc.bottom;
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

void Hahudon::playerCollision()
{
	frameX = 0;

	for (int j = 0; j < ENEMYMANAGER->getEnemy().size(); j++)
	{
		auto enemyX = ENEMYMANAGER->getEnemy()[j]->getEnemyX();
		auto enemyY = ENEMYMANAGER->getEnemy()[j]->getEnemyY();
		auto enemyHit = ENEMYMANAGER->getEnemy()[j]->getIsHit();
		auto& enemyRect = ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rc;

		for (int k = 0; k < 4; k++)
		{
			if (IntersectRect(&temp, &hahudon.rcAtk[k], &enemyRect))
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

void Hahudon::playerAnimation()
{
	if (isTurn)
	{
		if (isAtk)
		{
			switch (pDirection)
			{
			case PLAYER_LEFT:
				ANIMATIONMANAGER->addAnimation("leftAtk", "하후돈ATK", 12, 15, 5, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("leftAtk");
				ANIMATIONMANAGER->resume("leftAtk");
				break;
			case PLAYER_RIGHT:
				ANIMATIONMANAGER->addAnimation("rightAtk", "하후돈ATK", 8, 11, 5, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("rightAtk");
				ANIMATIONMANAGER->resume("rightAtk");
				break;
			case PLAYER_UP:
				ANIMATIONMANAGER->addAnimation("upAtk", "하후돈ATK", 4, 7, 5, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("upAtk");
				ANIMATIONMANAGER->resume("upAtk");
				break;
			case PLAYER_DOWN:
				ANIMATIONMANAGER->addAnimation("downAtk", "하후돈ATK", 0, 3, 5, false, false);
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
				ANIMATIONMANAGER->addAnimation("playerLeft", "하후돈", 4, 5, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerLeft");
				ANIMATIONMANAGER->resume("playerLeft");
				break;
			case PLAYER_RIGHT:
				ANIMATIONMANAGER->addAnimation("playerRight", "하후돈", 6, 7, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerRight");
				ANIMATIONMANAGER->resume("playerRight");
				break;
			case PLAYER_UP:
				ANIMATIONMANAGER->addAnimation("playerUp", "하후돈", 2, 3, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerUp");
				ANIMATIONMANAGER->resume("playerUp");
				break;
			case PLAYER_DOWN:
				ANIMATIONMANAGER->addAnimation("playerDown", "하후돈", 0, 1, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerDown");
				ANIMATIONMANAGER->resume("playerDown");
				break;
			}

			if (currentHp < 40 && !isSelect && isMove)
			{
				ANIMATIONMANAGER->addAnimation("playerHp", "하후돈", 12, 13, 2, false, true);
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

void Hahudon::playerState()
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

		hahudon.level += 1;		//레벨
		maxHp += 3;				//체력
		maxMp += 3;				//마력
		hahudon.atk += 3;		//공격력
		hahudon.will += 3;		//정신력
		hahudon.def += 3;		//방어력
		hahudon.agi += 3;		//순발력

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

void Hahudon::setPosition(RECT rc)
{
	hahudon.rc = rc;
	playerX = hahudon.rc.left + (hahudon.rc.right - hahudon.rc.left) / 2;
	playerY = hahudon.rc.top + (hahudon.rc.bottom - hahudon.rc.top) / 2;
}
