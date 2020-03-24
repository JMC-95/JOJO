#include "stdafx.h"
#include "Ijeon.h"

Ijeon::Ijeon()
{
}

Ijeon::~Ijeon()
{
}

HRESULT Ijeon::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * playerImg, const char * atkImg, const char * blockImg)
{
	//����ü ���� ����
	PlayerInfo ijeon;
	//�̹��� �� �ִϸ��̼�
	ijeon.moveRngImg = IMAGEMANAGER->findImage(moveImg);	//ĳ���� Ŭ���� �̵����� �̹���
	ijeon.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//ĳ���� Ŭ���� ���ݹ��� �̹���
	ijeon.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//���ݹ�ư Ŭ���� ���ݹ��� �̹���
	ijeon.img = IMAGEMANAGER->findImage(playerImg);			//ĳ���� �̹���
	ijeon.atkImg = IMAGEMANAGER->findImage(atkImg);			//���� �̹���
	ijeon.blockImg = IMAGEMANAGER->findImage(blockImg);		//��� �� �ǰ� �̹���
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
	vIjeon.push_back(ijeon);

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
	currentExp = 39;
	maxExp = 100;

	//ĳ���� ���� �� ��ġ
	pDirection = PLAYER_LEFT;
	startTile = endTile = -1;
	speed = 6;	//�ӵ�

	isTurn = true;
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
		mouseMove();
	}

	playerAnimation();
	playerState();
}

void Ijeon::render(HDC hdc)
{
	for (int k = 0; k < vIjeon.size(); k++)
	{
		if (isTurn)
		{
			if (isAtk)
			{
				vIjeon[k].atkImg->aniRender(hdc, vIjeon[k].rc.left - 8, vIjeon[k].rc.top - 8, playerAni);
			}
			else if (isHit)
			{
				vIjeon[k].blockImg->frameRender(hdc, vIjeon[k].rc.left, vIjeon[k].rc.top, 0, 4);

				HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
				HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
				SetTextColor(hdc, RGB(255, 255, 255));
				sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
				TextOut(hdc, vIjeon[k].rc.left, vIjeon[k].rc.top, str, strlen(str));
				SelectObject(hdc, oldFont);
				DeleteObject(myFont);
			}
			else
			{
				vIjeon[k].img->aniRender(hdc, vIjeon[k].rc.left, vIjeon[k].rc.top, playerAni);
			}
		}
		else
		{
			if (isHit)
			{
				vIjeon[k].blockImg->frameRender(hdc, vIjeon[k].rc.left, vIjeon[k].rc.top, 0, 4);

				HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
				HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
				SetTextColor(hdc, RGB(255, 255, 255));
				sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
				TextOut(hdc, vIjeon[k].rc.left, vIjeon[k].rc.top, str, strlen(str));
				SelectObject(hdc, oldFont);
				DeleteObject(myFont);
			}
			else
			{
				vIjeon[k].img->frameAlphaRender(hdc, vIjeon[k].rc.left, vIjeon[k].rc.top, 0, frameY, 100);
			}
		}
	}
}

