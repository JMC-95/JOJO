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
	//����ü ���� ����
	FriendInfo yubi;
	//�̹��� �� �ִϸ��̼�
	yubi.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//ĳ���� Ŭ���� �̵����� �̹���
	yubi.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//ĳ���� Ŭ���� ���ݹ��� �̹���
	yubi.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//���ݹ�ư Ŭ���� ���ݹ��� �̹���
	yubi.img = IMAGEMANAGER->findImage(friendImg);			//ĳ���� �̹���
	yubi.atkImg = IMAGEMANAGER->findImage(atkImg);			//���� �̹���
	yubi.blockImg = IMAGEMANAGER->findImage(blockImg);		//��� �� �ǰ� �̹���
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
	vYubi.push_back(yubi);

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
	speed = 6;	//�ӵ�

	isTurn = true;
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
		mouseMove();
	}

	friendAnimation();
	friendState();
}

void Yubi::render(HDC hdc)
{
	for (int k = 0; k < vYubi.size(); k++)
	{
		if (isTurn)
		{
			if (isAtk)
			{
				vYubi[k].atkImg->aniRender(hdc, vYubi[k].rc.left - 8, vYubi[k].rc.top - 8, friendAni);
			}
			else if (isHit)
			{
				vYubi[k].blockImg->frameRender(hdc, vYubi[k].rc.left, vYubi[k].rc.top, 0, 4);

				HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
				HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
				SetTextColor(hdc, RGB(255, 255, 255));
				sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
				TextOut(hdc, vYubi[k].rc.left, vYubi[k].rc.top, str, strlen(str));
				SelectObject(hdc, oldFont);
				DeleteObject(myFont);
			}
			else
			{
				vYubi[k].img->aniRender(hdc, vYubi[k].rc.left, vYubi[k].rc.top, friendAni);
			}
		}
		else
		{
			if (isHit)
			{
				vYubi[k].blockImg->frameRender(hdc, vYubi[k].rc.left, vYubi[k].rc.top, 0, 4);

				HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
				HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
				SetTextColor(hdc, RGB(255, 255, 255));
				sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
				TextOut(hdc, vYubi[k].rc.left, vYubi[k].rc.top, str, strlen(str));
				SelectObject(hdc, oldFont);
				DeleteObject(myFont);
			}
			else
			{
				vYubi[k].img->frameAlphaRender(hdc, vYubi[k].rc.left, vYubi[k].rc.top, 0, frameY, 100);
			}
		}
	}
}

void Yubi::mouseMove()
{
	for (int k = 0; k < vYubi.size(); k++)
	{
		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			if (PtInRect(&vYubi[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
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
						vYubi[k].rcAtk[0] = RectMake(vYubi[k].rc.left - 48, vYubi[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
						vYubi[k].rcAtk[1] = RectMake(vYubi[k].rc.left + 48, vYubi[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
						vYubi[k].rcAtk[2] = RectMake(vYubi[k].rc.left, vYubi[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
						vYubi[k].rcAtk[3] = RectMake(vYubi[k].rc.left, vYubi[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
						atkList.push_back(vYubi[k].rcAtk[j]);
					}

					//�̵�����
					if (!isStop)
					{
						floodFill(startTile, vYubi[k].movingCount);
					}
				}
			}

			if (!PtInRect(&vYubi[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
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

void Yubi::friendMove()
{
	for (int k = 0; k < vYubi.size(); k++)
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

		if (vYubi[k].rc.left > 0 || vYubi[k].rc.right < WINSIZEY ||
			vYubi[k].rc.top > 0 || vYubi[k].rc.bottom < WINSIZEY)
		{
			switch (fDirection)
			{
			case FRIEND_LEFT:
				friendX -= speed;
				vYubi[k].rc = RectMakeCenter(friendX, friendY, vYubi[k].img->getFrameWidth(), vYubi[k].img->getFrameHeight());
				break;
			case FRIEND_RIGHT:
				friendX += speed;
				vYubi[k].rc = RectMakeCenter(friendX, friendY, vYubi[k].img->getFrameWidth(), vYubi[k].img->getFrameHeight());
				break;
			case FRIEND_UP:
				friendY -= speed;
				vYubi[k].rc = RectMakeCenter(friendX, friendY, vYubi[k].img->getFrameWidth(), vYubi[k].img->getFrameHeight());
				break;
			case FRIEND_DOWN:
				friendY += speed;
				vYubi[k].rc = RectMakeCenter(friendX, friendY, vYubi[k].img->getFrameWidth(), vYubi[k].img->getFrameHeight());
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

void Yubi::friendAstar()
{
	for (int k = 0; k < vYubi.size(); k++)
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
					vYubi[k].rcAtk[0] = RectMake(vYubi[k].rc.left - 48, vYubi[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
					vYubi[k].rcAtk[1] = RectMake(vYubi[k].rc.left + 48, vYubi[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
					vYubi[k].rcAtk[2] = RectMake(vYubi[k].rc.left, vYubi[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					vYubi[k].rcAtk[3] = RectMake(vYubi[k].rc.left, vYubi[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(vYubi[k].rcAtk[j]);
				}

				//�޴����� ��Ʈ
				for (int j = 0; j < 5; j++)
				{
					rcMenu[0] = RectMake(vYubi[k].rc.left - 97, vYubi[k].rc.top - 30, 82, 20);
					rcMenu[1] = RectMake(vYubi[k].rc.left - 97, vYubi[k].rc.top - 9, 82, 20);
					rcMenu[2] = RectMake(vYubi[k].rc.left - 97, vYubi[k].rc.top + 12, 82, 20);
					rcMenu[3] = RectMake(vYubi[k].rc.left - 97, vYubi[k].rc.top + 38, 82, 20);
					rcMenu[4] = RectMake(vYubi[k].rc.left - 97, vYubi[k].rc.top + 63, 82, 20);
					menuList.push_back(rcMenu[j]);
				}
			}
		}
	}
}

void Yubi::friendMenu()
{
	for (int k = 0; k < vYubi.size(); k++)
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

void Yubi::friendCollision()
{
	for (int k = 0; k < vYubi.size(); k++)
	{
		RECT temp;

		if (IntersectRect(&temp, &vYubi[k].rcAtk[0], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
			IntersectRect(&temp, &vYubi[k].rcAtk[1], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
			IntersectRect(&temp, &vYubi[k].rcAtk[2], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
			IntersectRect(&temp, &vYubi[k].rcAtk[3], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc))
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
}

void Yubi::setPosition(RECT rc)
{
	for (int k = 0; k < vYubi.size(); k++)
	{
		vYubi[k].rc = rc;
		friendX = vYubi[k].rc.left + (vYubi[k].rc.right - vYubi[k].rc.left) / 2;
		friendY = vYubi[k].rc.top + (vYubi[k].rc.bottom - vYubi[k].rc.top) / 2;
	}
}
