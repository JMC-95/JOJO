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
	//�̸� �� ��
	jangbi.name = "���";
	jangbi.face = "���Face";
	jangbi.className = "��⺴";
	//�̹��� �� �ִϸ��̼�
	jangbi.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//ĳ���� Ŭ���� �̵����� �̹���
	jangbi.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//ĳ���� Ŭ���� ���ݹ��� �̹���
	jangbi.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//���ݹ�ư Ŭ���� ���ݹ��� �̹���
	jangbi.img = IMAGEMANAGER->findImage(friendImg);			//ĳ���� �̹���
	jangbi.atkImg = IMAGEMANAGER->findImage(atkImg);			//���� �̹���
	jangbi.blockImg = IMAGEMANAGER->findImage(blockImg);		//��� �� �ǰ� �̹���
	ANIMATIONMANAGER->addAnimation("friendLeft", "���", 4, 5, 2, false, true);
	friendAni = ANIMATIONMANAGER->findAnimation("friendLeft");
	//�������ͽ�
	jangbi.level = 6;		//����
	jangbi.hp = 125;		//ü��
	jangbi.mp = 15;			//����
	jangbi.atk = 70;		//���ݷ�
	jangbi.will = 49;		//���ŷ�
	jangbi.def = 60;		//����
	jangbi.agi = 49;		//���߷�
	jangbi.ten = 64;		//���
	jangbi.movingCount = 6;	//�̵���
	//������
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

	//ĳ���� ���� �� ��ġ
	fDirection = FRIEND_LEFT;
	startTile = endTile = -1;
	speed = 12;	//�ӵ�

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

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
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

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
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
				//������ Ÿ�� (ĳ����)
				startTile = i;

				isSelect = true;
				isFind = false;
				noPath = false;
				startAstar = false;

				//���ݹ���
				for (int j = 0; j < 4; j++)
				{
					jangbi.rcAtk[0] = RectMake(jangbi.rc.left - 48, jangbi.rc.top, TILE_WIDTH, TILE_HEIGHT);
					jangbi.rcAtk[1] = RectMake(jangbi.rc.left + 48, jangbi.rc.top, TILE_WIDTH, TILE_HEIGHT);
					jangbi.rcAtk[2] = RectMake(jangbi.rc.left, jangbi.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					jangbi.rcAtk[3] = RectMake(jangbi.rc.left, jangbi.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(jangbi.rcAtk[j]);
				}

				//�̵�����
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

					//������ ���� x��ǥ�� y��ǥ
					mapX = mainMap->getMap()[i].rc.left + (mainMap->getMap()[i].rc.right - mainMap->getMap()[i].rc.left) / 2;
					mapY = mainMap->getMap()[i].rc.top + (mainMap->getMap()[i].rc.bottom - mainMap->getMap()[i].rc.top) / 2;

					//������ Ÿ�� (��ǥ)
					endTile = i;

					//�̼��� Astar�� ���۵ȴ�.
					//Astar�� �ʿ��� ��� ���� �ʱ�ȭ ��������.
					openList.clear();
					closeList.clear();

					if (startTile != -1 && endTile != -1)
					{
						startAstar = true;
						currentTile = startTile;

						//���� ������ openList�� ����
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
	//��ǥ Ÿ���� Ŭ���ϸ� A* ����
	if (startAstar && !isFind && !noPath)
	{
		while (!isFind)
		{
			aStar();
		}
	}

	//��ǥ Ÿ���� Ŭ���ϸ� ĳ���� �̵�
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

			//���ݹ���
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
		//�޴����� ��Ʈ
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
	//�޴�
	if (isClick)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			if (PtInRect(&rcMenu[0], m_ptMouse) && isTarget)	//����
			{
				atkList.clear();
				menuList.clear();

				isAtkRng = true;
				isClick = false;
			}
			if (PtInRect(&rcMenu[1], m_ptMouse))	//å��
			{
				atkList.clear();
				menuList.clear();
			}
			if (PtInRect(&rcMenu[2], m_ptMouse))	//����
			{
				atkList.clear();
				menuList.clear();
			}
			if (PtInRect(&rcMenu[3], m_ptMouse))	//���
			{
				atkList.clear();
				menuList.clear();

				isMove = true;
				isTurn = false;
				isSelect = false;
				isClick = false;
			}
			if (PtInRect(&rcMenu[4], m_ptMouse))	//���
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
				ANIMATIONMANAGER->addAnimation("leftAtk", "���ATK", 12, 15, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("leftAtk");
				ANIMATIONMANAGER->resume("leftAtk");
				break;
			case FRIEND_RIGHT:
				ANIMATIONMANAGER->addAnimation("rightAtk", "���ATK", 8, 11, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("rightAtk");
				ANIMATIONMANAGER->resume("rightAtk");
				break;
			case FRIEND_UP:
				ANIMATIONMANAGER->addAnimation("upAtk", "���ATK", 4, 7, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("upAtk");
				ANIMATIONMANAGER->resume("upAtk");
				break;
			case FRIEND_DOWN:
				ANIMATIONMANAGER->addAnimation("downAtk", "���ATK", 0, 3, 5, false, false);
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
				ANIMATIONMANAGER->addAnimation("friendLeft", "���", 4, 5, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendLeft");
				ANIMATIONMANAGER->resume("friendLeft");
				break;
			case FRIEND_RIGHT:
				ANIMATIONMANAGER->addAnimation("friendRight", "���", 6, 7, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendRight");
				ANIMATIONMANAGER->resume("friendRight");
				break;
			case FRIEND_UP:
				ANIMATIONMANAGER->addAnimation("friendUp", "���", 2, 3, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendUp");
				ANIMATIONMANAGER->resume("friendUp");
				break;
			case FRIEND_DOWN:
				ANIMATIONMANAGER->addAnimation("friendDown", "���", 0, 1, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendDown");
				ANIMATIONMANAGER->resume("friendDown");
				break;
			}

			if (currentHp < 40 && !isSelect && isMove)
			{
				ANIMATIONMANAGER->addAnimation("friendHp", "���", 12, 13, 2, false, true);
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
	//�챺 ��ġ
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
				//������ Ÿ�� (ĳ����)
				startTile = i;

				//ĳ���� Ŭ��
				isSelect = true;

				//���ݹ���
				for (int j = 0; j < 4; j++)
				{
					jangbi.rcAtk[0] = RectMake(jangbi.rc.left - 48, jangbi.rc.top, TILE_WIDTH, TILE_HEIGHT);
					jangbi.rcAtk[1] = RectMake(jangbi.rc.left + 48, jangbi.rc.top, TILE_WIDTH, TILE_HEIGHT);
					jangbi.rcAtk[2] = RectMake(jangbi.rc.left, jangbi.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					jangbi.rcAtk[3] = RectMake(jangbi.rc.left, jangbi.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(jangbi.rcAtk[j]);
				}

				//�̵�����
				if (isMove) floodFill(startTile, jangbi.movingCount);
			}
		}

		if (PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				//ĳ���� Ŭ��
				isSelect = false;

				//���ݹ���
				atkList.clear();

				//�̵�����
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
