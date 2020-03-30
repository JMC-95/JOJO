#include "stdafx.h"
#include "Archer.h"

Archer::Archer(int num)
{
	number = num;
}

Archer::~Archer()
{
}

HRESULT Archer::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * enemyImg, const char * atkImg, const char * blockImg)
{
	//�̸� �� ��
	archer.name = "�ú�";
	archer.face = "���ú�Face";
	archer.className = "�ú�";
	//�̹��� �� �ִϸ��̼�
	archer.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//ĳ���� Ŭ���� �̵����� �̹���
	archer.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//ĳ���� Ŭ���� ���ݹ��� �̹���
	archer.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//���ݹ�ư Ŭ���� ���ݹ��� �̹���
	archer.img = IMAGEMANAGER->findImage(enemyImg);				//ĳ���� �̹���
	archer.atkImg = IMAGEMANAGER->findImage(atkImg);			//���� �̹���
	archer.blockImg = IMAGEMANAGER->findImage(blockImg);		//��� �� �ǰ� �̹���
	ANIMATIONMANAGER->addAnimation("enemyRight", "���ú�", 6, 7, 2, false, true);
	enemyAni = ANIMATIONMANAGER->findAnimation("enemyRight");
	//�������ͽ�
	archer.level = 2;			//����
	archer.hp = 94;				//ü��
	archer.mp = 11;				//����
	archer.atk = 42;			//���ݷ�
	archer.will = 32;			//���ŷ�
	archer.def = 32;			//����
	archer.agi = 32;			//���߷�
	archer.ten = 43;			//���
	archer.movingCount = 4;		//�̵���

	//HP ProgressBar
	_Hp = new progressBar;
	_Hp->init("images/UI/Info/HP.bmp", "images/UI/Info/Back_P.bmp", 1056, 289, 84, 12);
	_Hp->setGauge(currentHp, maxHp);
	currentHp = maxHp = 94;

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

	isMove = true;
	isTurn = false;
	isSelect = false;

	return S_OK;
}

void Archer::release()
{
	SAFE_DELETE(_Hp);
	SAFE_DELETE(_Mp);
	SAFE_DELETE(_Exp);
}

void Archer::update()
{
	if (isTurn)
	{
		mouseMove();
	}

	enemyAnimation();
	enemyState();
}

