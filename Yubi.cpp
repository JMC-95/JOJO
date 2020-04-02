#include "stdafx.h"
#include "Yubi.h"

Yubi::Yubi()
{
}

Yubi::~Yubi()
{
}

HRESULT Yubi::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * friendImg, const char * atkImg, const char * blockImg)
{
	//이름 및 얼굴
	yubi.name = "유비";
	yubi.face = "유비Face";
	yubi.className = "군웅";
	//이미지 및 애니메이션
	yubi.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//캐릭터 클릭시 이동범위 이미지
	yubi.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//캐릭터 클릭시 공격범위 이미지
	yubi.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//공격버튼 클릭시 공격범위 이미지
	yubi.img = IMAGEMANAGER->findImage(friendImg);			//캐릭터 이미지
	yubi.atkImg = IMAGEMANAGER->findImage(atkImg);			//공격 이미지
	yubi.blockImg = IMAGEMANAGER->findImage(blockImg);		//방어 및 피격 이미지
	ANIMATIONMANAGER->addAnimation("friendLeft", "유비", 4, 5, 2, false, true);
	friendAni = ANIMATIONMANAGER->findAnimation("friendLeft");
	//스테이터스
	yubi.level = 6;			//레벨
	yubi.hp = 125;			//체력
	yubi.mp = 35;			//마력
	yubi.atk = 54;			//공격력
	yubi.will = 56;			//정신력
	yubi.def = 56;			//방어력
	yubi.agi = 51;			//순발력
	yubi.ten = 65;			//사기
	yubi.movingCount = 6;	//이동력
	//아이템
	yubi.weaponName = ITEMMANAGER->getWeapon()[3].name;
	yubi.armorName = ITEMMANAGER->getArmor()[0].name;
	yubi.weaponImg = ITEMMANAGER->getWeapon()[3].itemImage;
	yubi.armorImg = ITEMMANAGER->getArmor()[0].itemImage;
	yubi.addAtk = ITEMMANAGER->getWeapon()[3].power;
	yubi.addDef = ITEMMANAGER->getArmor()[0].power;

	//HP ProgressBar
	_Hp = new progressBar;
	_Hp->init("images/UI/Info/HP.bmp", "images/UI/Info/Back_P.bmp", 1056, 289, 84, 12);
	_Hp->setGauge(currentHp, maxHp);
	currentHp = maxHp = 125;

	//MP ProgressBar
	_Mp = new progressBar;
	_Mp->init("images/UI/Info/MP.bmp", "images/UI/Info/Back_P.bmp", 1056, 307, 84, 12);
	_Mp->setGauge(currentMp, maxMp);
	currentMp = maxMp = 35;

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

void Yubi::release()
{
	SAFE_DELETE(_Hp);
	SAFE_DELETE(_Mp);
	SAFE_DELETE(_Exp);
}

void Yubi::update()
{
	if (isTurn)
	{
		if (FRIENDMANAGER->getFturn()) friendAi();
	}

	friendAnimation();
	friendState();
	mouseMove();
}

void Yubi::render(HDC hdc)
{
	if (isTurn)
	{
		if (isAtk)
		{
			yubi.atkImg->aniRender(hdc, yubi.rc.left - 8, yubi.rc.top - 8, friendAni);
		}
		else if (isHit)
		{
			yubi.blockImg->frameRender(hdc, yubi.rc.left, yubi.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, yubi.rc.left, yubi.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			yubi.img->aniRender(hdc, yubi.rc.left, yubi.rc.top, friendAni);
		}
	}
	else
	{
		if (isHit)
		{
			yubi.blockImg->frameRender(hdc, yubi.rc.left, yubi.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, yubi.rc.left, yubi.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			yubi.img->frameAlphaRender(hdc, yubi.rc.left, yubi.rc.top, 0, frameY, 100);
		}
	}
}

void Yubi::friendAi()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&yubi.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
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
					yubi.rcAtk[0] = RectMake(yubi.rc.left - 48, yubi.rc.top, TILE_WIDTH, TILE_HEIGHT);
					yubi.rcAtk[1] = RectMake(yubi.rc.left + 48, yubi.rc.top, TILE_WIDTH, TILE_HEIGHT);
					yubi.rcAtk[2] = RectMake(yubi.rc.left, yubi.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					yubi.rcAtk[3] = RectMake(yubi.rc.left, yubi.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(yubi.rcAtk[j]);
				}

				//이동범위
				if (isMove)
				{
					floodFill(startTile, yubi.movingCount);

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

		if (!PtInRect(&yubi.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
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

	friendAstar();
	friendCollision();
	friendMenu();
}

void Yubi::friendMove()
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

	if (yubi.rc.left > 0 || yubi.rc.right < WINSIZEY ||
		yubi.rc.top > 0 || yubi.rc.bottom < WINSIZEY)
	{
		switch (fDirection)
		{
		case FRIEND_LEFT:
			friendX -= speed;
			yubi.rc = RectMakeCenter(friendX, friendY, yubi.img->getFrameWidth(), yubi.img->getFrameHeight());
			break;
		case FRIEND_RIGHT:
			friendX += speed;
			yubi.rc = RectMakeCenter(friendX, friendY, yubi.img->getFrameWidth(), yubi.img->getFrameHeight());
			break;
		case FRIEND_UP:
			friendY -= speed;
			yubi.rc = RectMakeCenter(friendX, friendY, yubi.img->getFrameWidth(), yubi.img->getFrameHeight());
			break;
		case FRIEND_DOWN:
			friendY += speed;
			yubi.rc = RectMakeCenter(friendX, friendY, yubi.img->getFrameWidth(), yubi.img->getFrameHeight());
			break;
		}

		if (friendX == stackX && friendY == stackY)
		{
			optimalPath.pop();
		}
	}
}

void Yubi::friendAstar()
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
				yubi.rcAtk[0] = RectMake(yubi.rc.left - 48, yubi.rc.top, TILE_WIDTH, TILE_HEIGHT);
				yubi.rcAtk[1] = RectMake(yubi.rc.left + 48, yubi.rc.top, TILE_WIDTH, TILE_HEIGHT);
				yubi.rcAtk[2] = RectMake(yubi.rc.left, yubi.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				yubi.rcAtk[3] = RectMake(yubi.rc.left, yubi.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				atkList.push_back(yubi.rcAtk[j]);
			}
		}
	}

	if (isClick)
	{
		//메뉴선택 렉트
		for (int j = 0; j < 5; j++)
		{
			rcMenu[0] = RectMake(yubi.rc.left - 97, yubi.rc.top - 30, 82, 20);
			rcMenu[1] = RectMake(yubi.rc.left - 97, yubi.rc.top - 9, 82, 20);
			rcMenu[2] = RectMake(yubi.rc.left - 97, yubi.rc.top + 12, 82, 20);
			rcMenu[3] = RectMake(yubi.rc.left - 97, yubi.rc.top + 38, 82, 20);
			rcMenu[4] = RectMake(yubi.rc.left - 97, yubi.rc.top + 63, 82, 20);
			menuList.push_back(rcMenu[j]);
		}
	}
}

void Yubi::friendMenu()
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

void Yubi::friendCollision()
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
			if (IntersectRect(&temp, &yubi.rcAtk[k], &enemyRect))
			{
				isTarget = true;
				frameX = 1;
			}
		}

		if (enemyHit)
		{
			if (friendX > enemyX) fDirection = FRIEND_LEFT;
			else if (friendX < enemyX) fDirection = FRIEND_RIGHT;
			else if (friendY > enemyY) fDirection = FRIEND_UP;
			else if (friendY < enemyY) fDirection = FRIEND_DOWN;
		}
	}
}

