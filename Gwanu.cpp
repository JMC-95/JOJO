#include "stdafx.h"
#include "Gwanu.h"

Gwanu::Gwanu()
{
}

Gwanu::~Gwanu()
{
}

HRESULT Gwanu::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * friendImg, const char * atkImg, const char * blockImg)
{
	//이름 및 얼굴
	gwanu.name = "관우";
	gwanu.face = "관우Face";
	gwanu.className = "경기병";
	//이미지 및 애니메이션
	gwanu.moveRngImg = IMAGEMANAGER->findImage(moveImg);	//캐릭터 클릭시 이동범위 이미지
	gwanu.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//캐릭터 클릭시 공격범위 이미지
	gwanu.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//공격버튼 클릭시 공격범위 이미지
	gwanu.img = IMAGEMANAGER->findImage(friendImg);			//캐릭터 이미지
	gwanu.atkImg = IMAGEMANAGER->findImage(atkImg);			//공격 이미지
	gwanu.blockImg = IMAGEMANAGER->findImage(blockImg);		//방어 및 피격 이미지
	ANIMATIONMANAGER->addAnimation("friendLeft", "관우", 4, 5, 2, false, true);
	friendAni = ANIMATIONMANAGER->findAnimation("friendLeft");
	//스테이터스
	gwanu.level = 6;		//레벨
	gwanu.hp = 132;			//체력
	gwanu.mp = 17;			//마력
	gwanu.atk = 70;			//공격력
	gwanu.will = 44;		//정신력
	gwanu.def = 57;			//방어력
	gwanu.agi = 61;			//순발력
	gwanu.ten = 61;			//사기
	gwanu.movingCount = 6;	//이동력
	//아이템
	gwanu.weaponName = ITEMMANAGER->getWeapon()[1].name;
	gwanu.armorName = ITEMMANAGER->getArmor()[0].name;
	gwanu.weaponImg = ITEMMANAGER->getWeapon()[1].itemImage;
	gwanu.armorImg = ITEMMANAGER->getArmor()[0].itemImage;
	gwanu.addAtk = ITEMMANAGER->getWeapon()[1].power;
	gwanu.addDef = ITEMMANAGER->getArmor()[0].power;

	//HP ProgressBar
	_Hp = new progressBar;
	_Hp->init("images/UI/Info/HP.bmp", "images/UI/Info/Back_P.bmp", 1056, 289, 84, 12);
	_Hp->setGauge(currentHp, maxHp);
	currentHp = maxHp = 132;

	//MP ProgressBar
	_Mp = new progressBar;
	_Mp->init("images/UI/Info/MP.bmp", "images/UI/Info/Back_P.bmp", 1056, 307, 84, 12);
	_Mp->setGauge(currentMp, maxMp);
	currentMp = maxMp = 17;

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

void Gwanu::release()
{
	SAFE_DELETE(_Hp);
	SAFE_DELETE(_Mp);
	SAFE_DELETE(_Exp);
}

void Gwanu::update()
{
	if (isTurn)
	{
		if (FRIENDMANAGER->getFturn()) friendAi();
	}

	friendAnimation();
	friendState();
	mouseMove();
}

void Gwanu::render(HDC hdc)
{
	if (isTurn)
	{
		if (isAtk)
		{
			gwanu.atkImg->aniRender(hdc, gwanu.rc.left - 8, gwanu.rc.top - 8, friendAni);
		}
		else if (isHit)
		{
			gwanu.blockImg->frameRender(hdc, gwanu.rc.left, gwanu.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, gwanu.rc.left, gwanu.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			gwanu.img->aniRender(hdc, gwanu.rc.left, gwanu.rc.top, friendAni);
		}
	}
	else
	{
		if (isHit)
		{
			gwanu.blockImg->frameRender(hdc, gwanu.rc.left, gwanu.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, gwanu.rc.left, gwanu.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			gwanu.img->frameAlphaRender(hdc, gwanu.rc.left, gwanu.rc.top, 0, frameY, 100);
		}
	}
}

void Gwanu::friendAi()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&gwanu.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
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
					gwanu.rcAtk[0] = RectMake(gwanu.rc.left - 48, gwanu.rc.top, TILE_WIDTH, TILE_HEIGHT);
					gwanu.rcAtk[1] = RectMake(gwanu.rc.left + 48, gwanu.rc.top, TILE_WIDTH, TILE_HEIGHT);
					gwanu.rcAtk[2] = RectMake(gwanu.rc.left, gwanu.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					gwanu.rcAtk[3] = RectMake(gwanu.rc.left, gwanu.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(gwanu.rcAtk[j]);
				}

				//이동범위
				if (isMove)
				{
					floodFill(startTile, gwanu.movingCount);

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

		if (!PtInRect(&gwanu.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
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

	if (isSelect && PtInRect(&gwanu.rc, m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
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

void Gwanu::friendMove()
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

	if (gwanu.rc.left > 0 || gwanu.rc.right < WINSIZEY ||
		gwanu.rc.top > 0 || gwanu.rc.bottom < WINSIZEY)
	{
		switch (fDirection)
		{
		case FRIEND_LEFT:
			friendX -= speed;
			gwanu.rc = RectMakeCenter(friendX, friendY, gwanu.img->getFrameWidth(), gwanu.img->getFrameHeight());
			break;
		case FRIEND_RIGHT:
			friendX += speed;
			gwanu.rc = RectMakeCenter(friendX, friendY, gwanu.img->getFrameWidth(), gwanu.img->getFrameHeight());
			break;
		case FRIEND_UP:
			friendY -= speed;
			gwanu.rc = RectMakeCenter(friendX, friendY, gwanu.img->getFrameWidth(), gwanu.img->getFrameHeight());
			break;
		case FRIEND_DOWN:
			friendY += speed;
			gwanu.rc = RectMakeCenter(friendX, friendY, gwanu.img->getFrameWidth(), gwanu.img->getFrameHeight());
			break;
		}

		if (friendX == stackX && friendY == stackY)
		{
			optimalPath.pop();
		}
	}
}

void Gwanu::friendAstar()
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
				gwanu.rcAtk[0] = RectMake(gwanu.rc.left - 48, gwanu.rc.top, TILE_WIDTH, TILE_HEIGHT);
				gwanu.rcAtk[1] = RectMake(gwanu.rc.left + 48, gwanu.rc.top, TILE_WIDTH, TILE_HEIGHT);
				gwanu.rcAtk[2] = RectMake(gwanu.rc.left, gwanu.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				gwanu.rcAtk[3] = RectMake(gwanu.rc.left, gwanu.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				atkList.push_back(gwanu.rcAtk[j]);
			}
		}
	}

	if (isClick)
	{
		//메뉴선택 렉트
		for (int j = 0; j < 5; j++)
		{
			rcMenu[0] = RectMake(gwanu.rc.left - 97, gwanu.rc.top - 30, 82, 20);
			rcMenu[1] = RectMake(gwanu.rc.left - 97, gwanu.rc.top - 9, 82, 20);
			rcMenu[2] = RectMake(gwanu.rc.left - 97, gwanu.rc.top + 12, 82, 20);
			rcMenu[3] = RectMake(gwanu.rc.left - 97, gwanu.rc.top + 38, 82, 20);
			rcMenu[4] = RectMake(gwanu.rc.left - 97, gwanu.rc.top + 63, 82, 20);
			menuList.push_back(rcMenu[j]);
		}
	}
}

void Gwanu::friendMenu()
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

void Gwanu::friendCollision()
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
			if (IntersectRect(&temp, &gwanu.rcAtk[k], &enemyRect))
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

void Gwanu::friendAnimation()
{
	if (isTurn)
	{
		if (isAtk)
		{
			switch (fDirection)
			{
			case FRIEND_LEFT:
				ANIMATIONMANAGER->addAnimation("leftAtk", "관우ATK", 12, 15, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("leftAtk");
				ANIMATIONMANAGER->resume("leftAtk");
				break;
			case FRIEND_RIGHT:
				ANIMATIONMANAGER->addAnimation("rightAtk", "관우ATK", 8, 11, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("rightAtk");
				ANIMATIONMANAGER->resume("rightAtk");
				break;
			case FRIEND_UP:
				ANIMATIONMANAGER->addAnimation("upAtk", "관우ATK", 4, 7, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("upAtk");
				ANIMATIONMANAGER->resume("upAtk");
				break;
			case FRIEND_DOWN:
				ANIMATIONMANAGER->addAnimation("downAtk", "관우ATK", 0, 3, 5, false, false);
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
				ANIMATIONMANAGER->addAnimation("friendLeft", "관우", 4, 5, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendLeft");
				ANIMATIONMANAGER->resume("friendLeft");
				break;
			case FRIEND_RIGHT:
				ANIMATIONMANAGER->addAnimation("friendRight", "관우", 6, 7, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendRight");
				ANIMATIONMANAGER->resume("friendRight");
				break;
			case FRIEND_UP:
				ANIMATIONMANAGER->addAnimation("friendUp", "관우", 2, 3, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendUp");
				ANIMATIONMANAGER->resume("friendUp");
				break;
			case FRIEND_DOWN:
				ANIMATIONMANAGER->addAnimation("friendDown", "관우", 0, 1, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendDown");
				ANIMATIONMANAGER->resume("friendDown");
				break;
			}

			if (currentHp < 40 && !isSelect && isMove)
			{
				ANIMATIONMANAGER->addAnimation("friendHp", "관우", 12, 13, 2, false, true);
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

void Gwanu::friendState()
{
	//우군 위치
	positionX = gwanu.rc.left / TILE_WIDTH;
	positionY = gwanu.rc.top / TILE_HEIGHT;
	friendTile = positionX + (positionY * TILE_Y);

	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);

	if (currentHp < 0) currentHp = 0;
}

void Gwanu::mouseMove()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && PtInRect(&gwanu.rc, m_ptMouse))
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
					gwanu.rcAtk[0] = RectMake(gwanu.rc.left - 48, gwanu.rc.top, TILE_WIDTH, TILE_HEIGHT);
					gwanu.rcAtk[1] = RectMake(gwanu.rc.left + 48, gwanu.rc.top, TILE_WIDTH, TILE_HEIGHT);
					gwanu.rcAtk[2] = RectMake(gwanu.rc.left, gwanu.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					gwanu.rcAtk[3] = RectMake(gwanu.rc.left, gwanu.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(gwanu.rcAtk[j]);
				}

				//이동범위
				if (isMove) floodFill(startTile, gwanu.movingCount);
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

void Gwanu::setPosition(RECT rc)
{
	gwanu.rc = rc;
	friendX = gwanu.rc.left + (gwanu.rc.right - gwanu.rc.left) / 2;
	friendY = gwanu.rc.top + (gwanu.rc.bottom - gwanu.rc.top) / 2;
}
