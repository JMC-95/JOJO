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
	//����ü ���� ����
	FriendInfo jangbi;
	//�̹��� �� �ִϸ��̼�
	jangbi.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//ĳ���� Ŭ���� �̵����� �̹���
	jangbi.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//ĳ���� Ŭ���� ���ݹ��� �̹���
	jangbi.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//���ݹ�ư Ŭ���� ���ݹ��� �̹���
	jangbi.img = IMAGEMANAGER->findImage(friendImg);			//ĳ���� �̹���
	jangbi.atkImg = IMAGEMANAGER->findImage(atkImg);			//���� �̹���
	jangbi.blockImg = IMAGEMANAGER->findImage(blockImg);		//��� �� �ǰ� �̹���
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
	vJangbi.push_back(jangbi);

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
	speed = 6;	//�ӵ�

	isTurn = true;
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
		mouseMove();
	}

	friendAnimation();
	friendState();
}

void Jangbi::render(HDC hdc)
{
	for (int k = 0; k < vJangbi.size(); k++)
	{
		if (isTurn)
		{
			if (isAtk)
			{
				vJangbi[k].atkImg->aniRender(hdc, vJangbi[k].rc.left - 8, vJangbi[k].rc.top - 8, friendAni);
			}
			else if (isHit)
			{
				vJangbi[k].blockImg->frameRender(hdc, vJangbi[k].rc.left, vJangbi[k].rc.top, 0, 4);

				HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
				HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
				SetTextColor(hdc, RGB(255, 255, 255));
				sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
				TextOut(hdc, vJangbi[k].rc.left, vJangbi[k].rc.top, str, strlen(str));
				SelectObject(hdc, oldFont);
				DeleteObject(myFont);
			}
			else
			{
				vJangbi[k].img->aniRender(hdc, vJangbi[k].rc.left, vJangbi[k].rc.top, friendAni);
			}
		}
		else
		{
			if (isHit)
			{
				vJangbi[k].blockImg->frameRender(hdc, vJangbi[k].rc.left, vJangbi[k].rc.top, 0, 4);

				HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
				HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
				SetTextColor(hdc, RGB(255, 255, 255));
				sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
				TextOut(hdc, vJangbi[k].rc.left, vJangbi[k].rc.top, str, strlen(str));
				SelectObject(hdc, oldFont);
				DeleteObject(myFont);
			}
			else
			{
				vJangbi[k].img->frameAlphaRender(hdc, vJangbi[k].rc.left, vJangbi[k].rc.top, 0, frameY, 100);
			}
		}
	}
}

