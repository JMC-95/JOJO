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
	//�̸� �� ��
	yubi.name = "����";
	yubi.face = "����Face";
	yubi.className = "����";
	//�̹��� �� �ִϸ��̼�
	yubi.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//ĳ���� Ŭ���� �̵����� �̹���
	yubi.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//ĳ���� Ŭ���� ���ݹ��� �̹���
	yubi.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//���ݹ�ư Ŭ���� ���ݹ��� �̹���
	yubi.img = IMAGEMANAGER->findImage(friendImg);			//ĳ���� �̹���
	yubi.atkImg = IMAGEMANAGER->findImage(atkImg);			//���� �̹���
	yubi.blockImg = IMAGEMANAGER->findImage(blockImg);		//��� �� �ǰ� �̹���
	ANIMATIONMANAGER->addAnimation("friendLeft", "����", 4, 5, 2, false, true);
	friendAni = ANIMATIONMANAGER->findAnimation("friendLeft");
	//�������ͽ�
	yubi.level = 6;			//����
	yubi.hp = 125;			//ü��
	yubi.mp = 35;			//����
	yubi.atk = 54;			//���ݷ�
	yubi.will = 56;			//���ŷ�
	yubi.def = 56;			//����
	yubi.agi = 51;			//���߷�
	yubi.ten = 65;			//���
	yubi.movingCount = 6;	//�̵���
	//������
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

	//ĳ���� ���� �� ��ġ
	fDirection = FRIEND_LEFT;
	startTile = endTile = -1;
	speed = 12;	//�ӵ�

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

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
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

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
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
				//������ Ÿ�� (ĳ����)
				startTile = i;

				isSelect = true;
				isFind = false;
				noPath = false;
				startAstar = false;

				//���ݹ���
				for (int j = 0; j < 4; j++)
				{
					yubi.rcAtk[0] = RectMake(yubi.rc.left - 48, yubi.rc.top, TILE_WIDTH, TILE_HEIGHT);
					yubi.rcAtk[1] = RectMake(yubi.rc.left + 48, yubi.rc.top, TILE_WIDTH, TILE_HEIGHT);
					yubi.rcAtk[2] = RectMake(yubi.rc.left, yubi.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					yubi.rcAtk[3] = RectMake(yubi.rc.left, yubi.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(yubi.rcAtk[j]);
				}

				//�̵�����
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
		//�޴����� ��Ʈ
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
				ANIMATIONMANAGER->addAnimation("leftAtk", "����ATK", 12, 15, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("leftAtk");
				ANIMATIONMANAGER->resume("leftAtk");
				break;
			case FRIEND_RIGHT:
				ANIMATIONMANAGER->addAnimation("rightAtk", "����ATK", 8, 11, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("rightAtk");
				ANIMATIONMANAGER->resume("rightAtk");
				break;
			case FRIEND_UP:
				ANIMATIONMANAGER->addAnimation("upAtk", "����ATK", 4, 7, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("upAtk");
				ANIMATIONMANAGER->resume("upAtk");
				break;
			case FRIEND_DOWN:
				ANIMATIONMANAGER->addAnimation("downAtk", "����ATK", 0, 3, 5, false, false);
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
				ANIMATIONMANAGER->addAnimation("friendLeft", "����", 4, 5, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendLeft");
				ANIMATIONMANAGER->resume("friendLeft");
				break;
			case FRIEND_RIGHT:
				ANIMATIONMANAGER->addAnimation("friendRight", "����", 6, 7, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendRight");
				ANIMATIONMANAGER->resume("friendRight");
				break;
			case FRIEND_UP:
				ANIMATIONMANAGER->addAnimation("friendUp", "����", 2, 3, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendUp");
				ANIMATIONMANAGER->resume("friendUp");
				break;
			case FRIEND_DOWN:
				ANIMATIONMANAGER->addAnimation("friendDown", "����", 0, 1, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendDown");
				ANIMATIONMANAGER->resume("friendDown");
				break;
			}

			if (currentHp < 40 && !isSelect && isMove)
			{
				ANIMATIONMANAGER->addAnimation("friendHp", "����", 12, 13, 2, false, true);
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
				//������ Ÿ�� (ĳ����)
				startTile = i;

				//ĳ���� Ŭ��
				isSelect = true;

				//���ݹ���
				for (int j = 0; j < 4; j++)
				{
					yubi.rcAtk[0] = RectMake(yubi.rc.left - 48, yubi.rc.top, TILE_WIDTH, TILE_HEIGHT);
					yubi.rcAtk[1] = RectMake(yubi.rc.left + 48, yubi.rc.top, TILE_WIDTH, TILE_HEIGHT);
					yubi.rcAtk[2] = RectMake(yubi.rc.left, yubi.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					yubi.rcAtk[3] = RectMake(yubi.rc.left, yubi.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(yubi.rcAtk[j]);
				}

				//�̵�����
				if (isMove) floodFill(startTile, yubi.movingCount);
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

void Yubi::setPosition(RECT rc)
{
	yubi.rc = rc;
	friendX = yubi.rc.left + (yubi.rc.right - yubi.rc.left) / 2;
	friendY = yubi.rc.top + (yubi.rc.bottom - yubi.rc.top) / 2;
}
