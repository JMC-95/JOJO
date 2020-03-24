#include "stdafx.h"
#include "Agjin.h"

Agjin::Agjin()
{
}

Agjin::~Agjin()
{
}

HRESULT Agjin::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * playerImg, const char * atkImg, const char * blockImg)
{
	//�̹��� �� �ִϸ��̼�
	agjin.moveRngImg = IMAGEMANAGER->findImage(moveImg);	//ĳ���� Ŭ���� �̵����� �̹���
	agjin.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//ĳ���� Ŭ���� ���ݹ��� �̹���
	agjin.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//���ݹ�ư Ŭ���� ���ݹ��� �̹���
	agjin.img = IMAGEMANAGER->findImage(playerImg);			//ĳ���� �̹���
	agjin.atkImg = IMAGEMANAGER->findImage(atkImg);			//���� �̹���
	agjin.blockImg = IMAGEMANAGER->findImage(blockImg);		//��� �� �ǰ� �̹���
	//�������ͽ�
	agjin.level = 4;		//����
	agjin.hp = 142;			//ü��
	agjin.mp = 19;			//����
	agjin.atk = 47;			//���ݷ�
	agjin.will = 50;		//���ŷ�
	agjin.def = 62;			//����
	agjin.agi = 35;			//���߷�
	agjin.ten = 51;			//���
	agjin.movingCount = 4;	//�̵���

	//HP ProgressBar
	_Hp = new progressBar;
	_Hp->init("images/UI/Info/HP.bmp", "images/UI/Info/Back_P.bmp", 1056, 289, 84, 12);
	_Hp->setGauge(currentHp, maxHp);
	currentHp = maxHp = 142;

	//MP ProgressBar
	_Mp = new progressBar;
	_Mp->init("images/UI/Info/MP.bmp", "images/UI/Info/Back_P.bmp", 1056, 307, 84, 12);
	_Mp->setGauge(currentMp, maxMp);
	currentMp = maxMp = 19;

	//EXP ProgressBar
	_Exp = new progressBar;
	_Exp->init("images/UI/Info/EXP.bmp", "images/UI/Info/Back_EXP.bmp", 1095, 243, 45, 12);
	_Exp->setGauge(currentExp, maxExp);
	currentExp = 52;
	maxExp = 100;

	//ĳ���� ���� �� ��ġ
	pDirection = PLAYER_LEFT;
	startTile = endTile = -1;
	speed = 6;	//�ӵ�

	isTurn = true;
	isSelect = false;

	return S_OK;
}

void Agjin::release()
{
}

void Agjin::update()
{
	if (isTurn)
	{
		mouseMove();
	}

	playerAnimation();
	playerState();
}