void Archer::render(HDC hdc)
{
	if (isTurn)
	{
		if (isAtk)
		{
			archer.atkImg->aniRender(hdc, archer.rc.left - 8, archer.rc.top - 8, enemyAni);
		}
		else if (isHit)
		{
			archer.blockImg->frameRender(hdc, archer.rc.left, archer.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, archer.rc.left, archer.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			archer.img->aniRender(hdc, archer.rc.left, archer.rc.top, enemyAni);
		}
	}
	else
	{
		if (isHit)
		{
			archer.blockImg->frameRender(hdc, archer.rc.left, archer.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, archer.rc.left, archer.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			archer.img->frameAlphaRender(hdc, archer.rc.left, archer.rc.top, 0, frameY, 100);
		}
	}
}

void Archer::mouseMove()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&archer.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
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
					archer.rcAtk[0] = RectMake(archer.rc.left - 96, archer.rc.top, TILE_WIDTH, TILE_HEIGHT);
					archer.rcAtk[1] = RectMake(archer.rc.left, archer.rc.top - 96, TILE_WIDTH, TILE_HEIGHT);
					archer.rcAtk[2] = RectMake(archer.rc.left - 48, archer.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					archer.rcAtk[3] = RectMake(archer.rc.left - 48, archer.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					archer.rcAtk[4] = RectMake(archer.rc.left + 96, archer.rc.top, TILE_WIDTH, TILE_HEIGHT);
					archer.rcAtk[5] = RectMake(archer.rc.left, archer.rc.top + 96, TILE_WIDTH, TILE_HEIGHT);
					archer.rcAtk[6] = RectMake(archer.rc.left + 48, archer.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					archer.rcAtk[7] = RectMake(archer.rc.left + 48, archer.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(archer.rcAtk[j]);
				}

				//�̵�����
				if (isMove)
				{
					floodFill(startTile, archer.movingCount);
				}
			}
		}

		if (!PtInRect(&archer.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
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

void Archer::enemyMove()
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

	if (archer.rc.left > 0 || archer.rc.right < WINSIZEY ||
		archer.rc.top > 0 || archer.rc.bottom < WINSIZEY)
	{
		switch (eDirection)
		{
		case ENEMY_LEFT:
			enemyX -= speed;
			archer.rc = RectMakeCenter(enemyX, enemyY, archer.img->getFrameWidth(), archer.img->getFrameHeight());
			break;
		case ENEMY_RIGHT:
			enemyX += speed;
			archer.rc = RectMakeCenter(enemyX, enemyY, archer.img->getFrameWidth(), archer.img->getFrameHeight());
			break;
		case ENEMY_UP:
			enemyY -= speed;
			archer.rc = RectMakeCenter(enemyX, enemyY, archer.img->getFrameWidth(), archer.img->getFrameHeight());
			break;
		case ENEMY_DOWN:
			enemyY += speed;
			archer.rc = RectMakeCenter(enemyX, enemyY, archer.img->getFrameWidth(), archer.img->getFrameHeight());
			break;
		}

		if (enemyX == stackX && enemyY == stackY)
		{
			optimalPath.pop();
		}
	}
}

void Archer::enemyAstar()
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
				archer.rcAtk[0] = RectMake(archer.rc.left - 96, archer.rc.top, TILE_WIDTH, TILE_HEIGHT);
				archer.rcAtk[1] = RectMake(archer.rc.left, archer.rc.top - 96, TILE_WIDTH, TILE_HEIGHT);
				archer.rcAtk[2] = RectMake(archer.rc.left - 48, archer.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				archer.rcAtk[3] = RectMake(archer.rc.left - 48, archer.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				archer.rcAtk[4] = RectMake(archer.rc.left + 96, archer.rc.top, TILE_WIDTH, TILE_HEIGHT);
				archer.rcAtk[5] = RectMake(archer.rc.left, archer.rc.top + 96, TILE_WIDTH, TILE_HEIGHT);
				archer.rcAtk[6] = RectMake(archer.rc.left + 48, archer.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				archer.rcAtk[7] = RectMake(archer.rc.left + 48, archer.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				atkList.push_back(archer.rcAtk[j]);
			}

			//�޴����� ��Ʈ
			for (int j = 0; j < 5; j++)
			{
				rcMenu[0] = RectMake(archer.rc.left - 97, archer.rc.top - 30, 82, 20);
				rcMenu[1] = RectMake(archer.rc.left - 97, archer.rc.top - 9, 82, 20);
				rcMenu[2] = RectMake(archer.rc.left - 97, archer.rc.top + 12, 82, 20);
				rcMenu[3] = RectMake(archer.rc.left - 97, archer.rc.top + 38, 82, 20);
				rcMenu[4] = RectMake(archer.rc.left - 97, archer.rc.top + 63, 82, 20);
				menuList.push_back(rcMenu[j]);
			}
		}
	}
}

void Archer::enemyAnimation()
{
	if (isTurn)
	{
		if (isAtk)
		{
			switch (eDirection)
			{
			case ENEMY_LEFT:
				ANIMATIONMANAGER->addAnimation("leftAtk", "���ú�ATK", 12, 15, 5, false, false);
				enemyAni = ANIMATIONMANAGER->findAnimation("leftAtk");
				ANIMATIONMANAGER->resume("leftAtk");
				break;
			case ENEMY_RIGHT:
				ANIMATIONMANAGER->addAnimation("rightAtk", "���ú�ATK", 8, 11, 5, false, false);
				enemyAni = ANIMATIONMANAGER->findAnimation("rightAtk");
				ANIMATIONMANAGER->resume("rightAtk");
				break;
			case ENEMY_UP:
				ANIMATIONMANAGER->addAnimation("upAtk", "���ú�ATK", 4, 7, 5, false, false);
				enemyAni = ANIMATIONMANAGER->findAnimation("upAtk");
				ANIMATIONMANAGER->resume("upAtk");
				break;
			case ENEMY_DOWN:
				ANIMATIONMANAGER->addAnimation("downAtk", "���ú�ATK", 0, 3, 5, false, false);
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
				ANIMATIONMANAGER->addAnimation("enemyLeft", "���ú�", 4, 5, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyLeft");
				ANIMATIONMANAGER->resume("enemyLeft");
				break;
			case ENEMY_RIGHT:
				ANIMATIONMANAGER->addAnimation("enemyRight", "���ú�", 6, 7, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyRight");
				ANIMATIONMANAGER->resume("enemyRight");
				break;
			case ENEMY_UP:
				ANIMATIONMANAGER->addAnimation("enemyUp", "���ú�", 2, 3, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyUp");
				ANIMATIONMANAGER->resume("enemyUp");
				break;
			case ENEMY_DOWN:
				ANIMATIONMANAGER->addAnimation("enemyDown", "���ú�", 0, 1, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyDown");
				ANIMATIONMANAGER->resume("enemyDown");
				break;
			}

			if (currentHp < 40 && !isSelect && isMove)
			{
				ANIMATIONMANAGER->addAnimation("enemyHp", "���ú�", 12, 13, 2, false, true);
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

void Archer::enemyState()
{
	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);

	if (currentHp < 0) currentHp = 0;
}

void Archer::enemyMenu()
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

void Archer::enemyCollision()
{
}

void Archer::setPosition(RECT rc)
{
	archer.rc = rc;
	enemyX = archer.rc.left + (archer.rc.right - archer.rc.left) / 2;
	enemyY = archer.rc.top + (archer.rc.bottom - archer.rc.top) / 2;
}
