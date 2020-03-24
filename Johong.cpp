#include "stdafx.h"
#include "Johong.h"

Johong::Johong()
{
}

Johong::~Johong()
{
}

HRESULT Johong::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * playerImg, const char * atkImg, const char * blockImg)
{
	//�̹��� �� �ִϸ��̼�
	johong.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//ĳ���� Ŭ���� �̵����� �̹���
	johong.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//ĳ���� Ŭ���� ���ݹ��� �̹���
	johong.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//���ݹ�ư Ŭ���� ���ݹ��� �̹���
	johong.img = IMAGEMANAGER->findImage(playerImg);			//ĳ���� �̹���
	johong.atkImg = IMAGEMANAGER->findImage(atkImg);			//���� �̹���
	johong.blockImg = IMAGEMANAGER->findImage(blockImg);		//��� �� �ǰ� �̹���
	//�������ͽ�
	johong.level = 4;			//����
	johong.hp = 134;			//ü��
	johong.mp = 17;				//����
	johong.atk = 40;			//���ݷ�
	johong.will = 53;			//���ŷ�
	johong.def = 51;			//����
	johong.agi = 41;			//���߷�
	johong.ten = 43;			//���
	johong.movingCount = 4;		//�̵���

	//HP ProgressBar
	_Hp = new progressBar;
	_Hp->init("images/UI/Info/HP.bmp", "images/UI/Info/Back_P.bmp", 1056, 289, 84, 12);
	_Hp->setGauge(currentHp, maxHp);
	currentHp = maxHp = 134;

	//MP ProgressBar
	_Mp = new progressBar;
	_Mp->init("images/UI/Info/MP.bmp", "images/UI/Info/Back_P.bmp", 1056, 307, 84, 12);
	_Mp->setGauge(currentMp, maxMp);
	currentMp = maxMp = 17;

	//EXP ProgressBar
	_Exp = new progressBar;
	_Exp->init("images/UI/Info/EXP.bmp", "images/UI/Info/Back_EXP.bmp", 1095, 243, 45, 12);
	_Exp->setGauge(currentExp, maxExp);
	currentExp = 47;
	maxExp = 100;

	//ĳ���� ���� �� ��ġ
	pDirection = PLAYER_LEFT;
	startTile = endTile = -1;
	speed = 6;	//�ӵ�

	isTurn = true;
	isSelect = false;

	return S_OK;
}

void Johong::release()
{
}

void Johong::update()
{
	if (isTurn)
	{
		mouseMove();
	}

	playerAnimation();
	playerState();
}