void Ijeon::mouseMove()
{
	for (int k = 0; k < vIjeon.size(); k++)
	{
		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			if (PtInRect(&vIjeon[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
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
						vIjeon[k].rcAtk[0] = RectMake(vIjeon[k].rc.left - 48, vIjeon[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
						vIjeon[k].rcAtk[1] = RectMake(vIjeon[k].rc.left - 48, vIjeon[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
						vIjeon[k].rcAtk[2] = RectMake(vIjeon[k].rc.left - 48, vIjeon[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
						vIjeon[k].rcAtk[3] = RectMake(vIjeon[k].rc.left + 48, vIjeon[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
						vIjeon[k].rcAtk[4] = RectMake(vIjeon[k].rc.left + 48, vIjeon[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
						vIjeon[k].rcAtk[5] = RectMake(vIjeon[k].rc.left + 48, vIjeon[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
						vIjeon[k].rcAtk[6] = RectMake(vIjeon[k].rc.left, vIjeon[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
						vIjeon[k].rcAtk[7] = RectMake(vIjeon[k].rc.left, vIjeon[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
						atkList.push_back(vIjeon[k].rcAtk[j]);
					}

					//�̵�����
					if (!isStop)
					{
						floodFill(startTile, vIjeon[k].movingCount);
					}
				}
			}

			if (!PtInRect(&vIjeon[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
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
	}

	playerAstar();
	playerMenu();
	playerCollision();
}

void Ijeon::playerMove()
{
	for (int k = 0; k < vIjeon.size(); k++)
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

		if (vIjeon[k].rc.left > 0 || vIjeon[k].rc.right < WINSIZEY ||
			vIjeon[k].rc.top > 0 || vIjeon[k].rc.bottom < WINSIZEY)
		{
			switch (pDirection)
			{
			case PLAYER_LEFT:
				playerX -= speed;
				vIjeon[k].rc = RectMakeCenter(playerX, playerY, vIjeon[k].img->getFrameWidth(), vIjeon[k].img->getFrameHeight());
				break;
			case PLAYER_RIGHT:
				playerX += speed;
				vIjeon[k].rc = RectMakeCenter(playerX, playerY, vIjeon[k].img->getFrameWidth(), vIjeon[k].img->getFrameHeight());
				break;
			case PLAYER_UP:
				playerY -= speed;
				vIjeon[k].rc = RectMakeCenter(playerX, playerY, vIjeon[k].img->getFrameWidth(), vIjeon[k].img->getFrameHeight());
				break;
			case PLAYER_DOWN:
				playerY += speed;
				vIjeon[k].rc = RectMakeCenter(playerX, playerY, vIjeon[k].img->getFrameWidth(), vIjeon[k].img->getFrameHeight());
				break;
			}

			if (playerX == stackX && playerY == stackY)
			{
				isMove = false;
				optimalPath.pop();
			}
		}
	}
}

void Ijeon::playerAstar()
{
	for (int k = 0; k < vIjeon.size(); k++)
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
					vIjeon[k].rcAtk[0] = RectMake(vIjeon[k].rc.left - 48, vIjeon[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
					vIjeon[k].rcAtk[1] = RectMake(vIjeon[k].rc.left - 48, vIjeon[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					vIjeon[k].rcAtk[2] = RectMake(vIjeon[k].rc.left - 48, vIjeon[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					vIjeon[k].rcAtk[3] = RectMake(vIjeon[k].rc.left + 48, vIjeon[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
					vIjeon[k].rcAtk[4] = RectMake(vIjeon[k].rc.left + 48, vIjeon[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					vIjeon[k].rcAtk[5] = RectMake(vIjeon[k].rc.left + 48, vIjeon[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					vIjeon[k].rcAtk[6] = RectMake(vIjeon[k].rc.left, vIjeon[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					vIjeon[k].rcAtk[7] = RectMake(vIjeon[k].rc.left, vIjeon[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(vIjeon[k].rcAtk[j]);
				}

				//�޴����� ��Ʈ
				for (int j = 0; j < 5; j++)
				{
					rcMenu[0] = RectMake(vIjeon[k].rc.left - 97, vIjeon[k].rc.top - 30, 82, 20);
					rcMenu[1] = RectMake(vIjeon[k].rc.left - 97, vIjeon[k].rc.top - 9, 82, 20);
					rcMenu[2] = RectMake(vIjeon[k].rc.left - 97, vIjeon[k].rc.top + 12, 82, 20);
					rcMenu[3] = RectMake(vIjeon[k].rc.left - 97, vIjeon[k].rc.top + 38, 82, 20);
					rcMenu[4] = RectMake(vIjeon[k].rc.left - 97, vIjeon[k].rc.top + 63, 82, 20);
					menuList.push_back(rcMenu[j]);
				}
			}
		}
	}
}

void Ijeon::playerMenu()
{
	for (int k = 0; k < vIjeon.size(); k++)
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

					vIjeon[k].rc.left = prevTile.rc.left;
					vIjeon[k].rc.right = prevTile.rc.right;
					vIjeon[k].rc.top = prevTile.rc.top;
					vIjeon[k].rc.bottom = prevTile.rc.bottom;
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
}

void Ijeon::playerCollision()
{
	for (int k = 0; k < vIjeon.size(); k++)
	{
		RECT temp;

		if (IntersectRect(&temp, &vIjeon[k].rcAtk[0], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
			IntersectRect(&temp, &vIjeon[k].rcAtk[1], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
			IntersectRect(&temp, &vIjeon[k].rcAtk[2], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
			IntersectRect(&temp, &vIjeon[k].rcAtk[3], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
			IntersectRect(&temp, &vIjeon[k].rcAtk[4], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
			IntersectRect(&temp, &vIjeon[k].rcAtk[5], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
			IntersectRect(&temp, &vIjeon[k].rcAtk[6], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
			IntersectRect(&temp, &vIjeon[k].rcAtk[7], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc))
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

void Ijeon::playerState()
{
	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);
}

void Ijeon::setPosition(RECT rc)
{
	for (int k = 0; k < vIjeon.size(); k++)
	{
		vIjeon[k].rc = rc;
		playerX = vIjeon[k].rc.left + (vIjeon[k].rc.right - vIjeon[k].rc.left) / 2;
		playerY = vIjeon[k].rc.top + (vIjeon[k].rc.bottom - vIjeon[k].rc.top) / 2;
	}
}
