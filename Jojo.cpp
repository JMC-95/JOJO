#include "stdafx.h"
#include "Jojo.h"

Jojo::Jojo()
{
}

Jojo::~Jojo()
{
}

HRESULT Jojo::init(const char* skillImg, const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * playerImg, const char * atkImg, const char * blockImg)
{
	//이름 및 얼굴
	jojo.name = "조조";
	jojo.face = "조조Face";
	jojo.className = "군웅";
	//이미지 및 애니메이션
	jojo.skillImg = IMAGEMANAGER->findImage(skillImg);		//스킬 이미지
	jojo.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//캐릭터 클릭시 이동범위 이미지
	jojo.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//캐릭터 클릭시 공격범위 이미지
	jojo.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//공격버튼 클릭시 공격범위 이미지
	jojo.img = IMAGEMANAGER->findImage(playerImg);			//캐릭터 이미지
	jojo.atkImg = IMAGEMANAGER->findImage(atkImg);			//공격 이미지
	jojo.blockImg = IMAGEMANAGER->findImage(blockImg);		//방어 및 피격 이미지
	ANIMATIONMANAGER->addAnimation("playerLeft", "조조", 4, 5, 2, false, true);
	playerAni = ANIMATIONMANAGER->findAnimation("playerLeft");
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
	//아이템
	jojo.weaponName = ITEMMANAGER->getWeapon()[0].name;
	jojo.armorName = ITEMMANAGER->getArmor()[0].name;
	jojo.weaponImg = ITEMMANAGER->getWeapon()[0].itemImage;
	jojo.armorImg = ITEMMANAGER->getArmor()[0].itemImage;
	jojo.addAtk = ITEMMANAGER->getWeapon()[0].power;
	jojo.addDef = ITEMMANAGER->getArmor()[0].power;

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
	currentExp = 57;
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
		if (PLAYERMANAGER->getPturn())
		{
			auto& playerVector = PLAYERMANAGER->getPlayer();

			bool isMouseMove = true;

			for (int i = 0; i < playerVector.size(); ++i)
			{
				if (playerVector[i]->getPlayerInfo().name == "조조")
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
				mouseMove();
			}
		}
	}

	playerAnimation();
	playerState();

	if (KEYMANAGER->isOnceKeyDown(VK_DOWN)) currentHp = 0;

	if (KEYMANAGER->isOnceKeyDown(VK_LEFT)) isTurn = true;
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
			if (isHeal)
			{
				jojo.blockImg->frameRender(hdc, jojo.rc.left, jojo.rc.top, 0, 5);
			}
			else
			{
				jojo.img->aniRender(hdc, jojo.rc.left, jojo.rc.top, playerAni);
			}
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
					jojo.rcAtk[0] = RectMake(jojo.rc.left - 48, jojo.rc.top, TILE_WIDTH, TILE_HEIGHT);
					jojo.rcAtk[1] = RectMake(jojo.rc.left + 48, jojo.rc.top, TILE_WIDTH, TILE_HEIGHT);
					jojo.rcAtk[2] = RectMake(jojo.rc.left, jojo.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					jojo.rcAtk[3] = RectMake(jojo.rc.left, jojo.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(jojo.rcAtk[j]);
				}

				//이동범위
				if (isMove)
				{
					floodFill(startTile, jojo.movingCount);

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

		if (!PtInRect(&jojo.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
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

		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			if (mainMap->getMap()[i].flood) mainMap->getMap()[i].flood = false;
		}
	}

	playerAstar();
	playerCollision();
	playerMenu();
}

void Jojo::playerMove()
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
				jojo.rcAtk[0] = RectMake(jojo.rc.left - 48, jojo.rc.top, TILE_WIDTH, TILE_HEIGHT);
				jojo.rcAtk[1] = RectMake(jojo.rc.left + 48, jojo.rc.top, TILE_WIDTH, TILE_HEIGHT);
				jojo.rcAtk[2] = RectMake(jojo.rc.left, jojo.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				jojo.rcAtk[3] = RectMake(jojo.rc.left, jojo.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				atkList.push_back(jojo.rcAtk[j]);
			}
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

void Jojo::playerMenu()
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

				skillRect[0] = RectMake(jojo.rc.left - 88, jojo.rc.top + 83, 73, 19);
				skillRect[1] = RectMake(jojo.rc.left - 204, jojo.rc.top - 9, 189, 18);
				vSkill.push_back(skillRect[0]);
				vSkill.push_back(skillRect[1]);

				isSkill = true;
				isClick = false;
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

				jojo.rc.left = prevTile.rc.left;
				jojo.rc.right = prevTile.rc.right;
				jojo.rc.top = prevTile.rc.top;
				jojo.rc.bottom = prevTile.rc.bottom;
				pDirection = sDirection;

				playerX = prevTile.rc.left + TILE_WIDTH * 0.5;
				playerY = prevTile.rc.top + TILE_HEIGHT * 0.5;

				isMove = true;
				isSelect = false;
				isClick = false;
			}
		}
	}

	//스킬
	if (isSkill)
	{
		if (PtInRect(&skillRect[0], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("click", 1.0f);
			vSkill.clear();
			isClick = true;
			isSkill = false;
		}
		else if (PtInRect(&skillRect[1], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON) && currentMp >= 6)
		{
			SOUNDMANAGER->play("click", 1.0f);
			vSkill.clear();
			isSkillCheck = true;
			isSkill = false;

			int positionX = jojo.rc.left / TILE_WIDTH;
			int positionY = jojo.rc.top / TILE_HEIGHT;
			int playerTile = positionX + (positionY * TILE_Y);

			if (isSkillCheck) skillFill(playerTile, 4);
		}
	}

	if (isSkillCheck)
	{
		for (int i = 1; i < PLAYERMANAGER->getPlayer().size(); i++)
		{
			auto& player = PLAYERMANAGER->getPlayer()[i];
			auto& playerRect = PLAYERMANAGER->getPlayer()[i]->getPlayerInfo().rc;

			if (PtInRect(&playerRect, m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				SOUNDMANAGER->play("skillStart", 1.0f);
				isHeal = true;
				currentMp -= 6;
				playerNumber = i;

				for (int i = 0; i < TILE_X * TILE_Y; i++)
				{
					mainMap->getMap()[i].flood = false;
					mainMap->getMap()[i].skill = false;
				}
			}
		}

		if (isHeal)
		{
			skillCount++;

			if (skillCount > 50)
			{
				isHealCheck = true;
				isHeal = false;
			}
		}
		else skillCount = 0;
	}

	if (isHealCheck)
	{
		auto& player = PLAYERMANAGER->getPlayer()[playerNumber];

		player->setIsHeal(true);
		frameCount++;

		if (frameCount < 2) SOUNDMANAGER->play("healStart", 1.0f);
		else if (frameCount > 40)
		{
			player->setIsHeal(false);
			if (player->getMaxHp() - player->getCurrentHp() < 30) player->healDamage(player->getMaxHp() - player->getCurrentHp());
			else player->healDamage(30);
			isMove = true;
			isTurn = false;
			isSelect = false;
			isSkillCheck = false;
			isHealCheck = false;
		}
	}
	else frameCount = 0;
}

void Jojo::playerCollision()
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
			if (IntersectRect(&temp, &jojo.rcAtk[k], &enemyRect))
			{
				isTarget = true;
				frameX = 1;
			}

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

			if (currentHp < 40 && !isSelect && isMove)
			{
				ANIMATIONMANAGER->addAnimation("playerHp", "조조", 12, 13, 2, false, true);
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

void Jojo::playerState()
{
	//플레이어 위치
	positionX = jojo.rc.left / TILE_WIDTH;
	positionY = jojo.rc.top / TILE_HEIGHT;
	playerTile = positionX + (positionY * TILE_Y);

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

		jojo.level += 1;		//레벨
		maxHp += 3;				//체력
		maxMp += 3;				//마력
		jojo.atk += 3;			//공격력
		jojo.will += 3;			//정신력
		jojo.def += 3;			//방어력
		jojo.agi += 3;			//순발력

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

void Jojo::setPosition(RECT rc)
{
	jojo.rc = rc;
	playerX = jojo.rc.left + (jojo.rc.right - jojo.rc.left) / 2;
	playerY = jojo.rc.top + (jojo.rc.bottom - jojo.rc.top) / 2;
}
