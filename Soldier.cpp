#include "stdafx.h"
#include "Soldier.h"

Soldier::Soldier(int num)
{
	number = num;
}

Soldier::~Soldier()
{
}

HRESULT Soldier::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * friendImg, const char * atkImg, const char * blockImg)
{
	//�̸� �� ��
	soldier.name = "����";
	soldier.face = "�챺����Face";
	soldier.className = "�溸��";
	//�̹��� �� �ִϸ��̼�
	soldier.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//ĳ���� Ŭ���� �̵����� �̹���
	soldier.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//ĳ���� Ŭ���� ���ݹ��� �̹���
	soldier.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//���ݹ�ư Ŭ���� ���ݹ��� �̹���
	soldier.img = IMAGEMANAGER->findImage(friendImg);			//ĳ���� �̹���
	soldier.atkImg = IMAGEMANAGER->findImage(atkImg);			//���� �̹���
	soldier.blockImg = IMAGEMANAGER->findImage(blockImg);		//��� �� �ǰ� �̹���
	ANIMATIONMANAGER->addAnimation("friendLeft", "�챺����", 4, 5, 2, false, true);
	friendAni = ANIMATIONMANAGER->findAnimation("friendLeft");
	//�������ͽ�
	soldier.level = 2;			//����
	soldier.hp = 116;			//ü��
	soldier.mp = 11;			//����
	soldier.atk = 36;			//���ݷ�
	soldier.will = 38;			//���ŷ�
	soldier.def = 43;			//����
	soldier.agi = 32;			//���߷�
	soldier.ten = 32;			//���
	soldier.movingCount = 4;	//�̵���
	//������
	soldier.weaponName = ITEMMANAGER->getWeapon()[0].name;
	soldier.armorName = ITEMMANAGER->getArmor()[0].name;
	soldier.weaponImg = ITEMMANAGER->getWeapon()[0].itemImage;
	soldier.armorImg = ITEMMANAGER->getArmor()[0].itemImage;
	soldier.addAtk = ITEMMANAGER->getWeapon()[0].power;
	soldier.addDef = ITEMMANAGER->getArmor()[0].power;

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
	fDirection = FRIEND_LEFT;
	startTile = endTile = -1;
	speed = 12;	//�ӵ�

	isTurn = true;
	isMove = true;
	isSelect = false;

	return S_OK;
}

void Soldier::release()
{
	SAFE_DELETE(_Hp);
	SAFE_DELETE(_Mp);
	SAFE_DELETE(_Exp);
}

void Soldier::update()
{
	if (isTurn)
	{
		if (FRIENDMANAGER->getFturn()) friendAi();
	}

	friendAnimation();
	friendState();
	mouseMove();
}

