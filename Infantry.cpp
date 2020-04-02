#include "stdafx.h"
#include "Infantry.h"

Infantry::Infantry(int num)
{
	number = num;
}

Infantry::~Infantry()
{
}

HRESULT Infantry::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * enemyImg, const char * atkImg, const char * blockImg)
{
	//�̸� �� ��
	infantry.name = "������";
	infantry.face = "������Face";
	infantry.className = "�溸��";
	//�̹��� �� �ִϸ��̼�
	infantry.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//ĳ���� Ŭ���� �̵����� �̹���
	infantry.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//ĳ���� Ŭ���� ���ݹ��� �̹���
	infantry.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//���ݹ�ư Ŭ���� ���ݹ��� �̹���
	infantry.img = IMAGEMANAGER->findImage(enemyImg);			//ĳ���� �̹���
	infantry.atkImg = IMAGEMANAGER->findImage(atkImg);			//���� �̹���
	infantry.blockImg = IMAGEMANAGER->findImage(blockImg);		//��� �� �ǰ� �̹���
	ANIMATIONMANAGER->addAnimation("enemyRight", "������", 6, 7, 2, false, true);
	enemyAni = ANIMATIONMANAGER->findAnimation("enemyRight");
	//�������ͽ�
	infantry.level = 2;			//����
	infantry.hp = 116;			//ü��
	infantry.mp = 11;			//����
	infantry.atk = 36;			//���ݷ�
	infantry.will = 38;			//���ŷ�
	infantry.def = 43;			//����
	infantry.agi = 32;			//���߷�
	infantry.ten = 32;			//���
	infantry.movingCount = 4;	//�̵���
	//������
	infantry.weaponName = ITEMMANAGER->getWeapon()[0].name;
	infantry.armorName = ITEMMANAGER->getArmor()[0].name;
	infantry.weaponImg = ITEMMANAGER->getWeapon()[0].itemImage;
	infantry.armorImg = ITEMMANAGER->getArmor()[0].itemImage;
	infantry.addAtk = ITEMMANAGER->getWeapon()[0].power;
	infantry.addDef = ITEMMANAGER->getArmor()[0].power;

	//HP ProgressBar
	_Hp = new progressBar;
	_Hp->init("images/UI/Info/HP.bmp", "images/UI/Info/Back_P.bmp", 1056, 289, 84, 12);
	_Hp->setGauge(currentHp, maxHp);
	currentHp = maxHp = 116;

	//MP ProgressBar
	_Mp = new progressBar;
	_Mp->init("images/UI/Info/MP.bmp", "images/UI/Info/Back_P.bmp", 1056, 307, 84, 12);
	_Mp->setGauge(currentMp, maxMp);
	currentMp = maxMp = 11;

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

void Infantry::release()
{
	SAFE_DELETE(_Hp);
	SAFE_DELETE(_Mp);
	SAFE_DELETE(_Exp);
}

void Infantry::update()
{
	if (isTurn)
	{
		if (ENEMYMANAGER->getEturn()) enemyAi();
	}

	enemyAnimation();
	enemyState();
	mouseMove();
}

void Infantry::render(HDC hdc)
{
	if (isTurn)
	{
		if (isAtk)
		{
			infantry.atkImg->aniRender(hdc, infantry.rc.left - 8, infantry.rc.top - 8, enemyAni);
		}
		else if (isHit)
		{
			infantry.blockImg->frameRender(hdc, infantry.rc.left, infantry.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, infantry.rc.left, infantry.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			infantry.img->aniRender(hdc, infantry.rc.left, infantry.rc.top, enemyAni);
		}
	}
	else
	{
		if (isHit)
		{
			infantry.blockImg->frameRender(hdc, infantry.rc.left, infantry.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, infantry.rc.left, infantry.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			infantry.img->frameAlphaRender(hdc, infantry.rc.left, infantry.rc.top, 0, frameY, 100);
		}
	}
}

void Infantry::enemyAi()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&infantry.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
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
				for (int j = 0; j < 8; j++)
				{
					infantry.rcAtk[0] = RectMake(infantry.rc.left - 48, infantry.rc.top, TILE_WIDTH, TILE_HEIGHT);
					infantry.rcAtk[1] = RectMake(infantry.rc.left - 48, infantry.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					infantry.rcAtk[2] = RectMake(infantry.rc.left - 48, infantry.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					infantry.rcAtk[3] = RectMake(infantry.rc.left + 48, infantry.rc.top, TILE_WIDTH, TILE_HEIGHT);
					infantry.rcAtk[4] = RectMake(infantry.rc.left + 48, infantry.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					infantry.rcAtk[5] = RectMake(infantry.rc.left + 48, infantry.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					infantry.rcAtk[6] = RectMake(infantry.rc.left, infantry.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					infantry.rcAtk[7] = RectMake(infantry.rc.left, infantry.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(infantry.rcAtk[j]);
				}

				//�̵�����
				if (isMove)
				{
					floodFill(startTile, infantry.movingCount);
				}
			}
		}

		if (!PtInRect(&infantry.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
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

	enemyAstar();
	enemyMenu();
	enemyCollision();
}

void Infantry::mouseMove()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && PtInRect(&infantry.rc, m_ptMouse))
		{
			if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
			{
				//������ Ÿ�� (ĳ����)
				startTile = i;

				//ĳ���� Ŭ��
				isSelect = true;

				//���ݹ���
				for (int j = 0; j < 8; j++)
				{
					infantry.rcAtk[0] = RectMake(infantry.rc.left - 48, infantry.rc.top, TILE_WIDTH, TILE_HEIGHT);
					infantry.rcAtk[1] = RectMake(infantry.rc.left - 48, infantry.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					infantry.rcAtk[2] = RectMake(infantry.rc.left - 48, infantry.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					infantry.rcAtk[3] = RectMake(infantry.rc.left + 48, infantry.rc.top, TILE_WIDTH, TILE_HEIGHT);
					infantry.rcAtk[4] = RectMake(infantry.rc.left + 48, infantry.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					infantry.rcAtk[5] = RectMake(infantry.rc.left + 48, infantry.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					infantry.rcAtk[6] = RectMake(infantry.rc.left, infantry.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					infantry.rcAtk[7] = RectMake(infantry.rc.left, infantry.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(infantry.rcAtk[j]);
				}

				//�̵�����
				if (isMove) floodFill(startTile, infantry.movingCount);
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

void Infantry::enemyMove()
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

	if (infantry.rc.left > 0 || infantry.rc.right < WINSIZEY ||
		infantry.rc.top > 0 || infantry.rc.bottom < WINSIZEY)
	{
		switch (eDirection)
		{
		case ENEMY_LEFT:
			enemyX -= speed;
			infantry.rc = RectMakeCenter(enemyX, enemyY, infantry.img->getFrameWidth(), infantry.img->getFrameHeight());
			break;
		case ENEMY_RIGHT:
			enemyX += speed;
			infantry.rc = RectMakeCenter(enemyX, enemyY, infantry.img->getFrameWidth(), infantry.img->getFrameHeight());
			break;
		case ENEMY_UP:
			enemyY -= speed;
			infantry.rc = RectMakeCenter(enemyX, enemyY, infantry.img->getFrameWidth(), infantry.img->getFrameHeight());
			break;
		case ENEMY_DOWN:
			enemyY += speed;
			infantry.rc = RectMakeCenter(enemyX, enemyY, infantry.img->getFrameWidth(), infantry.img->getFrameHeight());
			break;
		}

		if (enemyX == stackX && enemyY == stackY)
		{
			optimalPath.pop();
		}
	}
}

void Infantry::enemyAstar()
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
			for (int j = 0; j < 8; j++)
			{
				infantry.rcAtk[0] = RectMake(infantry.rc.left - 48, infantry.rc.top, TILE_WIDTH, TILE_HEIGHT);
				infantry.rcAtk[1] = RectMake(infantry.rc.left - 48, infantry.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				infantry.rcAtk[2] = RectMake(infantry.rc.left - 48, infantry.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				infantry.rcAtk[3] = RectMake(infantry.rc.left + 48, infantry.rc.top, TILE_WIDTH, TILE_HEIGHT);
				infantry.rcAtk[4] = RectMake(infantry.rc.left + 48, infantry.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				infantry.rcAtk[5] = RectMake(infantry.rc.left + 48, infantry.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				infantry.rcAtk[6] = RectMake(infantry.rc.left, infantry.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				infantry.rcAtk[7] = RectMake(infantry.rc.left, infantry.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				atkList.push_back(infantry.rcAtk[j]);
			}
		}
	}

	if (isClick)
	{
		//�޴����� ��Ʈ 
		for (int j = 0; j < 5; j++)
		{
			rcMenu[0] = RectMake(infantry.rc.left - 97, infantry.rc.top - 30, 82, 20);
			rcMenu[1] = RectMake(infantry.rc.left - 97, infantry.rc.top - 9, 82, 20);
			rcMenu[2] = RectMake(infantry.rc.left - 97, infantry.rc.top + 12, 82, 20);
			rcMenu[3] = RectMake(infantry.rc.left - 97, infantry.rc.top + 38, 82, 20);
			rcMenu[4] = RectMake(infantry.rc.left - 97, infantry.rc.top + 63, 82, 20);
			menuList.push_back(rcMenu[j]);
		}
	}
}

void Infantry::enemyAnimation()
{
	if (isTurn)
	{
		if (isAtk)
		{
			switch (eDirection)
			{
			case ENEMY_LEFT:
				ANIMATIONMANAGER->addAnimation("leftAtk", "������ATK", 12, 15, 5, false, false);
				enemyAni = ANIMATIONMANAGER->findAnimation("leftAtk");
				ANIMATIONMANAGER->resume("leftAtk");
				break;
			case ENEMY_RIGHT:
				ANIMATIONMANAGER->addAnimation("rightAtk", "������ATK", 8, 11, 5, false, false);
				enemyAni = ANIMATIONMANAGER->findAnimation("rightAtk");
				ANIMATIONMANAGER->resume("rightAtk");
				break;
			case ENEMY_UP:
				ANIMATIONMANAGER->addAnimation("upAtk", "������ATK", 4, 7, 5, false, false);
				enemyAni = ANIMATIONMANAGER->findAnimation("upAtk");
				ANIMATIONMANAGER->resume("upAtk");
				break;
			case ENEMY_DOWN:
				ANIMATIONMANAGER->addAnimation("downAtk", "������ATK", 0, 3, 5, false, false);
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
				ANIMATIONMANAGER->addAnimation("enemyLeft", "������", 4, 5, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyLeft");
				ANIMATIONMANAGER->resume("enemyLeft");
				break;
			case ENEMY_RIGHT:
				ANIMATIONMANAGER->addAnimation("enemyRight", "������", 6, 7, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyRight");
				ANIMATIONMANAGER->resume("enemyRight");
				break;
			case ENEMY_UP:
				ANIMATIONMANAGER->addAnimation("enemyUp", "������", 2, 3, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyUp");
				ANIMATIONMANAGER->resume("enemyUp");
				break;
			case ENEMY_DOWN:
				ANIMATIONMANAGER->addAnimation("enemyDown", "������", 0, 1, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyDown");
				ANIMATIONMANAGER->resume("enemyDown");
				break;
			}

			if (currentHp < 40 && !isSelect && isMove)
			{
				ANIMATIONMANAGER->addAnimation("enemyHp", "������", 12, 13, 2, false, true);
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

void Infantry::enemyState()
{
	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);

	if (currentHp < 0) currentHp = 0;
}

void Infantry::enemyMenu()
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

void Infantry::enemyCollision()
{
	RECT temp;
	frameX = 0;

	for (int j = 0; j < PLAYERMANAGER->getPlayer().size(); j++)
	{
		auto playerX = PLAYERMANAGER->getPlayer()[j]->getPlayerX();
		auto playerY = PLAYERMANAGER->getPlayer()[j]->getPlayerY();
		auto playerHit = PLAYERMANAGER->getPlayer()[j]->getIsHit();
		auto& playerRect = PLAYERMANAGER->getPlayer()[j]->getPlayerInfo().rc;

		for (int k = 0; k < 8; k++)
		{
			if (IntersectRect(&temp, &infantry.rcAtk[k], &playerRect))
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

		for (int k = 0; k < 8; k++)
		{
			if (IntersectRect(&temp, &infantry.rcAtk[k], &friendRect))
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

void Infantry::setPosition(RECT rc)
{
	infantry.rc = rc;
	enemyX = infantry.rc.left + (infantry.rc.right - infantry.rc.left) / 2;
	enemyY = infantry.rc.top + (infantry.rc.bottom - infantry.rc.top) / 2;
}
