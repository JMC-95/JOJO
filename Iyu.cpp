#include "stdafx.h"
#include "Iyu.h"

Iyu::Iyu()
{
}

Iyu::~Iyu()
{
}

HRESULT Iyu::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * enemyImg, const char * atkImg, const char * blockImg)
{
	//�̸� �� ��
	iyu.name = "����";
	iyu.face = "����Face";
	iyu.className = "å��";
	//�̹��� �� �ִϸ��̼�
	iyu.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//ĳ���� Ŭ���� �̵����� �̹���
	iyu.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//ĳ���� Ŭ���� ���ݹ��� �̹���
	iyu.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//���ݹ�ư Ŭ���� ���ݹ��� �̹���
	iyu.img = IMAGEMANAGER->findImage(enemyImg);			//ĳ���� �̹���
	iyu.atkImg = IMAGEMANAGER->findImage(atkImg);			//���� �̹���
	iyu.blockImg = IMAGEMANAGER->findImage(blockImg);		//��� �� �ǰ� �̹���
	ANIMATIONMANAGER->addAnimation("enemyRight", "����", 6, 7, 2, false, true);
	enemyAni = ANIMATIONMANAGER->findAnimation("enemyRight");
	//�������ͽ�
	iyu.level = 6;			//����
	iyu.hp = 110;			//ü��
	iyu.mp = 50;			//����
	iyu.atk = 18;			//���ݷ�
	iyu.will = 66;			//���ŷ�
	iyu.def = 40;			//����
	iyu.agi = 50;			//���߷�
	iyu.ten = 50;			//���
	iyu.movingCount = 4;	//�̵���
	//������
	iyu.weaponName = ITEMMANAGER->getWeapon()[0].name;
	iyu.armorName = ITEMMANAGER->getArmor()[0].name;
	iyu.weaponImg = ITEMMANAGER->getWeapon()[0].itemImage;
	iyu.armorImg = ITEMMANAGER->getArmor()[0].itemImage;
	iyu.addAtk = ITEMMANAGER->getWeapon()[0].power;
	iyu.addDef = ITEMMANAGER->getArmor()[0].power;

	//HP ProgressBar
	_Hp = new progressBar;
	_Hp->init("images/UI/Info/HP.bmp", "images/UI/Info/Back_P.bmp", 1056, 289, 84, 12);
	_Hp->setGauge(currentHp, maxHp);
	currentHp = maxHp = 110;

	//MP ProgressBar
	_Mp = new progressBar;
	_Mp->init("images/UI/Info/MP.bmp", "images/UI/Info/Back_P.bmp", 1056, 307, 84, 12);
	_Mp->setGauge(currentMp, maxMp);
	currentMp = maxMp = 50;

	//EXP ProgressBar
	_Exp = new progressBar;
	_Exp->init("images/UI/Info/EXP.bmp", "images/UI/Info/Back_EXP.bmp", 1095, 243, 45, 12);
	_Exp->setGauge(currentExp, maxExp);
	currentExp = 0;
	maxExp = 100;

	//ĳ���� ���� �� ��ġ
	eDirection = ENEMY_RIGHT;
	startTile = endTile = -1;
	speed = 12;	//�ӵ�

	isTurn = true;
	isMove = true;
	isSelect = false;

	return S_OK;
}

void Iyu::release()
{
	SAFE_DELETE(_Hp);
	SAFE_DELETE(_Mp);
	SAFE_DELETE(_Exp);
}

void Iyu::update()
{
	if (isTurn)
	{
		if (ENEMYMANAGER->getEturn()) enemyAi();
	}

	enemyAnimation();
	enemyState();
	mouseMove();
}

