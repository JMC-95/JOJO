#include "stdafx.h"
#include "Beonju.h"

Beonju::Beonju()
{
}

Beonju::~Beonju()
{
}

HRESULT Beonju::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * enemyImg, const char * atkImg, const char * blockImg)
{
	//����ü ���� ����
	EnemyInfo beonju;
	//�̹��� �� �ִϸ��̼�
	beonju.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//ĳ���� Ŭ���� �̵����� �̹���
	beonju.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//ĳ���� Ŭ���� ���ݹ��� �̹���
	beonju.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//���ݹ�ư Ŭ���� ���ݹ��� �̹���
	beonju.img = IMAGEMANAGER->findImage(enemyImg);				//ĳ���� �̹���
	beonju.atkImg = IMAGEMANAGER->findImage(atkImg);			//���� �̹���
	beonju.blockImg = IMAGEMANAGER->findImage(blockImg);		//��� �� �ǰ� �̹���
	//�������ͽ�
	beonju.level = 6;			//����
	beonju.hp = 125;			//ü��
	beonju.mp = 15;				//����
	beonju.atk = 51;			//���ݷ�
	beonju.will = 41;			//���ŷ�
	beonju.def = 34;			//����
	beonju.agi = 62;			//���߷�
	beonju.ten = 50;			//���
	beonju.movingCount = 6;		//�̵���
	vBeonju.push_back(beonju);

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
	eDirection = ENEMY_RIGHT;
	startTile = endTile = -1;
	speed = 6;	//�ӵ�

	isTurn = true;
	isSelect = false;

	return S_OK;
}

void Beonju::release()
{
	SAFE_DELETE(_Hp);
	SAFE_DELETE(_Mp);
	SAFE_DELETE(_Exp);
}

void Beonju::update()
{
	if (isTurn)
	{
		mouseMove();
	}

	enemyAnimation();
	enemyState();
}

void Beonju::render(HDC hdc)
{
	for (int k = 0; k < vBeonju.size(); k++)
	{
		if (isTurn)
		{
			if (isAtk)
			{
				vBeonju[k].atkImg->aniRender(hdc, vBeonju[k].rc.left - 8, vBeonju[k].rc.top - 8, enemyAni);
			}
			else if (isHit)
			{
				vBeonju[k].blockImg->frameRender(hdc, vBeonju[k].rc.left, vBeonju[k].rc.top, 0, 4);

				HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
				HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
				SetTextColor(hdc, RGB(255, 255, 255));
				sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
				TextOut(hdc, vBeonju[k].rc.left, vBeonju[k].rc.top, str, strlen(str));
				SelectObject(hdc, oldFont);
				DeleteObject(myFont);
			}
			else
			{
				vBeonju[k].img->aniRender(hdc, vBeonju[k].rc.left, vBeonju[k].rc.top, enemyAni);
			}
		}
		else
		{
			if (isHit)
			{
				vBeonju[k].blockImg->frameRender(hdc, vBeonju[k].rc.left, vBeonju[k].rc.top, 0, 4);

				HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
				HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
				SetTextColor(hdc, RGB(255, 255, 255));
				sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
				TextOut(hdc, vBeonju[k].rc.left, vBeonju[k].rc.top, str, strlen(str));
				SelectObject(hdc, oldFont);
				DeleteObject(myFont);
			}
			else
			{
				vBeonju[k].img->frameAlphaRender(hdc, vBeonju[k].rc.left, vBeonju[k].rc.top, 0, frameY, 100);
			}
		}
	}
}