void Jangbi::mouseMove()
{
	for (int k = 0; k < vJangbi.size(); k++)
	{
		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			if (PtInRect(&vJangbi[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
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
						vJangbi[k].rcAtk[0] = RectMake(vJangbi[k].rc.left - 48, vJangbi[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
						vJangbi[k].rcAtk[1] = RectMake(vJangbi[k].rc.left + 48, vJangbi[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
						vJangbi[k].rcAtk[2] = RectMake(vJangbi[k].rc.left, vJangbi[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
						vJangbi[k].rcAtk[3] = RectMake(vJangbi[k].rc.left, vJangbi[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
						atkList.push_back(vJangbi[k].rcAtk[j]);
					}

					//�̵�����
					if (!isStop)
					{
						floodFill(startTile, vJangbi[k].movingCount);
					}
				}
			}

			if (!PtInRect(&vJangbi[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
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

		friendAstar();
		friendMenu();
		friendCollision();
	}
}

void Jangbi::friendMove()
{
	for (int k = 0; k < vJangbi.size(); k++)
	{
		stackX = optimalPath.top().rc.left + (optimalPath.top().rc.right - optimalPath.top().rc.left) / 2;
		stackY = optimalPath.top().rc.top + (optimalPath.top().rc.bottom - optimalPath.top().rc.top) / 2;

		if (!isMove)
		{
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

			isMove = true;
		}

		if (vJangbi[k].rc.left > 0 || vJangbi[k].rc.right < WINSIZEY ||
			vJangbi[k].rc.top > 0 || vJangbi[k].rc.bottom < WINSIZEY)
		{
			switch (fDirection)
			{
			case FRIEND_LEFT:
				friendX -= speed;
				vJangbi[k].rc = RectMakeCenter(friendX, friendY, vJangbi[k].img->getFrameWidth(), vJangbi[k].img->getFrameHeight());
				break;
			case FRIEND_RIGHT:
				friendX += speed;
				vJangbi[k].rc = RectMakeCenter(friendX, friendY, vJangbi[k].img->getFrameWidth(), vJangbi[k].img->getFrameHeight());
				break;
			case FRIEND_UP:
				friendY -= speed;
				vJangbi[k].rc = RectMakeCenter(friendX, friendY, vJangbi[k].img->getFrameWidth(), vJangbi[k].img->getFrameHeight());
				break;
			case FRIEND_DOWN:
				friendY += speed;
				vJangbi[k].rc = RectMakeCenter(friendX, friendY, vJangbi[k].img->getFrameWidth(), vJangbi[k].img->getFrameHeight());
				break;
			}

			if (friendX == stackX && friendY == stackY)
			{
				isMove = false;
				optimalPath.pop();
			}
		}
	}
}

void Jangbi::friendAstar()
{
	for (int k = 0; k < vJangbi.size(); k++)
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
				friendMove();

			if (friendX == mapX && friendY == mapY)
			{
				isStop = true;
				isClick = true;

				//���ݹ���
				for (int j = 0; j < 4; j++)
				{
					vJangbi[k].rcAtk[0] = RectMake(vJangbi[k].rc.left - 48, vJangbi[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
					vJangbi[k].rcAtk[1] = RectMake(vJangbi[k].rc.left + 48, vJangbi[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
					vJangbi[k].rcAtk[2] = RectMake(vJangbi[k].rc.left, vJangbi[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					vJangbi[k].rcAtk[3] = RectMake(vJangbi[k].rc.left, vJangbi[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(vJangbi[k].rcAtk[j]);
				}

				//�޴����� ��Ʈ
				for (int j = 0; j < 5; j++)
				{
					rcMenu[0] = RectMake(vJangbi[k].rc.left - 97, vJangbi[k].rc.top - 30, 82, 20);
					rcMenu[1] = RectMake(vJangbi[k].rc.left - 97, vJangbi[k].rc.top - 9, 82, 20);
					rcMenu[2] = RectMake(vJangbi[k].rc.left - 97, vJangbi[k].rc.top + 12, 82, 20);
					rcMenu[3] = RectMake(vJangbi[k].rc.left - 97, vJangbi[k].rc.top + 38, 82, 20);
					rcMenu[4] = RectMake(vJangbi[k].rc.left - 97, vJangbi[k].rc.top + 63, 82, 20);
					menuList.push_back(rcMenu[j]);
				}
			}
		}
	}
}

void Jangbi::friendMenu()
{
	for (int k = 0; k < vJangbi.size(); k++)
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

void Jangbi::friendCollision()
{
	for (int k = 0; k < vJangbi.size(); k++)
	{
		RECT temp;

		if (IntersectRect(&temp, &vJangbi[k].rcAtk[0], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
			IntersectRect(&temp, &vJangbi[k].rcAtk[1], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
			IntersectRect(&temp, &vJangbi[k].rcAtk[2], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
			IntersectRect(&temp, &vJangbi[k].rcAtk[3], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc))
		{
			isTarget = true;
			frameX = 1;

			if (PtInRect(&ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc, m_ptMouse) &&
				KEYMANAGER->isStayKeyDown(VK_LBUTTON) && isAtkRng)
			{
				isAtkRng = false;
				isAtk = true;

				if (friendX > ENEMYMANAGER->getYeopo()->getEnemyX())
					fDirection = FRIEND_LEFT;
				else if (friendX < ENEMYMANAGER->getYeopo()->getEnemyX())
					fDirection = FRIEND_RIGHT;
				else if (friendY > ENEMYMANAGER->getYeopo()->getEnemyY())
					fDirection = FRIEND_UP;
				else if (friendY < ENEMYMANAGER->getYeopo()->getEnemyY())
					fDirection = FRIEND_DOWN;
			}
		}
		else
		{
			frameX = 0;
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
	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);
}

void Jangbi::setPosition(RECT rc)
{
	for (int k = 0; k < vJangbi.size(); k++)
	{
		vJangbi[k].rc = rc;
		friendX = vJangbi[k].rc.left + (vJangbi[k].rc.right - vJangbi[k].rc.left) / 2;
		friendY = vJangbi[k].rc.top + (vJangbi[k].rc.bottom - vJangbi[k].rc.top) / 2;
	}
}