void Iyu::render(HDC hdc)
{
	if (isTurn)
	{
		if (isAtk)
		{
			iyu.atkImg->aniRender(hdc, iyu.rc.left - 8, iyu.rc.top - 8, enemyAni);
		}
		else if (isHit)
		{
			iyu.blockImg->frameRender(hdc, iyu.rc.left, iyu.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, iyu.rc.left, iyu.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			iyu.img->aniRender(hdc, iyu.rc.left, iyu.rc.top, enemyAni);
		}
	}
	else
	{
		if (isHit)
		{
			iyu.blockImg->frameRender(hdc, iyu.rc.left, iyu.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, iyu.rc.left, iyu.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			iyu.img->frameAlphaRender(hdc, iyu.rc.left, iyu.rc.top, 0, frameY, 100);
		}
	}
}

void Iyu::enemyAi()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&iyu.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
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
					iyu.rcAtk[0] = RectMake(iyu.rc.left - 48, iyu.rc.top, TILE_WIDTH, TILE_HEIGHT);
					iyu.rcAtk[1] = RectMake(iyu.rc.left + 48, iyu.rc.top, TILE_WIDTH, TILE_HEIGHT);
					iyu.rcAtk[2] = RectMake(iyu.rc.left, iyu.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					iyu.rcAtk[3] = RectMake(iyu.rc.left, iyu.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(iyu.rcAtk[j]);
				}

				//�̵�����
				if (isMove)
				{
					floodFill(startTile, iyu.movingCount);

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

		if (!PtInRect(&iyu.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				if (mainMap->getMap()[i].flood)
				{
					SOUNDMANAGER->play("move", 1.0f);

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

	enemyAstar();
	enemyMenu();
	enemyCollision();
}

void Iyu::mouseMove()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && PtInRect(&iyu.rc, m_ptMouse))
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
					iyu.rcAtk[0] = RectMake(iyu.rc.left - 48, iyu.rc.top, TILE_WIDTH, TILE_HEIGHT);
					iyu.rcAtk[1] = RectMake(iyu.rc.left + 48, iyu.rc.top, TILE_WIDTH, TILE_HEIGHT);
					iyu.rcAtk[2] = RectMake(iyu.rc.left, iyu.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					iyu.rcAtk[3] = RectMake(iyu.rc.left, iyu.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(iyu.rcAtk[j]);
				}

				//�̵�����
				if (isMove) floodFill(startTile, iyu.movingCount);
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

void Iyu::enemyMove()
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

	if (iyu.rc.left > 0 || iyu.rc.right < WINSIZEY ||
		iyu.rc.top > 0 || iyu.rc.bottom < WINSIZEY)
	{
		switch (eDirection)
		{
		case ENEMY_LEFT:
			enemyX -= speed;
			iyu.rc = RectMakeCenter(enemyX, enemyY, iyu.img->getFrameWidth(), iyu.img->getFrameHeight());
			break;
		case ENEMY_RIGHT:
			enemyX += speed;
			iyu.rc = RectMakeCenter(enemyX, enemyY, iyu.img->getFrameWidth(), iyu.img->getFrameHeight());
			break;
		case ENEMY_UP:
			enemyY -= speed;
			iyu.rc = RectMakeCenter(enemyX, enemyY, iyu.img->getFrameWidth(), iyu.img->getFrameHeight());
			break;
		case ENEMY_DOWN:
			enemyY += speed;
			iyu.rc = RectMakeCenter(enemyX, enemyY, iyu.img->getFrameWidth(), iyu.img->getFrameHeight());
			break;
		}

		if (enemyX == stackX && enemyY == stackY)
		{
			optimalPath.pop();
		}
	}
}

void Iyu::enemyAstar()
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
			enemyMove();
		}

		if (enemyX == mapX && enemyY == mapY)
		{
			isMove = false;
			isClick = true;

			//���ݹ���
			for (int j = 0; j < 4; j++)
			{
				iyu.rcAtk[0] = RectMake(iyu.rc.left - 48, iyu.rc.top, TILE_WIDTH, TILE_HEIGHT);
				iyu.rcAtk[1] = RectMake(iyu.rc.left + 48, iyu.rc.top, TILE_WIDTH, TILE_HEIGHT);
				iyu.rcAtk[2] = RectMake(iyu.rc.left, iyu.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				iyu.rcAtk[3] = RectMake(iyu.rc.left, iyu.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				atkList.push_back(iyu.rcAtk[j]);
			}
		}
	}

	if (isClick)
	{
		//�޴����� ��Ʈ
		for (int j = 0; j < 5; j++)
		{
			rcMenu[0] = RectMake(iyu.rc.left - 97, iyu.rc.top - 30, 82, 20);
			rcMenu[1] = RectMake(iyu.rc.left - 97, iyu.rc.top - 9, 82, 20);
			rcMenu[2] = RectMake(iyu.rc.left - 97, iyu.rc.top + 12, 82, 20);
			rcMenu[3] = RectMake(iyu.rc.left - 97, iyu.rc.top + 38, 82, 20);
			rcMenu[4] = RectMake(iyu.rc.left - 97, iyu.rc.top + 63, 82, 20);
			menuList.push_back(rcMenu[j]);
		}
	}
}

void Iyu::enemyAnimation()
{
	if (isTurn)
	{
		if (isAtk)
		{
			switch (eDirection)
			{
			case ENEMY_LEFT:
				ANIMATIONMANAGER->addAnimation("leftAtk", "����ATK", 12, 15, 5, false, false);
				enemyAni = ANIMATIONMANAGER->findAnimation("leftAtk");
				ANIMATIONMANAGER->resume("leftAtk");
				break;
			case ENEMY_RIGHT:
				ANIMATIONMANAGER->addAnimation("rightAtk", "����ATK", 8, 11, 5, false, false);
				enemyAni = ANIMATIONMANAGER->findAnimation("rightAtk");
				ANIMATIONMANAGER->resume("rightAtk");
				break;
			case ENEMY_UP:
				ANIMATIONMANAGER->addAnimation("upAtk", "����ATK", 4, 7, 5, false, false);
				enemyAni = ANIMATIONMANAGER->findAnimation("upAtk");
				ANIMATIONMANAGER->resume("upAtk");
				break;
			case ENEMY_DOWN:
				ANIMATIONMANAGER->addAnimation("downAtk", "����ATK", 0, 3, 5, false, false);
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
				ANIMATIONMANAGER->addAnimation("enemyLeft", "����", 4, 5, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyLeft");
				ANIMATIONMANAGER->resume("enemyLeft");
				break;
			case ENEMY_RIGHT:
				ANIMATIONMANAGER->addAnimation("enemyRight", "����", 6, 7, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyRight");
				ANIMATIONMANAGER->resume("enemyRight");
				break;
			case ENEMY_UP:
				ANIMATIONMANAGER->addAnimation("enemyUp", "����", 2, 3, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyUp");
				ANIMATIONMANAGER->resume("enemyUp");
				break;
			case ENEMY_DOWN:
				ANIMATIONMANAGER->addAnimation("enemyDown", "����", 0, 1, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyDown");
				ANIMATIONMANAGER->resume("enemyDown");
				break;
			}

			if (currentHp < 40 && !isSelect && isMove)
			{
				ANIMATIONMANAGER->addAnimation("enemyHp", "����", 12, 13, 2, false, true);
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

void Iyu::enemyState()
{
	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);
}

void Iyu::enemyMenu()
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

void Iyu::enemyCollision()
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
			if (IntersectRect(&temp, &iyu.rcAtk[k], &playerRect))
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
			if (IntersectRect(&temp, &iyu.rcAtk[k], &friendRect))
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

void Iyu::setPosition(RECT rc)
{
	iyu.rc = rc;
	enemyX = iyu.rc.left + (iyu.rc.right - iyu.rc.left) / 2;
	enemyY = iyu.rc.top + (iyu.rc.bottom - iyu.rc.top) / 2;
}