void Beonju::mouseMove()
{
	for (int k = 0; k < vBeonju.size(); k++)
	{
		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			if (PtInRect(&vBeonju[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
			{
				if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
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
						vBeonju[k].rcAtk[0] = RectMake(vBeonju[k].rc.left - 48, vBeonju[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
						vBeonju[k].rcAtk[1] = RectMake(vBeonju[k].rc.left + 48, vBeonju[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
						vBeonju[k].rcAtk[2] = RectMake(vBeonju[k].rc.left, vBeonju[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
						vBeonju[k].rcAtk[3] = RectMake(vBeonju[k].rc.left, vBeonju[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
						atkList.push_back(vBeonju[k].rcAtk[j]);
					}

					//�̵�����
					if (!isStop)
					{
						floodFill(startTile, vBeonju[k].movingCount);
					}
				}
			}

			if (!PtInRect(&vBeonju[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
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

		enemyAstar();
		enemyMenu();
		enemyCollision();
	}
}

void Beonju::enemyMove()
{
	for (int k = 0; k < vBeonju.size(); k++)
	{
		stackX = optimalPath.top().rc.left + (optimalPath.top().rc.right - optimalPath.top().rc.left) / 2;
		stackY = optimalPath.top().rc.top + (optimalPath.top().rc.bottom - optimalPath.top().rc.top) / 2;

		if (!isMove)
		{
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

			isMove = true;
		}

		if (vBeonju[k].rc.left > 0 || vBeonju[k].rc.right < WINSIZEY ||
			vBeonju[k].rc.top > 0 || vBeonju[k].rc.bottom < WINSIZEY)
		{
			switch (eDirection)
			{
			case ENEMY_LEFT:
				enemyX -= speed;
				vBeonju[k].rc = RectMakeCenter(enemyX, enemyY, vBeonju[k].img->getFrameWidth(), vBeonju[k].img->getFrameHeight());
				break;
			case ENEMY_RIGHT:
				enemyX += speed;
				vBeonju[k].rc = RectMakeCenter(enemyX, enemyY, vBeonju[k].img->getFrameWidth(), vBeonju[k].img->getFrameHeight());
				break;
			case ENEMY_UP:
				enemyY -= speed;
				vBeonju[k].rc = RectMakeCenter(enemyX, enemyY, vBeonju[k].img->getFrameWidth(), vBeonju[k].img->getFrameHeight());
				break;
			case ENEMY_DOWN:
				enemyY += speed;
				vBeonju[k].rc = RectMakeCenter(enemyX, enemyY, vBeonju[k].img->getFrameWidth(), vBeonju[k].img->getFrameHeight());
				break;
			}

			if (enemyX == stackX && enemyY == stackY)
			{
				isMove = false;
				optimalPath.pop();
			}
		}
	}
}

void Beonju::enemyAstar()
{
	for (int k = 0; k < vBeonju.size(); k++)
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
				enemyMove();

			if (enemyX == mapX && enemyY == mapY)
			{
				isStop = true;
				isClick = true;

				//���ݹ���
				for (int j = 0; j < 4; j++)
				{
					vBeonju[k].rcAtk[0] = RectMake(vBeonju[k].rc.left - 48, vBeonju[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
					vBeonju[k].rcAtk[1] = RectMake(vBeonju[k].rc.left + 48, vBeonju[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
					vBeonju[k].rcAtk[2] = RectMake(vBeonju[k].rc.left, vBeonju[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					vBeonju[k].rcAtk[3] = RectMake(vBeonju[k].rc.left, vBeonju[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(vBeonju[k].rcAtk[j]);
				}

				//�޴����� ��Ʈ
				for (int j = 0; j < 5; j++)
				{
					rcMenu[0] = RectMake(vBeonju[k].rc.left - 97, vBeonju[k].rc.top - 30, 82, 20);
					rcMenu[1] = RectMake(vBeonju[k].rc.left - 97, vBeonju[k].rc.top - 9, 82, 20);
					rcMenu[2] = RectMake(vBeonju[k].rc.left - 97, vBeonju[k].rc.top + 12, 82, 20);
					rcMenu[3] = RectMake(vBeonju[k].rc.left - 97, vBeonju[k].rc.top + 38, 82, 20);
					rcMenu[4] = RectMake(vBeonju[k].rc.left - 97, vBeonju[k].rc.top + 63, 82, 20);
					menuList.push_back(rcMenu[j]);
				}
			}
		}
	}
}

void Beonju::enemyAnimation()
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

void Beonju::enemyState()
{
	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);
}

void Beonju::enemyMenu()
{
	for (int k = 0; k < vBeonju.size(); k++)
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

					isSelect = false;
					isStop = false;
					isClick = false;
				}
			}
		}
	}
}

void Beonju::enemyCollision()
{
	/*for (int k = 0; k < vBeonju.size(); k++)
	{
		RECT temp;

		if (IntersectRect(&temp, &vBeonju[k].rcAtk[0], &PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vBeonju[k].rcAtk[1], &PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vBeonju[k].rcAtk[2], &PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vBeonju[k].rcAtk[3], &PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rc))
		{
			isTarget = true;
			frameX = 1;

			if (PtInRect(&PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rc, m_ptMouse) &&
				KEYMANAGER->isStayKeyDown(VK_LBUTTON) && isAtkRng)
			{
				isAtkRng = false;
				isAtk = true;

				if (enemyX > PLAYERMANAGER->getAgjin()->getPlayerX())
					eDirection = ENEMY_LEFT;
				else if (enemyX < PLAYERMANAGER->getAgjin()->getPlayerX())
					eDirection = ENEMY_RIGHT;
				else if (enemyY > PLAYERMANAGER->getAgjin()->getPlayerY())
					eDirection = ENEMY_UP;
				else if (enemyY < PLAYERMANAGER->getAgjin()->getPlayerY())
					eDirection = ENEMY_DOWN;
			}
		}
		else if (IntersectRect(&temp, &vBeonju[k].rcAtk[0], &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vBeonju[k].rcAtk[1], &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vBeonju[k].rcAtk[2], &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vBeonju[k].rcAtk[3], &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc))
		{
			isTarget = true;
			frameX = 1;

			if (PtInRect(&PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc, m_ptMouse) &&
				KEYMANAGER->isStayKeyDown(VK_LBUTTON) && isAtkRng)
			{
				isAtkRng = false;
				isAtk = true;

				if (enemyX > PLAYERMANAGER->getHahudon()->getPlayerX())
					eDirection = ENEMY_LEFT;
				else if (enemyX < PLAYERMANAGER->getHahudon()->getPlayerX())
					eDirection = ENEMY_RIGHT;
				else if (enemyY > PLAYERMANAGER->getHahudon()->getPlayerY())
					eDirection = ENEMY_UP;
				else if (enemyY < PLAYERMANAGER->getHahudon()->getPlayerY())
					eDirection = ENEMY_DOWN;
			}
		}
		else if (IntersectRect(&temp, &vBeonju[k].rcAtk[0], &PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vBeonju[k].rcAtk[1], &PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vBeonju[k].rcAtk[2], &PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vBeonju[k].rcAtk[3], &PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rc))
		{
			isTarget = true;
			frameX = 1;

			if (PtInRect(&PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rc, m_ptMouse) &&
				KEYMANAGER->isStayKeyDown(VK_LBUTTON) && isAtkRng)
			{
				isAtkRng = false;
				isAtk = true;

				if (enemyX > PLAYERMANAGER->getHahuyeon()->getPlayerX())
					eDirection = ENEMY_LEFT;
				else if (enemyX < PLAYERMANAGER->getHahuyeon()->getPlayerX())
					eDirection = ENEMY_RIGHT;
				else if (enemyY > PLAYERMANAGER->getHahuyeon()->getPlayerY())
					eDirection = ENEMY_UP;
				else if (enemyY < PLAYERMANAGER->getHahuyeon()->getPlayerY())
					eDirection = ENEMY_DOWN;
			}
		}
		else if (IntersectRect(&temp, &vBeonju[k].rcAtk[0], &PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vBeonju[k].rcAtk[1], &PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vBeonju[k].rcAtk[2], &PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vBeonju[k].rcAtk[3], &PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rc))
		{
			isTarget = true;
			frameX = 1;

			if (PtInRect(&PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rc, m_ptMouse) &&
				KEYMANAGER->isStayKeyDown(VK_LBUTTON) && isAtkRng)
			{
				isAtkRng = false;
				isAtk = true;

				if (enemyX > PLAYERMANAGER->getIjeon()->getPlayerX())
					eDirection = ENEMY_LEFT;
				else if (enemyX < PLAYERMANAGER->getIjeon()->getPlayerX())
					eDirection = ENEMY_RIGHT;
				else if (enemyY > PLAYERMANAGER->getIjeon()->getPlayerY())
					eDirection = ENEMY_UP;
				else if (enemyY < PLAYERMANAGER->getIjeon()->getPlayerY())
					eDirection = ENEMY_DOWN;
			}
		}
		else if (IntersectRect(&temp, &vBeonju[k].rcAtk[0], &PLAYERMANAGER->getJohong()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vBeonju[k].rcAtk[1], &PLAYERMANAGER->getJohong()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vBeonju[k].rcAtk[2], &PLAYERMANAGER->getJohong()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vBeonju[k].rcAtk[3], &PLAYERMANAGER->getJohong()->getPlayerVector()[0].rc))
		{
			isTarget = true;
			frameX = 1;

			if (PtInRect(&PLAYERMANAGER->getJohong()->getPlayerVector()[0].rc, m_ptMouse) &&
				KEYMANAGER->isStayKeyDown(VK_LBUTTON) && isAtkRng)
			{
				isAtkRng = false;
				isAtk = true;

				if (enemyX > PLAYERMANAGER->getJohong()->getPlayerX())
					eDirection = ENEMY_LEFT;
				else if (enemyX < PLAYERMANAGER->getJohong()->getPlayerX())
					eDirection = ENEMY_RIGHT;
				else if (enemyY > PLAYERMANAGER->getJohong()->getPlayerY())
					eDirection = ENEMY_UP;
				else if (enemyY < PLAYERMANAGER->getJohong()->getPlayerY())
					eDirection = ENEMY_DOWN;
			}
		}
		else if (IntersectRect(&temp, &vBeonju[k].rcAtk[0], &PLAYERMANAGER->getJoin()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vBeonju[k].rcAtk[1], &PLAYERMANAGER->getJoin()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vBeonju[k].rcAtk[2], &PLAYERMANAGER->getJoin()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vBeonju[k].rcAtk[3], &PLAYERMANAGER->getJoin()->getPlayerVector()[0].rc))
		{
			isTarget = true;
			frameX = 1;

			if (PtInRect(&PLAYERMANAGER->getJoin()->getPlayerVector()[0].rc, m_ptMouse) &&
				KEYMANAGER->isStayKeyDown(VK_LBUTTON) && isAtkRng)
			{
				isAtkRng = false;
				isAtk = true;

				if (enemyX > PLAYERMANAGER->getJoin()->getPlayerX())
					eDirection = ENEMY_LEFT;
				else if (enemyX < PLAYERMANAGER->getJoin()->getPlayerX())
					eDirection = ENEMY_RIGHT;
				else if (enemyY > PLAYERMANAGER->getJoin()->getPlayerY())
					eDirection = ENEMY_UP;
				else if (enemyY < PLAYERMANAGER->getJoin()->getPlayerY())
					eDirection = ENEMY_DOWN;
			}
		}
		else if (IntersectRect(&temp, &vBeonju[k].rcAtk[0], &PLAYERMANAGER->getJojo()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vBeonju[k].rcAtk[1], &PLAYERMANAGER->getJojo()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vBeonju[k].rcAtk[2], &PLAYERMANAGER->getJojo()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vBeonju[k].rcAtk[3], &PLAYERMANAGER->getJojo()->getPlayerVector()[0].rc))
		{
			isTarget = true;
			frameX = 1;

			if (PtInRect(&PLAYERMANAGER->getJojo()->getPlayerVector()[0].rc, m_ptMouse) &&
				KEYMANAGER->isStayKeyDown(VK_LBUTTON) && isAtkRng)
			{
				isAtkRng = false;
				isAtk = true;

				if (enemyX > PLAYERMANAGER->getJojo()->getPlayerX())
					eDirection = ENEMY_LEFT;
				else if (enemyX < PLAYERMANAGER->getJojo()->getPlayerX())
					eDirection = ENEMY_RIGHT;
				else if (enemyY > PLAYERMANAGER->getJojo()->getPlayerY())
					eDirection = ENEMY_UP;
				else if (enemyY < PLAYERMANAGER->getJojo()->getPlayerY())
					eDirection = ENEMY_DOWN;
			}
		}
		else
		{
			frameX = 0;
		}
	}*/
}

void Beonju::setPosition(RECT rc)
{
	for (int k = 0; k < vBeonju.size(); k++)
	{
		vBeonju[k].rc = rc;
		enemyX = vBeonju[k].rc.left + (vBeonju[k].rc.right - vBeonju[k].rc.left) / 2;
		enemyY = vBeonju[k].rc.top + (vBeonju[k].rc.bottom - vBeonju[k].rc.top) / 2;
	}
}