void Johong::render(HDC hdc)
{
	if (isTurn)
	{
		if (isAtk)
		{
			johong.atkImg->aniRender(hdc, johong.rc.left - 8, johong.rc.top - 8, playerAni);
		}
		else if (isHit)
		{
			johong.blockImg->frameRender(hdc, johong.rc.left, johong.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, johong.rc.left, johong.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			johong.img->aniRender(hdc, johong.rc.left, johong.rc.top, playerAni);
		}
	}
	else
	{
		if (isHit)
		{
			johong.blockImg->frameRender(hdc, johong.rc.left, johong.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, johong.rc.left, johong.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			johong.img->frameAlphaRender(hdc, johong.rc.left, johong.rc.top, 0, frameY, 100);
		}
	}
}

void Johong::mouseMove()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&johong.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
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
					johong.rcAtk[0] = RectMake(johong.rc.left - 48, johong.rc.top, TILE_WIDTH, TILE_HEIGHT);
					johong.rcAtk[1] = RectMake(johong.rc.left - 48, johong.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					johong.rcAtk[2] = RectMake(johong.rc.left - 48, johong.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					johong.rcAtk[3] = RectMake(johong.rc.left + 48, johong.rc.top, TILE_WIDTH, TILE_HEIGHT);
					johong.rcAtk[4] = RectMake(johong.rc.left + 48, johong.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					johong.rcAtk[5] = RectMake(johong.rc.left + 48, johong.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					johong.rcAtk[6] = RectMake(johong.rc.left, johong.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					johong.rcAtk[7] = RectMake(johong.rc.left, johong.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(johong.rcAtk[j]);
				}

				//�̵�����
				if (!isStop)
				{
					floodFill(startTile, johong.movingCount);
				}
			}
		}

		if (!PtInRect(&johong.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				if (mainMap->getMap()[i].flood)
				{
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

	playerAstar();
	playerMenu();
	playerCollision();
}

void Johong::playerMove()
{
	stackX = optimalPath.top().rc.left + (optimalPath.top().rc.right - optimalPath.top().rc.left) / 2;
	stackY = optimalPath.top().rc.top + (optimalPath.top().rc.bottom - optimalPath.top().rc.top) / 2;

	if (!isMove)
	{
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

		isMove = true;
	}

	if (johong.rc.left > 0 || johong.rc.right < WINSIZEY ||
		johong.rc.top > 0 || johong.rc.bottom < WINSIZEY)
	{
		switch (pDirection)
		{
		case PLAYER_LEFT:
			playerX -= speed;
			johong.rc = RectMakeCenter(playerX, playerY, johong.img->getFrameWidth(), johong.img->getFrameHeight());
			break;
		case PLAYER_RIGHT:
			playerX += speed;
			johong.rc = RectMakeCenter(playerX, playerY, johong.img->getFrameWidth(), johong.img->getFrameHeight());
			break;
		case PLAYER_UP:
			playerY -= speed;
			johong.rc = RectMakeCenter(playerX, playerY, johong.img->getFrameWidth(), johong.img->getFrameHeight());
			break;
		case PLAYER_DOWN:
			playerY += speed;
			johong.rc = RectMakeCenter(playerX, playerY, johong.img->getFrameWidth(), johong.img->getFrameHeight());
			break;
		}

		if (playerX == stackX && playerY == stackY)
		{
			isMove = false;
			optimalPath.pop();
		}
	}
}

void Johong::playerAstar()
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
		if (!isStop)
		{
			playerMove();
		}

		if (playerX == mapX && playerY == mapY)
		{
			isStop = true;
			isClick = true;

			//���ݹ���
			for (int j = 0; j < 8; j++)
			{
				johong.rcAtk[0] = RectMake(johong.rc.left - 48, johong.rc.top, TILE_WIDTH, TILE_HEIGHT);
				johong.rcAtk[1] = RectMake(johong.rc.left - 48, johong.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				johong.rcAtk[2] = RectMake(johong.rc.left - 48, johong.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				johong.rcAtk[3] = RectMake(johong.rc.left + 48, johong.rc.top, TILE_WIDTH, TILE_HEIGHT);
				johong.rcAtk[4] = RectMake(johong.rc.left + 48, johong.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				johong.rcAtk[5] = RectMake(johong.rc.left + 48, johong.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				johong.rcAtk[6] = RectMake(johong.rc.left, johong.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				johong.rcAtk[7] = RectMake(johong.rc.left, johong.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				atkList.push_back(johong.rcAtk[j]);
			}

			//�޴����� ��Ʈ
			for (int j = 0; j < 5; j++)
			{
				rcMenu[0] = RectMake(johong.rc.left - 97, johong.rc.top - 30, 82, 20);
				rcMenu[1] = RectMake(johong.rc.left - 97, johong.rc.top - 9, 82, 20);
				rcMenu[2] = RectMake(johong.rc.left - 97, johong.rc.top + 12, 82, 20);
				rcMenu[3] = RectMake(johong.rc.left - 97, johong.rc.top + 38, 82, 20);
				rcMenu[4] = RectMake(johong.rc.left - 97, johong.rc.top + 63, 82, 20);
				menuList.push_back(rcMenu[j]);
			}
		}
	}
}

void Johong::playerMenu()
{
	//�޴�
	if (isClick)
	{
		if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
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

				//isClick = false;
			}
			if (PtInRect(&rcMenu[2], m_ptMouse))	//����
			{
				atkList.clear();
				menuList.clear();

				//isClick = false;
			}
			if (PtInRect(&rcMenu[3], m_ptMouse))	//���
			{
				atkList.clear();
				menuList.clear();

				isTurn = false;
				isSelect = false;
				isStop = false;
				isClick = false;
			}
			if (PtInRect(&rcMenu[4], m_ptMouse))	//���
			{
				atkList.clear();
				menuList.clear();

				auto& prevTile = mainMap->getMap()[saveTile];

				johong.rc.left = prevTile.rc.left;
				johong.rc.right = prevTile.rc.right;
				johong.rc.top = prevTile.rc.top;
				johong.rc.bottom = prevTile.rc.bottom;
				pDirection = sDirection;

				playerX = prevTile.rc.left + TILE_WIDTH * 0.5;
				playerY = prevTile.rc.top + TILE_HEIGHT * 0.5;

				isSelect = false;
				isStop = false;
				isClick = false;
			}
		}
	}
}

void Johong::playerCollision()
{
	RECT temp;

	if (IntersectRect(&temp, &johong.rcAtk[0], &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rc) ||
		IntersectRect(&temp, &johong.rcAtk[1], &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rc) ||
		IntersectRect(&temp, &johong.rcAtk[2], &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rc) ||
		IntersectRect(&temp, &johong.rcAtk[3], &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rc) ||
		IntersectRect(&temp, &johong.rcAtk[4], &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rc) ||
		IntersectRect(&temp, &johong.rcAtk[5], &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rc) ||
		IntersectRect(&temp, &johong.rcAtk[6], &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rc) ||
		IntersectRect(&temp, &johong.rcAtk[7], &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rc))
	{
		isTarget = true;
		frameX = 1;

		if (PtInRect(&ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rc, m_ptMouse) &&
			KEYMANAGER->isStayKeyDown(VK_LBUTTON) && isAtkRng)
		{
			isAtkRng = false;
			isAtk = true;

			if (playerX > ENEMYMANAGER->getEnemy()[4]->getEnemyX())
				pDirection = PLAYER_LEFT;
			else if (playerX < ENEMYMANAGER->getEnemy()[4]->getEnemyX())
				pDirection = PLAYER_RIGHT;
			else if (playerY > ENEMYMANAGER->getEnemy()[4]->getEnemyY())
				pDirection = PLAYER_UP;
			else if (playerY < ENEMYMANAGER->getEnemy()[4]->getEnemyY())
				pDirection = PLAYER_DOWN;
		}
	}
	else
	{
		frameX = 0;
	}
}

void Johong::playerAnimation()
{
	if (isTurn)
	{
		if (isAtk)
		{
			switch (pDirection)
			{
			case PLAYER_LEFT:
				ANIMATIONMANAGER->addAnimation("leftAtk", "��ȫATK", 12, 15, 5, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("leftAtk");
				ANIMATIONMANAGER->resume("leftAtk");
				break;
			case PLAYER_RIGHT:
				ANIMATIONMANAGER->addAnimation("rightAtk", "��ȫATK", 8, 11, 5, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("rightAtk");
				ANIMATIONMANAGER->resume("rightAtk");
				break;
			case PLAYER_UP:
				ANIMATIONMANAGER->addAnimation("upAtk", "��ȫATK", 4, 7, 5, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("upAtk");
				ANIMATIONMANAGER->resume("upAtk");
				break;
			case PLAYER_DOWN:
				ANIMATIONMANAGER->addAnimation("downAtk", "��ȫATK", 0, 3, 5, false, false);
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
				ANIMATIONMANAGER->addAnimation("playerLeft", "��ȫ", 4, 5, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerLeft");
				ANIMATIONMANAGER->resume("playerLeft");
				break;
			case PLAYER_RIGHT:
				ANIMATIONMANAGER->addAnimation("playerRight", "��ȫ", 6, 7, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerRight");
				ANIMATIONMANAGER->resume("playerRight");
				break;
			case PLAYER_UP:
				ANIMATIONMANAGER->addAnimation("playerUp", "��ȫ", 2, 3, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerUp");
				ANIMATIONMANAGER->resume("playerUp");
				break;
			case PLAYER_DOWN:
				ANIMATIONMANAGER->addAnimation("playerDown", "��ȫ", 0, 1, 2, false, true);
				playerAni = ANIMATIONMANAGER->findAnimation("playerDown");
				ANIMATIONMANAGER->resume("playerDown");
				break;
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

void Johong::playerState()
{
	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);
}

void Johong::setPosition(RECT rc)
{
	johong.rc = rc;
	playerX = johong.rc.left + (johong.rc.right - johong.rc.left) / 2;
	playerY = johong.rc.top + (johong.rc.bottom - johong.rc.top) / 2;
}
