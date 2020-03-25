#include "stdafx.h"
#include "Dogyeom.h"

Dogyeom::Dogyeom()
{
}

Dogyeom::~Dogyeom()
{
}

HRESULT Dogyeom::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * friendImg, const char * atkImg, const char * blockImg)
{
	//�̸� �� ��
	dogyeom.name = "����";
	dogyeom.face = "����Face";
	dogyeom.className = "����";
	//�̹��� �� �ִϸ��̼�
	dogyeom.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//ĳ���� Ŭ���� �̵����� �̹���
	dogyeom.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//ĳ���� Ŭ���� ���ݹ��� �̹���
	dogyeom.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//���ݹ�ư Ŭ���� ���ݹ��� �̹���
	dogyeom.img = IMAGEMANAGER->findImage(friendImg);			//ĳ���� �̹���
	dogyeom.atkImg = IMAGEMANAGER->findImage(atkImg);			//���� �̹���
	dogyeom.blockImg = IMAGEMANAGER->findImage(blockImg);		//��� �� �ǰ� �̹���
	//�������ͽ�
	dogyeom.level = 6;			//����
	dogyeom.hp = 125;			//ü��
	dogyeom.mp = 15;			//����
	dogyeom.atk = 35;			//���ݷ�
	dogyeom.will = 43;			//���ŷ�
	dogyeom.def = 36;			//����
	dogyeom.agi = 62;			//���߷�
	dogyeom.ten = 62;			//���
	dogyeom.movingCount = 6;	//�̵���

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

void Dogyeom::release()
{
	SAFE_DELETE(_Hp);
	SAFE_DELETE(_Mp);
	SAFE_DELETE(_Exp);
}

void Dogyeom::update()
{
	if (isTurn)
	{
		mouseMove();
	}

	friendAnimation();
	friendState();
}

void Dogyeom::render(HDC hdc)
{
	if (isTurn)
	{
		if (isAtk)
		{
			dogyeom.atkImg->aniRender(hdc, dogyeom.rc.left - 8, dogyeom.rc.top - 8, friendAni);
		}
		else if (isHit)
		{
			dogyeom.blockImg->frameRender(hdc, dogyeom.rc.left, dogyeom.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, dogyeom.rc.left, dogyeom.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			dogyeom.img->aniRender(hdc, dogyeom.rc.left, dogyeom.rc.top, friendAni);
		}
	}
	else
	{
		if (isHit)
		{
			dogyeom.blockImg->frameRender(hdc, dogyeom.rc.left, dogyeom.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, dogyeom.rc.left, dogyeom.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			dogyeom.img->frameAlphaRender(hdc, dogyeom.rc.left, dogyeom.rc.top, 0, frameY, 100);
		}
	}
}

void Dogyeom::mouseMove()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&dogyeom.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
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
					dogyeom.rcAtk[0] = RectMake(dogyeom.rc.left - 48, dogyeom.rc.top, TILE_WIDTH, TILE_HEIGHT);
					dogyeom.rcAtk[1] = RectMake(dogyeom.rc.left + 48, dogyeom.rc.top, TILE_WIDTH, TILE_HEIGHT);
					dogyeom.rcAtk[2] = RectMake(dogyeom.rc.left, dogyeom.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					dogyeom.rcAtk[3] = RectMake(dogyeom.rc.left, dogyeom.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(dogyeom.rcAtk[j]);
				}

				//�̵�����
				if (!isStop)
				{
					floodFill(startTile, dogyeom.movingCount);
				}
			}
		}

		if (!PtInRect(&dogyeom.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
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

void Dogyeom::friendMove()
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

	if (dogyeom.rc.left > 0 || dogyeom.rc.right < WINSIZEY ||
		dogyeom.rc.top > 0 || dogyeom.rc.bottom < WINSIZEY)
	{
		switch (fDirection)
		{
		case FRIEND_LEFT:
			friendX -= speed;
			dogyeom.rc = RectMakeCenter(friendX, friendY, dogyeom.img->getFrameWidth(), dogyeom.img->getFrameHeight());
			break;
		case FRIEND_RIGHT:
			friendX += speed;
			dogyeom.rc = RectMakeCenter(friendX, friendY, dogyeom.img->getFrameWidth(), dogyeom.img->getFrameHeight());
			break;
		case FRIEND_UP:
			friendY -= speed;
			dogyeom.rc = RectMakeCenter(friendX, friendY, dogyeom.img->getFrameWidth(), dogyeom.img->getFrameHeight());
			break;
		case FRIEND_DOWN:
			friendY += speed;
			dogyeom.rc = RectMakeCenter(friendX, friendY, dogyeom.img->getFrameWidth(), dogyeom.img->getFrameHeight());
			break;
		}

		if (friendX == stackX && friendY == stackY)
		{
			isMove = false;
			optimalPath.pop();
		}
	}
}

void Dogyeom::friendAstar()
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
			friendMove();
		}

		if (friendX == mapX && friendY == mapY)
		{
			isStop = true;
			isClick = true;

			//���ݹ���
			for (int j = 0; j < 4; j++)
			{
				dogyeom.rcAtk[0] = RectMake(dogyeom.rc.left - 48, dogyeom.rc.top, TILE_WIDTH, TILE_HEIGHT);
				dogyeom.rcAtk[1] = RectMake(dogyeom.rc.left + 48, dogyeom.rc.top, TILE_WIDTH, TILE_HEIGHT);
				dogyeom.rcAtk[2] = RectMake(dogyeom.rc.left, dogyeom.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				dogyeom.rcAtk[3] = RectMake(dogyeom.rc.left, dogyeom.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				atkList.push_back(dogyeom.rcAtk[j]);
			}

			//�޴����� ��Ʈ
			for (int j = 0; j < 5; j++)
			{
				rcMenu[0] = RectMake(dogyeom.rc.left - 97, dogyeom.rc.top - 30, 82, 20);
				rcMenu[1] = RectMake(dogyeom.rc.left - 97, dogyeom.rc.top - 9, 82, 20);
				rcMenu[2] = RectMake(dogyeom.rc.left - 97, dogyeom.rc.top + 12, 82, 20);
				rcMenu[3] = RectMake(dogyeom.rc.left - 97, dogyeom.rc.top + 38, 82, 20);
				rcMenu[4] = RectMake(dogyeom.rc.left - 97, dogyeom.rc.top + 63, 82, 20);
				menuList.push_back(rcMenu[j]);
			}
		}
	}
}

void Dogyeom::friendMenu()
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

void Dogyeom::friendCollision()
{
	RECT temp;
	frameX = 0;

	for (int j = 0; j < ENEMYMANAGER->getEnemy().size(); j++)
	{
		auto enemyX = ENEMYMANAGER->getEnemy()[j]->getEnemyX();
		auto enemyY = ENEMYMANAGER->getEnemy()[j]->getEnemyY();
		auto& enemyRect = ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rc;

		bool isInterSect = false;

		for (int k = 0; k < 8; k++)
		{
			if (IntersectRect(&temp, &dogyeom.rcAtk[k], &enemyRect))
			{
				isInterSect = true;
				break;
			}
		}

		if (isInterSect)
		{
			isTarget = true;
			frameX = 1;

			if (PtInRect(&enemyRect, m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON) && isAtkRng)
			{
				isAtkRng = false;
				isAtk = true;
				isDamage = true;

				if (friendX > enemyX) fDirection = FRIEND_LEFT;
				else if (friendX < enemyX) fDirection = FRIEND_RIGHT;
				else if (friendY > enemyY) fDirection = FRIEND_UP;
				else if (friendY < enemyY) fDirection = FRIEND_DOWN;
			}
		}
	}
}

void Dogyeom::friendAnimation()
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

void Dogyeom::friendState()
{
	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);
}

void Dogyeom::setPosition(RECT rc)
{
	dogyeom.rc = rc;
	friendX = dogyeom.rc.left + (dogyeom.rc.right - dogyeom.rc.left) / 2;
	friendY = dogyeom.rc.top + (dogyeom.rc.bottom - dogyeom.rc.top) / 2;
}
