#include "stdafx.h"
#include "Ijeon.h"

Ijeon::Ijeon()
{
}

Ijeon::~Ijeon()
{
}

HRESULT Ijeon::init(const char* skillImg, const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * playerImg, const char * atkImg, const char * blockImg)
{
	//�̸� �� ��
	ijeon.name = "����";
	ijeon.face = "����Face";
	ijeon.className = "�溸��";
	//�̹��� �� �ִϸ��̼�
	ijeon.skillImg = IMAGEMANAGER->findImage(skillImg);		//��ų �̹���
	ANIMATIONMANAGER->addDefAnimation("healImg", "heal", 30, false, false);
	skillAni = ANIMATIONMANAGER->findAnimation("healImg");
	ijeon.moveRngImg = IMAGEMANAGER->findImage(moveImg);	//ĳ���� Ŭ���� �̵����� �̹���
	ijeon.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//ĳ���� Ŭ���� ���ݹ��� �̹���
	ijeon.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//���ݹ�ư Ŭ���� ���ݹ��� �̹���
	ijeon.img = IMAGEMANAGER->findImage(playerImg);			//ĳ���� �̹���
	ijeon.atkImg = IMAGEMANAGER->findImage(atkImg);			//���� �̹���
	ijeon.blockImg = IMAGEMANAGER->findImage(blockImg);		//��� �� �ǰ� �̹���
	ANIMATIONMANAGER->addAnimation("playerLeft", "����", 4, 5, 2, false, true);
	playerAni = ANIMATIONMANAGER->findAnimation("playerLeft");
	//�������ͽ�
	ijeon.level = 4;		//����
	ijeon.hp = 138;			//ü��
	ijeon.mp = 21;			//����
	ijeon.atk = 49;			//���ݷ�
	ijeon.will = 42;		//���ŷ�
	ijeon.def = 61;			//����
	ijeon.agi = 45;			//���߷�
	ijeon.ten = 36;			//���
	ijeon.movingCount = 4;	//�̵���
	//������
	ijeon.weaponName = ITEMMANAGER->getWeapon()[0].name;
	ijeon.armorName = ITEMMANAGER->getArmor()[0].name;
	ijeon.weaponImg = ITEMMANAGER->getWeapon()[0].itemImage;
	ijeon.armorImg = ITEMMANAGER->getArmor()[0].itemImage;
	ijeon.addAtk = ITEMMANAGER->getWeapon()[0].power;
	ijeon.addDef = ITEMMANAGER->getArmor()[0].power;

	//HP ProgressBar
	_Hp = new progressBar;
	_Hp->init("images/UI/Info/HP.bmp", "images/UI/Info/Back_P.bmp", 1056, 289, 84, 12);
	_Hp->setGauge(currentHp, maxHp);
	currentHp = maxHp = 138;

	//MP ProgressBar
	_Mp = new progressBar;
	_Mp->init("images/UI/Info/MP.bmp", "images/UI/Info/Back_P.bmp", 1056, 307, 84, 12);
	_Mp->setGauge(currentMp, maxMp);
	currentMp = maxMp = 21;

	//EXP ProgressBar
	_Exp = new progressBar;
	_Exp->init("images/UI/Info/EXP.bmp", "images/UI/Info/Back_EXP.bmp", 1095, 243, 45, 12);
	_Exp->setGauge(currentExp, maxExp);
	currentExp = 0;
	maxExp = 100;

	//ĳ���� ���� �� ��ġ
	pDirection = PLAYER_LEFT;
	startTile = endTile = -1;
	speed = 12;	//�ӵ�

	isTurn = true;
	isMove = true;
	isSelect = false;

	return S_OK;
}

void Ijeon::release()
{
}