void Soldier::render(HDC hdc)
{
	if (isTurn)
	{
		if (isAtk)
		{
			soldier.atkImg->aniRender(hdc, soldier.rc.left - 8, soldier.rc.top - 8, friendAni);
		}
		else if (isHit)
		{
			soldier.blockImg->frameRender(hdc, soldier.rc.left, soldier.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, soldier.rc.left, soldier.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			soldier.img->aniRender(hdc, soldier.rc.left, soldier.rc.top, friendAni);
		}
	}
	else
	{
		if (isHit)
		{
			soldier.blockImg->frameRender(hdc, soldier.rc.left, soldier.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, soldier.rc.left, soldier.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			soldier.img->frameAlphaRender(hdc, soldier.rc.left, soldier.rc.top, 0, frameY, 100);
		}
	}
}

void Soldier::friendAi()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&soldier.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
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
					soldier.rcAtk[0] = RectMake(soldier.rc.left - 48, soldier.rc.top, TILE_WIDTH, TILE_HEIGHT);
					soldier.rcAtk[1] = RectMake(soldier.rc.left - 48, soldier.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					soldier.rcAtk[2] = RectMake(soldier.rc.left - 48, soldier.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					soldier.rcAtk[3] = RectMake(soldier.rc.left + 48, soldier.rc.top, TILE_WIDTH, TILE_HEIGHT);
					soldier.rcAtk[4] = RectMake(soldier.rc.left + 48, soldier.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					soldier.rcAtk[5] = RectMake(soldier.rc.left + 48, soldier.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					soldier.rcAtk[6] = RectMake(soldier.rc.left, soldier.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					soldier.rcAtk[7] = RectMake(soldier.rc.left, soldier.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(soldier.rcAtk[j]);
				}

				//�̵�����
				if (isMove)
				{
					floodFill(startTile, soldier.movingCount);

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

		if (!PtInRect(&soldier.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
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

	friendAstar();
	friendCollision();
	friendMenu();
}

void Soldier::friendMove()
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

	if (soldier.rc.left > 0 || soldier.rc.right < WINSIZEY ||
		soldier.rc.top > 0 || soldier.rc.bottom < WINSIZEY)
	{
		switch (fDirection)
		{
		case FRIEND_LEFT:
			friendX -= speed;
			soldier.rc = RectMakeCenter(friendX, friendY, soldier.img->getFrameWidth(), soldier.img->getFrameHeight());
			break;
		case FRIEND_RIGHT:
			friendX += speed;
			soldier.rc = RectMakeCenter(friendX, friendY, soldier.img->getFrameWidth(), soldier.img->getFrameHeight());
			break;
		case FRIEND_UP:
			friendY -= speed;
			soldier.rc = RectMakeCenter(friendX, friendY, soldier.img->getFrameWidth(), soldier.img->getFrameHeight());
			break;
		case FRIEND_DOWN:
			friendY += speed;
			soldier.rc = RectMakeCenter(friendX, friendY, soldier.img->getFrameWidth(), soldier.img->getFrameHeight());
			break;
		}

		if (friendX == stackX && friendY == stackY)
		{
			optimalPath.pop();
		}
	}
}

void Soldier::friendAstar()
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
			for (int j = 0; j < 8; j++)
			{
				soldier.rcAtk[0] = RectMake(soldier.rc.left - 48, soldier.rc.top, TILE_WIDTH, TILE_HEIGHT);
				soldier.rcAtk[1] = RectMake(soldier.rc.left - 48, soldier.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				soldier.rcAtk[2] = RectMake(soldier.rc.left - 48, soldier.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				soldier.rcAtk[3] = RectMake(soldier.rc.left + 48, soldier.rc.top, TILE_WIDTH, TILE_HEIGHT);
				soldier.rcAtk[4] = RectMake(soldier.rc.left + 48, soldier.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				soldier.rcAtk[5] = RectMake(soldier.rc.left + 48, soldier.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				soldier.rcAtk[6] = RectMake(soldier.rc.left, soldier.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				soldier.rcAtk[7] = RectMake(soldier.rc.left, soldier.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				atkList.push_back(soldier.rcAtk[j]);
			}
		}
	}

	if (isClick)
	{
		//�޴����� ��Ʈ
		for (int j = 0; j < 5; j++)
		{
			rcMenu[0] = RectMake(soldier.rc.left - 97, soldier.rc.top - 30, 82, 20);
			rcMenu[1] = RectMake(soldier.rc.left - 97, soldier.rc.top - 9, 82, 20);
			rcMenu[2] = RectMake(soldier.rc.left - 97, soldier.rc.top + 12, 82, 20);
			rcMenu[3] = RectMake(soldier.rc.left - 97, soldier.rc.top + 38, 82, 20);
			rcMenu[4] = RectMake(soldier.rc.left - 97, soldier.rc.top + 63, 82, 20);
			menuList.push_back(rcMenu[j]);
		}
	}
}

void Soldier::friendMenu()
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

void Soldier::friendCollision()
{
	frameX = 0;

	for (int j = 0; j < ENEMYMANAGER->getEnemy().size(); j++)
	{
		auto enemyX = ENEMYMANAGER->getEnemy()[j]->getEnemyX();
		auto enemyY = ENEMYMANAGER->getEnemy()[j]->getEnemyY();
		auto enemyHit = ENEMYMANAGER->getEnemy()[j]->getIsHit();
		auto& enemyRect = ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rc;

		for (int k = 0; k < 8; k++)
		{
			if (IntersectRect(&temp, &soldier.rcAtk[k], &enemyRect))
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

void Soldier::friendAnimation()
{
	if (isTurn)
	{
		if (isAtk)
		{
			switch (fDirection)
			{
			case FRIEND_LEFT:
				ANIMATIONMANAGER->addAnimation("leftAtk", "�챺����ATK", 12, 15, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("leftAtk");
				ANIMATIONMANAGER->resume("leftAtk");
				break;
			case FRIEND_RIGHT:
				ANIMATIONMANAGER->addAnimation("rightAtk", "�챺����ATK", 8, 11, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("rightAtk");
				ANIMATIONMANAGER->resume("rightAtk");
				break;
			case FRIEND_UP:
				ANIMATIONMANAGER->addAnimation("upAtk", "�챺����ATK", 4, 7, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("upAtk");
				ANIMATIONMANAGER->resume("upAtk");
				break;
			case FRIEND_DOWN:
				ANIMATIONMANAGER->addAnimation("downAtk", "�챺����ATK", 0, 3, 5, false, false);
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
				ANIMATIONMANAGER->addAnimation("friendLeft", "�챺����", 4, 5, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendLeft");
				ANIMATIONMANAGER->resume("friendLeft");
				break;
			case FRIEND_RIGHT:
				ANIMATIONMANAGER->addAnimation("friendRight", "�챺����", 6, 7, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendRight");
				ANIMATIONMANAGER->resume("friendRight");
				break;
			case FRIEND_UP:
				ANIMATIONMANAGER->addAnimation("friendUp", "�챺����", 2, 3, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendUp");
				ANIMATIONMANAGER->resume("friendUp");
				break;
			case FRIEND_DOWN:
				ANIMATIONMANAGER->addAnimation("friendDown", "�챺����", 0, 1, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendDown");
				ANIMATIONMANAGER->resume("friendDown");
				break;
			}

			if (currentHp < 40 && !isSelect && isMove)
			{
				ANIMATIONMANAGER->addAnimation("friendHp", "�챺����", 12, 13, 2, false, true);
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

void Soldier::friendState()
{
	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);

	if (currentHp < 0) currentHp = 0;
}

void Soldier::mouseMove()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && PtInRect(&soldier.rc, m_ptMouse))
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
					soldier.rcAtk[0] = RectMake(soldier.rc.left - 48, soldier.rc.top, TILE_WIDTH, TILE_HEIGHT);
					soldier.rcAtk[1] = RectMake(soldier.rc.left - 48, soldier.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					soldier.rcAtk[2] = RectMake(soldier.rc.left - 48, soldier.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					soldier.rcAtk[3] = RectMake(soldier.rc.left + 48, soldier.rc.top, TILE_WIDTH, TILE_HEIGHT);
					soldier.rcAtk[4] = RectMake(soldier.rc.left + 48, soldier.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					soldier.rcAtk[5] = RectMake(soldier.rc.left + 48, soldier.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					soldier.rcAtk[6] = RectMake(soldier.rc.left, soldier.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					soldier.rcAtk[7] = RectMake(soldier.rc.left, soldier.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(soldier.rcAtk[j]);
				}

				//�̵�����
				if (isMove) floodFill(startTile, soldier.movingCount);
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

void Soldier::setPosition(RECT rc)
{
	soldier.rc = rc;
	friendX = soldier.rc.left + (soldier.rc.right - soldier.rc.left) / 2;
	friendY = soldier.rc.top + (soldier.rc.bottom - soldier.rc.top) / 2;
}