void Agjin::render(HDC hdc)
{
	//for (int k = 0; k < vAgjin.size(); k++)
	//{
	if (isTurn)
	{
		if (isAtk)
		{
			agjin.atkImg->aniRender(hdc, agjin.rc.left - 8, agjin.rc.top - 8, playerAni);
		}
		else if (isHit)
		{
			agjin.blockImg->frameRender(hdc, agjin.rc.left, agjin.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, agjin.rc.left, agjin.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			agjin.img->aniRender(hdc, agjin.rc.left, agjin.rc.top, playerAni);
		}
	}
	else
	{
		if (isHit)
		{
			agjin.blockImg->frameRender(hdc, agjin.rc.left, agjin.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, agjin.rc.left, agjin.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			agjin.img->frameAlphaRender(hdc, agjin.rc.left, agjin.rc.top, 0, frameY, 100);
		}
	}
	//}
}

void Agjin::mouseMove()
{
	//for (int k = 0; k < vAgjin.size(); k++)
	//{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&agjin.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
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
					agjin.rcAtk[0] = RectMake(agjin.rc.left - 48, agjin.rc.top, TILE_WIDTH, TILE_HEIGHT);
					agjin.rcAtk[1] = RectMake(agjin.rc.left - 48, agjin.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					agjin.rcAtk[2] = RectMake(agjin.rc.left - 48, agjin.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					agjin.rcAtk[3] = RectMake(agjin.rc.left + 48, agjin.rc.top, TILE_WIDTH, TILE_HEIGHT);
					agjin.rcAtk[4] = RectMake(agjin.rc.left + 48, agjin.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					agjin.rcAtk[5] = RectMake(agjin.rc.left + 48, agjin.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					agjin.rcAtk[6] = RectMake(agjin.rc.left, agjin.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					agjin.rcAtk[7] = RectMake(agjin.rc.left, agjin.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(agjin.rcAtk[j]);
				}

				//�̵�����
				if (!isStop)
				{
					floodFill(startTile, agjin.movingCount);
				}
			}
		}

		if (!PtInRect(&agjin.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
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
	//}

	playerAstar();
	playerMenu();
	playerCollision();
}

void Agjin::playerMove()
{
	//for (int k = 0; k < vAgjin.size(); k++)
	//{
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

	if (agjin.rc.left > 0 || agjin.rc.right < WINSIZEY ||
		agjin.rc.top > 0 || agjin.rc.bottom < WINSIZEY)
	{
		switch (pDirection)
		{
		case PLAYER_LEFT:
			playerX -= speed;
			agjin.rc = RectMakeCenter(playerX, playerY, agjin.img->getFrameWidth(), agjin.img->getFrameHeight());
			break;
		case PLAYER_RIGHT:
			playerX += speed;
			agjin.rc = RectMakeCenter(playerX, playerY, agjin.img->getFrameWidth(), agjin.img->getFrameHeight());
			break;
		case PLAYER_UP:
			playerY -= speed;
			agjin.rc = RectMakeCenter(playerX, playerY, agjin.img->getFrameWidth(), agjin.img->getFrameHeight());
			break;
		case PLAYER_DOWN:
			playerY += speed;
			agjin.rc = RectMakeCenter(playerX, playerY, agjin.img->getFrameWidth(), agjin.img->getFrameHeight());
			break;
		}

		if (playerX == stackX && playerY == stackY)
		{
			isMove = false;
			optimalPath.pop();
		}
	}
	//}
}

void Agjin::playerAstar()
{
	//	for (int k = 0; k < vAgjin.size(); k++)
		//{
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
				agjin.rcAtk[0] = RectMake(agjin.rc.left - 48, agjin.rc.top, TILE_WIDTH, TILE_HEIGHT);
				agjin.rcAtk[1] = RectMake(agjin.rc.left - 48, agjin.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				agjin.rcAtk[2] = RectMake(agjin.rc.left - 48, agjin.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				agjin.rcAtk[3] = RectMake(agjin.rc.left + 48, agjin.rc.top, TILE_WIDTH, TILE_HEIGHT);
				agjin.rcAtk[4] = RectMake(agjin.rc.left + 48, agjin.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				agjin.rcAtk[5] = RectMake(agjin.rc.left + 48, agjin.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				agjin.rcAtk[6] = RectMake(agjin.rc.left, agjin.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				agjin.rcAtk[7] = RectMake(agjin.rc.left, agjin.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				atkList.push_back(agjin.rcAtk[j]);
			}

			//�޴����� ��Ʈ
			for (int j = 0; j < 5; j++)
			{
				rcMenu[0] = RectMake(agjin.rc.left - 97, agjin.rc.top - 30, 82, 20);
				rcMenu[1] = RectMake(agjin.rc.left - 97, agjin.rc.top - 9, 82, 20);
				rcMenu[2] = RectMake(agjin.rc.left - 97, agjin.rc.top + 12, 82, 20);
				rcMenu[3] = RectMake(agjin.rc.left - 97, agjin.rc.top + 38, 82, 20);
				rcMenu[4] = RectMake(agjin.rc.left - 97, agjin.rc.top + 63, 82, 20);
				menuList.push_back(rcMenu[j]);
			}
		}
	}
	//}
}

void Agjin::playerMenu()
{
	//for (int k = 0; k < vAgjin.size(); k++)
	//{
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

				agjin.rc.left = prevTile.rc.left;
				agjin.rc.right = prevTile.rc.right;
				agjin.rc.top = prevTile.rc.top;
				agjin.rc.bottom = prevTile.rc.bottom;
				pDirection = sDirection;

				playerX = prevTile.rc.left + TILE_WIDTH * 0.5;
				playerY = prevTile.rc.top + TILE_HEIGHT * 0.5;

				isSelect = false;
				isStop = false;
				isClick = false;
			}
		}
	}
	//}
}

void Agjin::playerCollision()
{
	//for (int k = 0; k < vAgjin.size(); k++)
	//{
	RECT temp;

	if (IntersectRect(&temp, &agjin.rcAtk[0], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
		IntersectRect(&temp, &agjin.rcAtk[1], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
		IntersectRect(&temp, &agjin.rcAtk[2], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
		IntersectRect(&temp, &agjin.rcAtk[3], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
		IntersectRect(&temp, &agjin.rcAtk[4], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
		IntersectRect(&temp, &agjin.rcAtk[5], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
		IntersectRect(&temp, &agjin.rcAtk[6], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
		IntersectRect(&temp, &agjin.rcAtk[7], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc))
	{
		isTarget = true;
		frameX = 1;

		if (PtInRect(&ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc, m_ptMouse) &&
			KEYMANAGER->isStayKeyDown(VK_LBUTTON) && isAtkRng)
		{
			isAtkRng = false;
			isAtk = true;

			if (playerX > ENEMYMANAGER->getYeopo()->getEnemyX())
				pDirection = PLAYER_LEFT;
			else if (playerX < ENEMYMANAGER->getYeopo()->getEnemyX())
				pDirection = PLAYER_RIGHT;
			else if (playerY > ENEMYMANAGER->getYeopo()->getEnemyY())
				pDirection = PLAYER_UP;
			else if (playerY < ENEMYMANAGER->getYeopo()->getEnemyY())
				pDirection = PLAYER_DOWN;
		}
	}
	else
	{
		frameX = 0;
	}
	//}
}

void Agjin::playerAnimation()
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

void Agjin::playerState()
{
	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);
}

void Agjin::setPosition(RECT rc)
{
	//for (int k = 0; k < vAgjin.size(); k++)
	//{
	agjin.rc = rc;
	playerX = agjin.rc.left + (agjin.rc.right - agjin.rc.left) / 2;
	playerY = agjin.rc.top + (agjin.rc.bottom - agjin.rc.top) / 2;
	//}
}
