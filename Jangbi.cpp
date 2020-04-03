#include "stdafx.h"
#include "Jangbi.h"

Jangbi::Jangbi()
{
}

Jangbi::~Jangbi()
{
}

HRESULT Jangbi::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * friendImg, const char * atkImg, const char * blockImg)
{
	//이름 및 얼굴
	jangbi.name = "장비";
	jangbi.face = "장비Face";
	jangbi.className = "경기병";
	//이미지 및 애니메이션
	jangbi.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//캐릭터 클릭시 이동범위 이미지
	jangbi.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//캐릭터 클릭시 공격범위 이미지
	jangbi.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//공격버튼 클릭시 공격범위 이미지
	jangbi.img = IMAGEMANAGER->findImage(friendImg);			//캐릭터 이미지
	jangbi.atkImg = IMAGEMANAGER->findImage(atkImg);			//공격 이미지
	jangbi.blockImg = IMAGEMANAGER->findImage(blockImg);		//방어 및 피격 이미지
	ANIMATIONMANAGER->addAnimation("friendLeft", "장비", 4, 5, 2, false, true);
	friendAni = ANIMATIONMANAGER->findAnimation("friendLeft");
	//스테이터스
	jangbi.level = 6;		//레벨
	jangbi.hp = 125;		//체력
	jangbi.mp = 15;			//마력
	jangbi.atk = 70;		//공격력
	jangbi.will = 49;		//정신력
	jangbi.def = 60;		//방어력
	jangbi.agi = 49;		//순발력
	jangbi.ten = 64;		//사기
	jangbi.movingCount = 6;	//이동력
	//아이템
	jangbi.weaponName = ITEMMANAGER->getWeapon()[1].name;
	jangbi.armorName = ITEMMANAGER->getArmor()[0].name;
	jangbi.weaponImg = ITEMMANAGER->getWeapon()[1].itemImage;
	jangbi.armorImg = ITEMMANAGER->getArmor()[0].itemImage;
	jangbi.addAtk = ITEMMANAGER->getWeapon()[1].power;
	jangbi.addDef = ITEMMANAGER->getArmor()[0].power;

	//HP ProgressBar
	_Hp = new progressBar;
	_Hp->init("images/UI/Info/HP.bmp", "images/UI/Info/Back_P.bmp", 1056, 289, 84, 12);
	_Hp->setGauge(currentHp, maxHp);
	currentHp = maxHp = 125;

	//MP ProgressBar
	_Mp = new progressBar;
	_Mp->init("images/UI/Info/MP.bmp", "images/UI/Info/Back_P.bmp", 1056, 307, 84, 12);
	_Mp->setGauge(currentMp, maxMp);
	currentMp = maxMp = 15;

	//EXP ProgressBar
	_Exp = new progressBar;
	_Exp->init("images/UI/Info/EXP.bmp", "images/UI/Info/Back_EXP.bmp", 1095, 243, 45, 12);
	_Exp->setGauge(currentExp, maxExp);
	currentExp = 0;
	maxExp = 100;

	//캐릭터 방향 및 위치
	fDirection = FRIEND_LEFT;
	startTile = endTile = -1;
	speed = 12;	//속도

	isTurn = true;
	isMove = true;
	isSelect = false;

	return S_OK;
}

void Jangbi::release()
{
	SAFE_DELETE(_Hp);
	SAFE_DELETE(_Mp);
	SAFE_DELETE(_Exp);
}

void Jangbi::update()
{
	if (isTurn)
	{
		if (FRIENDMANAGER->getFturn()) friendAi();
	}

	friendAnimation();
	friendState();
	mouseMove();
}