void Yubi::friendAnimation()
{
	if (isTurn)
	{
		if (isAtk)
		{
			switch (fDirection)
			{
			case FRIEND_LEFT:
				ANIMATIONMANAGER->addAnimation("leftAtk", "유비ATK", 12, 15, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("leftAtk");
				ANIMATIONMANAGER->resume("leftAtk");
				break;
			case FRIEND_RIGHT:
				ANIMATIONMANAGER->addAnimation("rightAtk", "유비ATK", 8, 11, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("rightAtk");
				ANIMATIONMANAGER->resume("rightAtk");
				break;
			case FRIEND_UP:
				ANIMATIONMANAGER->addAnimation("upAtk", "유비ATK", 4, 7, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("upAtk");
				ANIMATIONMANAGER->resume("upAtk");
				break;
			case FRIEND_DOWN:
				ANIMATIONMANAGER->addAnimation("downAtk", "유비ATK", 0, 3, 5, false, false);
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
				ANIMATIONMANAGER->addAnimation("friendLeft", "유비", 4, 5, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendLeft");
				ANIMATIONMANAGER->resume("friendLeft");
				break;
			case FRIEND_RIGHT:
				ANIMATIONMANAGER->addAnimation("friendRight", "유비", 6, 7, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendRight");
				ANIMATIONMANAGER->resume("friendRight");
				break;
			case FRIEND_UP:
				ANIMATIONMANAGER->addAnimation("friendUp", "유비", 2, 3, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendUp");
				ANIMATIONMANAGER->resume("friendUp");
				break;
			case FRIEND_DOWN:
				ANIMATIONMANAGER->addAnimation("friendDown", "유비", 0, 1, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendDown");
				ANIMATIONMANAGER->resume("friendDown");
				break;
			}

			if (currentHp < 40 && !isSelect && isMove)
			{
				ANIMATIONMANAGER->addAnimation("friendHp", "유비", 12, 13, 2, false, true);
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

void Yubi::friendState()
{
	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);

	if (currentHp < 0) currentHp = 0;
}

void Yubi::mouseMove()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && PtInRect(&yubi.rc, m_ptMouse))
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
					yubi.rcAtk[0] = RectMake(yubi.rc.left - 48, yubi.rc.top, TILE_WIDTH, TILE_HEIGHT);
					yubi.rcAtk[1] = RectMake(yubi.rc.left + 48, yubi.rc.top, TILE_WIDTH, TILE_HEIGHT);
					yubi.rcAtk[2] = RectMake(yubi.rc.left, yubi.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					yubi.rcAtk[3] = RectMake(yubi.rc.left, yubi.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(yubi.rcAtk[j]);
				}

				//이동범위
				if (isMove) floodFill(startTile, yubi.movingCount);
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

void Yubi::setPosition(RECT rc)
{
	yubi.rc = rc;
	friendX = yubi.rc.left + (yubi.rc.right - yubi.rc.left) / 2;
	friendY = yubi.rc.top + (yubi.rc.bottom - yubi.rc.top) / 2;
}