void Ijeon::update()
{
	if (isTurn)
	{
		if (PLAYERMANAGER->getPturn())
		{
			auto& playerVector = PLAYERMANAGER->getPlayer();

			bool isMouseMove = true;

			for (int i = 0; i < playerVector.size(); ++i)
			{
				if (playerVector[i]->getPlayerInfo().name == "����")
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

void Ijeon::render(HDC hdc)
{
	if (isTurn)
	{
		if (isAtk)
		{
			ijeon.atkImg->aniRender(hdc, ijeon.rc.left - 8, ijeon.rc.top - 8, playerAni);
		}
		else if (isHit)
		{
			ijeon.blockImg->frameRender(hdc, ijeon.rc.left, ijeon.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, ijeon.rc.left, ijeon.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			if (isHeal)
			{
				ijeon.blockImg->frameRender(hdc, ijeon.rc.left, ijeon.rc.top, 0, 5);
				ijeon.skillImg->aniRender(hdc, ijeon.rc.left - 8, ijeon.rc.top - 8, skillAni);

				if (maxHp - currentHp < 30)
				{
					sprintf_s(str, "%d", maxHp - currentHp);
					TextOut(hdc, ijeon.rc.left, ijeon.rc.top, str, strlen(str));
				}
				else
				{
					sprintf_s(str, "%d", 30);
					TextOut(hdc, ijeon.rc.left, ijeon.rc.top, str, strlen(str));
				}
			}
			else
			{
				ijeon.img->aniRender(hdc, ijeon.rc.left, ijeon.rc.top, playerAni);
			}
		}
	}
	else
	{
		if (isHit)
		{
			ijeon.blockImg->frameRender(hdc, ijeon.rc.left, ijeon.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, ijeon.rc.left, ijeon.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			if (isHeal)
			{
				ijeon.blockImg->frameRender(hdc, ijeon.rc.left, ijeon.rc.top, 0, 5);
				ijeon.skillImg->aniRender(hdc, ijeon.rc.left - 8, ijeon.rc.top - 8, skillAni);

				if (maxHp - currentHp < 30)
				{
					sprintf_s(str, "%d", maxHp - currentHp);
					TextOut(hdc, ijeon.rc.left, ijeon.rc.top, str, strlen(str));
				}
				else
				{
					sprintf_s(str, "%d", 30);
					TextOut(hdc, ijeon.rc.left, ijeon.rc.top, str, strlen(str));
				}
			}
			else
			{
				ijeon.img->frameAlphaRender(hdc, ijeon.rc.left, ijeon.rc.top, 0, frameY, 100);
			}
		}
	}
}

void Ijeon::mouseMove()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&ijeon.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				SOUNDMANAGER->play("click", 1.0f);

				//������ Ÿ�� (ĳ����)
				startTile = i;
				saveTile = startTile;
				sDirection = pDirection;

				isSelect = true;
				isFind = false;
				noPath = false;
				startAstar = false;

				//���ݹ���
				for (int j = 0; j < 8; j++)
				{
					ijeon.rcAtk[0] = RectMake(ijeon.rc.left - 48, ijeon.rc.top, TILE_WIDTH, TILE_HEIGHT);
					ijeon.rcAtk[1] = RectMake(ijeon.rc.left - 48, ijeon.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					ijeon.rcAtk[2] = RectMake(ijeon.rc.left - 48, ijeon.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					ijeon.rcAtk[3] = RectMake(ijeon.rc.left + 48, ijeon.rc.top, TILE_WIDTH, TILE_HEIGHT);
					ijeon.rcAtk[4] = RectMake(ijeon.rc.left + 48, ijeon.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					ijeon.rcAtk[5] = RectMake(ijeon.rc.left + 48, ijeon.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					ijeon.rcAtk[6] = RectMake(ijeon.rc.left, ijeon.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					ijeon.rcAtk[7] = RectMake(ijeon.rc.left, ijeon.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(ijeon.rcAtk[j]);
				}

				//�̵�����
				if (isMove)
				{
					floodFill(startTile, ijeon.movingCount);

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

		if (!PtInRect(&ijeon.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
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

void Ijeon::playerMove()
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

	if (ijeon.rc.left > 0 || ijeon.rc.right < WINSIZEY ||
		ijeon.rc.top > 0 || ijeon.rc.bottom < WINSIZEY)
	{
		switch (pDirection)
		{
		case PLAYER_LEFT:
			playerX -= speed;
			ijeon.rc = RectMakeCenter(playerX, playerY, ijeon.img->getFrameWidth(), ijeon.img->getFrameHeight());
			break;
		case PLAYER_RIGHT:
			playerX += speed;
			ijeon.rc = RectMakeCenter(playerX, playerY, ijeon.img->getFrameWidth(), ijeon.img->getFrameHeight());
			break;
		case PLAYER_UP:
			playerY -= speed;
			ijeon.rc = RectMakeCenter(playerX, playerY, ijeon.img->getFrameWidth(), ijeon.img->getFrameHeight());
			break;
		case PLAYER_DOWN:
			playerY += speed;
			ijeon.rc = RectMakeCenter(playerX, playerY, ijeon.img->getFrameWidth(), ijeon.img->getFrameHeight());
			break;
		}

		if (playerX == stackX && playerY == stackY)
		{
			optimalPath.pop();
		}
	}
}

void Ijeon::playerAstar()
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
			playerMove();
		}

		if (playerX == mapX && playerY == mapY)
		{
			atkList.clear();

			isMove = false;
			isClick = true;

			//���ݹ���
			for (int j = 0; j < 8; j++)
			{
				ijeon.rcAtk[0] = RectMake(ijeon.rc.left - 48, ijeon.rc.top, TILE_WIDTH, TILE_HEIGHT);
				ijeon.rcAtk[1] = RectMake(ijeon.rc.left - 48, ijeon.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				ijeon.rcAtk[2] = RectMake(ijeon.rc.left - 48, ijeon.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				ijeon.rcAtk[3] = RectMake(ijeon.rc.left + 48, ijeon.rc.top, TILE_WIDTH, TILE_HEIGHT);
				ijeon.rcAtk[4] = RectMake(ijeon.rc.left + 48, ijeon.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				ijeon.rcAtk[5] = RectMake(ijeon.rc.left + 48, ijeon.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				ijeon.rcAtk[6] = RectMake(ijeon.rc.left, ijeon.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				ijeon.rcAtk[7] = RectMake(ijeon.rc.left, ijeon.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				atkList.push_back(ijeon.rcAtk[j]);
			}
		}
	}

	if (isClick)
	{
		//�޴����� ��Ʈ
		for (int j = 0; j < 5; j++)
		{
			rcMenu[0] = RectMake(ijeon.rc.left - 97, ijeon.rc.top - 30, 82, 20);
			rcMenu[1] = RectMake(ijeon.rc.left - 97, ijeon.rc.top - 9, 82, 20);
			rcMenu[2] = RectMake(ijeon.rc.left - 97, ijeon.rc.top + 12, 82, 20);
			rcMenu[3] = RectMake(ijeon.rc.left - 97, ijeon.rc.top + 38, 82, 20);
			rcMenu[4] = RectMake(ijeon.rc.left - 97, ijeon.rc.top + 63, 82, 20);
			menuList.push_back(rcMenu[j]);
		}
	}
}

void Ijeon::playerMenu()
{
	//�޴�
	if (isClick)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("click", 1.0f);

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

				auto& prevTile = mainMap->getMap()[saveTile];

				ijeon.rc.left = prevTile.rc.left;
				ijeon.rc.right = prevTile.rc.right;
				ijeon.rc.top = prevTile.rc.top;
				ijeon.rc.bottom = prevTile.rc.bottom;
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

void Ijeon::playerCollision()
{
	isTarget = false;
	frameX = 0;

	for (int j = 0; j < ENEMYMANAGER->getEnemy().size(); j++)
	{
		auto enemyX = ENEMYMANAGER->getEnemy()[j]->getEnemyX();
		auto enemyY = ENEMYMANAGER->getEnemy()[j]->getEnemyY();
		auto enemyHit = ENEMYMANAGER->getEnemy()[j]->getIsHit();
		auto& enemyRect = ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rc;

		for (int k = 0; k < 8; k++)
		{
			if (IntersectRect(&temp, &ijeon.rcAtk[k], &enemyRect))
			{
				isTarget = true;
				frameX = 1;

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
}

void Ijeon::playerAnimation()
{
	if (isTurn)
	{
		if (isAtk)
		{
			switch (pDirection)
			{
			case PLAYER_LEFT:
				ANIMATIONMANAGER->addAnimation("leftAtk", "����ATK", 12, 15, 5, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("leftAtk");
				ANIMATIONMANAGER->resume("leftAtk");
				break;
			case PLAYER_RIGHT:
				ANIMATIONMANAGER->addAnimation("rightAtk", "����ATK", 8, 11, 5, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("rightAtk");
				ANIMATIONMANAGER->resume("rightAtk");
				break;
			case PLAYER_UP:
				ANIMATIONMANAGER->addAnimation("upAtk", "����ATK", 4, 7, 5, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("upAtk");
				ANIMATIONMANAGER->resume("upAtk");
				break;
			case PLAYER_DOWN:
				ANIMATIONMANAGER->addAnimation("downAtk", "����ATK", 0, 3, 5, false, false);
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
				ANIMATIONMANAGER->addAnimation("playerLeft", "����", 4, 5, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerLeft");
				ANIMATIONMANAGER->resume("playerLeft");
				break;
			case PLAYER_RIGHT:
				ANIMATIONMANAGER->addAnimation("playerRight", "����", 6, 7, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerRight");
				ANIMATIONMANAGER->resume("playerRight");
				break;
			case PLAYER_UP:
				ANIMATIONMANAGER->addAnimation("playerUp", "����", 2, 3, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerUp");
				ANIMATIONMANAGER->resume("playerUp");
				break;
			case PLAYER_DOWN:
				ANIMATIONMANAGER->addAnimation("playerDown", "����", 0, 1, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerDown");
				ANIMATIONMANAGER->resume("playerDown");
				break;
			}

			if (currentHp < 40 && !isSelect && isMove)
			{
				ANIMATIONMANAGER->addAnimation("playerHp", "����", 12, 13, 2, false, true);
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

void Ijeon::playerState()
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

		ijeon.level += 1;		//����
		maxHp += 3;				//ü��
		maxMp += 3;				//����
		ijeon.atk += 3;			//���ݷ�
		ijeon.will += 3;		//���ŷ�
		ijeon.def += 3;			//����
		ijeon.agi += 3;			//���߷�

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

void Ijeon::setPosition(RECT rc)
{
	ijeon.rc = rc;
	playerX = ijeon.rc.left + (ijeon.rc.right - ijeon.rc.left) / 2;
	playerY = ijeon.rc.top + (ijeon.rc.bottom - ijeon.rc.top) / 2;
}