void Jangbi::render(HDC hdc)
{
	if (isTurn)
	{
		if (isAtk)
		{
			jangbi.atkImg->aniRender(hdc, jangbi.rc.left - 8, jangbi.rc.top - 8, friendAni);
		}
		else if (isHit)
		{
			jangbi.blockImg->frameRender(hdc, jangbi.rc.left, jangbi.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, jangbi.rc.left, jangbi.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			jangbi.img->aniRender(hdc, jangbi.rc.left, jangbi.rc.top, friendAni);
		}
	}
	else
	{
		if (isHit)
		{
			jangbi.blockImg->frameRender(hdc, jangbi.rc.left, jangbi.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, jangbi.rc.left, jangbi.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			jangbi.img->frameAlphaRender(hdc, jangbi.rc.left, jangbi.rc.top, 0, frameY, 100);
		}
	}
}

void Jangbi::friendAi()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&jangbi.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
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
					jangbi.rcAtk[0] = RectMake(jangbi.rc.left - 48, jangbi.rc.top, TILE_WIDTH, TILE_HEIGHT);
					jangbi.rcAtk[1] = RectMake(jangbi.rc.left + 48, jangbi.rc.top, TILE_WIDTH, TILE_HEIGHT);
					jangbi.rcAtk[2] = RectMake(jangbi.rc.left, jangbi.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					jangbi.rcAtk[3] = RectMake(jangbi.rc.left, jangbi.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(jangbi.rcAtk[j]);
				}

				//이동범위
				if (isMove)
				{
					floodFill(startTile, jangbi.movingCount);

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

		if (!PtInRect(&jangbi.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
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

	if (isSelect && PtInRect(&jangbi.rc, m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		isClick = true;
		isMove = false;
		isSelect = false;

		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			if (mainMap->getMap()[i].flood) mainMap->getMap()[i].flood = false;
		}
	}

	friendAstar();
	friendCollision();
	friendMenu();
}

void Jangbi::friendMove()
{
	stackX = optimalPath.top().rc.left + (optimalPath.top().rc.right - optimalPath.top().rc.left) / 2;
	stackY = optimalPath.top().rc.top + (optimalPath.top().rc.bottom - optimalPath.top().rc.top) / 2;

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

	if (jangbi.rc.left > 0 || jangbi.rc.right < WINSIZEY ||
		jangbi.rc.top > 0 || jangbi.rc.bottom < WINSIZEY)
	{
		switch (fDirection)
		{
		case FRIEND_LEFT:
			friendX -= speed;
			jangbi.rc = RectMakeCenter(friendX, friendY, jangbi.img->getFrameWidth(), jangbi.img->getFrameHeight());
			break;
		case FRIEND_RIGHT:
			friendX += speed;
			jangbi.rc = RectMakeCenter(friendX, friendY, jangbi.img->getFrameWidth(), jangbi.img->getFrameHeight());
			break;
		case FRIEND_UP:
			friendY -= speed;
			jangbi.rc = RectMakeCenter(friendX, friendY, jangbi.img->getFrameWidth(), jangbi.img->getFrameHeight());
			break;
		case FRIEND_DOWN:
			friendY += speed;
			jangbi.rc = RectMakeCenter(friendX, friendY, jangbi.img->getFrameWidth(), jangbi.img->getFrameHeight());
			break;
		}

		if (friendX == stackX && friendY == stackY)
		{
			optimalPath.pop();
		}
	}
}

void Jangbi::friendAstar()
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
			friendMove();
		}

		if (friendX == mapX && friendY == mapY)
		{
			isMove = false;
			isClick = true;

			//공격범위
			for (int j = 0; j < 4; j++)
			{
				jangbi.rcAtk[0] = RectMake(jangbi.rc.left - 48, jangbi.rc.top, TILE_WIDTH, TILE_HEIGHT);
				jangbi.rcAtk[1] = RectMake(jangbi.rc.left + 48, jangbi.rc.top, TILE_WIDTH, TILE_HEIGHT);
				jangbi.rcAtk[2] = RectMake(jangbi.rc.left, jangbi.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				jangbi.rcAtk[3] = RectMake(jangbi.rc.left, jangbi.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				atkList.push_back(jangbi.rcAtk[j]);
			}
		}
	}

	if (isClick)
	{
		//메뉴선택 렉트
		for (int j = 0; j < 5; j++)
		{
			rcMenu[0] = RectMake(jangbi.rc.left - 97, jangbi.rc.top - 30, 82, 20);
			rcMenu[1] = RectMake(jangbi.rc.left - 97, jangbi.rc.top - 9, 82, 20);
			rcMenu[2] = RectMake(jangbi.rc.left - 97, jangbi.rc.top + 12, 82, 20);
			rcMenu[3] = RectMake(jangbi.rc.left - 97, jangbi.rc.top + 38, 82, 20);
			rcMenu[4] = RectMake(jangbi.rc.left - 97, jangbi.rc.top + 63, 82, 20);
			menuList.push_back(rcMenu[j]);
		}
	}
}

void Jangbi::friendMenu()
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

void Jangbi::friendCollision()
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
			if (IntersectRect(&temp, &jangbi.rcAtk[k], &enemyRect))
			{
				isTarget = true;
				frameX = 1;

				if (isAtk && enemyHit)
				{
					if (friendX > enemyX) fDirection = FRIEND_LEFT;
					else if (friendX < enemyX) fDirection = FRIEND_RIGHT;
					else if (friendY > enemyY) fDirection = FRIEND_UP;
					else if (friendY < enemyY) fDirection = FRIEND_DOWN;
				}
			}
		}
	}
}

void Jangbi::friendAnimation()
{
	if (isTurn)
	{
		if (isAtk)
		{
			switch (fDirection)
			{
			case FRIEND_LEFT:
				ANIMATIONMANAGER->addAnimation("leftAtk", "장비ATK", 12, 15, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("leftAtk");
				ANIMATIONMANAGER->resume("leftAtk");
				break;
			case FRIEND_RIGHT:
				ANIMATIONMANAGER->addAnimation("rightAtk", "장비ATK", 8, 11, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("rightAtk");
				ANIMATIONMANAGER->resume("rightAtk");
				break;
			case FRIEND_UP:
				ANIMATIONMANAGER->addAnimation("upAtk", "장비ATK", 4, 7, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("upAtk");
				ANIMATIONMANAGER->resume("upAtk");
				break;
			case FRIEND_DOWN:
				ANIMATIONMANAGER->addAnimation("downAtk", "장비ATK", 0, 3, 5, false, false);
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
				ANIMATIONMANAGER->addAnimation("friendLeft", "장비", 4, 5, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendLeft");
				ANIMATIONMANAGER->resume("friendLeft");
				break;
			case FRIEND_RIGHT:
				ANIMATIONMANAGER->addAnimation("friendRight", "장비", 6, 7, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendRight");
				ANIMATIONMANAGER->resume("friendRight");
				break;
			case FRIEND_UP:
				ANIMATIONMANAGER->addAnimation("friendUp", "장비", 2, 3, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendUp");
				ANIMATIONMANAGER->resume("friendUp");
				break;
			case FRIEND_DOWN:
				ANIMATIONMANAGER->addAnimation("friendDown", "장비", 0, 1, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendDown");
				ANIMATIONMANAGER->resume("friendDown");
				break;
			}

			if (currentHp < 40 && !isSelect && isMove)
			{
				ANIMATIONMANAGER->addAnimation("friendHp", "장비", 12, 13, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendHp");
				ANIMATIONMANAGER->resume("friendHp");
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

void Jangbi::friendState()
{
	//우군 위치
	positionX = jangbi.rc.left / TILE_WIDTH;
	positionY = jangbi.rc.top / TILE_HEIGHT;
	friendTile = positionX + (positionY * TILE_Y);

	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);

	if (currentHp < 0) currentHp = 0;
}

void Jangbi::mouseMove()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && PtInRect(&jangbi.rc, m_ptMouse))
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
					jangbi.rcAtk[0] = RectMake(jangbi.rc.left - 48, jangbi.rc.top, TILE_WIDTH, TILE_HEIGHT);
					jangbi.rcAtk[1] = RectMake(jangbi.rc.left + 48, jangbi.rc.top, TILE_WIDTH, TILE_HEIGHT);
					jangbi.rcAtk[2] = RectMake(jangbi.rc.left, jangbi.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					jangbi.rcAtk[3] = RectMake(jangbi.rc.left, jangbi.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(jangbi.rcAtk[j]);
				}

				//이동범위
				if (isMove) floodFill(startTile, jangbi.movingCount);
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

void Jangbi::setPosition(RECT rc)
{
	jangbi.rc = rc;
	friendX = jangbi.rc.left + (jangbi.rc.right - jangbi.rc.left) / 2;
	friendY = jangbi.rc.top + (jangbi.rc.bottom - jangbi.rc.top) / 2;
}
