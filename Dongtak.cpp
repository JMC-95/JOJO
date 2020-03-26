#include "stdafx.h"
#include "Dongtak.h"

Dongtak::Dongtak()
{
}

Dongtak::~Dongtak()
{
}

HRESULT Dongtak::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * enemyImg, const char * atkImg, const char * blockImg)
{
	//�̸� �� ��
	dongtak.name = "��Ź";
	dongtak.face = "��ŹFace";
	dongtak.className = "����";
	//�̹��� �� �ִϸ��̼�
	dongtak.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//ĳ���� Ŭ���� �̵����� �̹���
	dongtak.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//ĳ���� Ŭ���� ���ݹ��� �̹���
	dongtak.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//���ݹ�ư Ŭ���� ���ݹ��� �̹���
	dongtak.img = IMAGEMANAGER->findImage(enemyImg);			//ĳ���� �̹���
	dongtak.atkImg = IMAGEMANAGER->findImage(atkImg);			//���� �̹���
	dongtak.blockImg = IMAGEMANAGER->findImage(blockImg);		//��� �� �ǰ� �̹���
	//�������ͽ�
	dongtak.level = 6;			//����
	dongtak.hp = 135;			//ü��
	dongtak.mp = 10;			//����
	dongtak.atk = 58;			//���ݷ�
	dongtak.will = 51;			//���ŷ�
	dongtak.def = 57;			//����
	dongtak.agi = 55;			//���߷�
	dongtak.ten = 55;			//���
	dongtak.movingCount = 6;	//�̵���

	//HP ProgressBar
	_Hp = new progressBar;
	_Hp->init("images/UI/Info/HP.bmp", "images/UI/Info/Back_P.bmp", 1056, 289, 84, 12);
	_Hp->setGauge(currentHp, maxHp);
	currentHp = maxHp = 135;

	//MP ProgressBar
	_Mp = new progressBar;
	_Mp->init("images/UI/Info/MP.bmp", "images/UI/Info/Back_P.bmp", 1056, 307, 84, 12);
	_Mp->setGauge(currentMp, maxMp);
	currentMp = maxMp = 10;

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

	isTurn = false;
	isSelect = false;

	return S_OK;
}

void Dongtak::release()
{
	SAFE_DELETE(_Hp);
	SAFE_DELETE(_Mp);
	SAFE_DELETE(_Exp);
}

void Dongtak::update()
{
	if (isTurn)
	{
		mouseMove();
	}

	enemyAnimation();
	enemyState();
}

void Dongtak::render(HDC hdc)
{
	if (isTurn)
	{
		if (isAtk)
		{
			dongtak.atkImg->aniRender(hdc, dongtak.rc.left - 8, dongtak.rc.top - 8, enemyAni);
		}
		else if (isHit)
		{
			dongtak.blockImg->frameRender(hdc, dongtak.rc.left, dongtak.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, dongtak.rc.left, dongtak.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			dongtak.img->aniRender(hdc, dongtak.rc.left, dongtak.rc.top, enemyAni);
		}
	}
	else
	{
		if (isHit)
		{
			dongtak.blockImg->frameRender(hdc, dongtak.rc.left, dongtak.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, dongtak.rc.left, dongtak.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			dongtak.img->frameAlphaRender(hdc, dongtak.rc.left, dongtak.rc.top, 0, frameY, 100);
		}
	}
}

void Dongtak::mouseMove()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&dongtak.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
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
					dongtak.rcAtk[0] = RectMake(dongtak.rc.left - 48, dongtak.rc.top, TILE_WIDTH, TILE_HEIGHT);
					dongtak.rcAtk[1] = RectMake(dongtak.rc.left + 48, dongtak.rc.top, TILE_WIDTH, TILE_HEIGHT);
					dongtak.rcAtk[2] = RectMake(dongtak.rc.left, dongtak.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					dongtak.rcAtk[3] = RectMake(dongtak.rc.left, dongtak.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(dongtak.rcAtk[j]);
				}

				//�̵�����
				if (!isStop)
				{
					floodFill(startTile, dongtak.movingCount);
				}
			}
		}

		if (!PtInRect(&dongtak.rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
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

void Dongtak::enemyMove()
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

	if (dongtak.rc.left > 0 || dongtak.rc.right < WINSIZEY ||
		dongtak.rc.top > 0 || dongtak.rc.bottom < WINSIZEY)
	{
		switch (eDirection)
		{
		case ENEMY_LEFT:
			enemyX -= speed;
			dongtak.rc = RectMakeCenter(enemyX, enemyY, dongtak.img->getFrameWidth(), dongtak.img->getFrameHeight());
			break;
		case ENEMY_RIGHT:
			enemyX += speed;
			dongtak.rc = RectMakeCenter(enemyX, enemyY, dongtak.img->getFrameWidth(), dongtak.img->getFrameHeight());
			break;
		case ENEMY_UP:
			enemyY -= speed;
			dongtak.rc = RectMakeCenter(enemyX, enemyY, dongtak.img->getFrameWidth(), dongtak.img->getFrameHeight());
			break;
		case ENEMY_DOWN:
			enemyY += speed;
			dongtak.rc = RectMakeCenter(enemyX, enemyY, dongtak.img->getFrameWidth(), dongtak.img->getFrameHeight());
			break;
		}

		if (enemyX == stackX && enemyY == stackY)
		{
			isMove = false;
			optimalPath.pop();
		}
	}
}

void Dongtak::enemyAstar()
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
			enemyMove();
		}

		if (enemyX == mapX && enemyY == mapY)
		{
			isStop = true;
			isClick = true;

			//���ݹ���
			for (int j = 0; j < 4; j++)
			{
				dongtak.rcAtk[0] = RectMake(dongtak.rc.left - 48, dongtak.rc.top, TILE_WIDTH, TILE_HEIGHT);
				dongtak.rcAtk[1] = RectMake(dongtak.rc.left + 48, dongtak.rc.top, TILE_WIDTH, TILE_HEIGHT);
				dongtak.rcAtk[2] = RectMake(dongtak.rc.left, dongtak.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				dongtak.rcAtk[3] = RectMake(dongtak.rc.left, dongtak.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				atkList.push_back(dongtak.rcAtk[j]);
			}

			//�޴����� ��Ʈ
			for (int j = 0; j < 5; j++)
			{
				rcMenu[0] = RectMake(dongtak.rc.left - 97, dongtak.rc.top - 30, 82, 20);
				rcMenu[1] = RectMake(dongtak.rc.left - 97, dongtak.rc.top - 9, 82, 20);
				rcMenu[2] = RectMake(dongtak.rc.left - 97, dongtak.rc.top + 12, 82, 20);
				rcMenu[3] = RectMake(dongtak.rc.left - 97, dongtak.rc.top + 38, 82, 20);
				rcMenu[4] = RectMake(dongtak.rc.left - 97, dongtak.rc.top + 63, 82, 20);
				menuList.push_back(rcMenu[j]);
			}
		}
	}
}

void Dongtak::enemyAnimation()
{
	if (isTurn)
	{
		if (isAtk)
		{
			switch (eDirection)
			{
			case ENEMY_LEFT:
				ANIMATIONMANAGER->addAnimation("leftAtk", "��ŹATK", 12, 15, 5, false, false);
				enemyAni = ANIMATIONMANAGER->findAnimation("leftAtk");
				ANIMATIONMANAGER->resume("leftAtk");
				break;
			case ENEMY_RIGHT:
				ANIMATIONMANAGER->addAnimation("rightAtk", "��ŹATK", 8, 11, 5, false, false);
				enemyAni = ANIMATIONMANAGER->findAnimation("rightAtk");
				ANIMATIONMANAGER->resume("rightAtk");
				break;
			case ENEMY_UP:
				ANIMATIONMANAGER->addAnimation("upAtk", "��ŹATK", 4, 7, 5, false, false);
				enemyAni = ANIMATIONMANAGER->findAnimation("upAtk");
				ANIMATIONMANAGER->resume("upAtk");
				break;
			case ENEMY_DOWN:
				ANIMATIONMANAGER->addAnimation("downAtk", "��ŹATK", 0, 3, 5, false, false);
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
				ANIMATIONMANAGER->addAnimation("enemyLeft", "��Ź", 4, 5, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyLeft");
				ANIMATIONMANAGER->resume("enemyLeft");
				break;
			case ENEMY_RIGHT:
				ANIMATIONMANAGER->addAnimation("enemyRight", "��Ź", 6, 7, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyRight");
				ANIMATIONMANAGER->resume("enemyRight");
				break;
			case ENEMY_UP:
				ANIMATIONMANAGER->addAnimation("enemyUp", "��Ź", 2, 3, 2, false, true);
				enemyAni = ANIMATIONMANAGER->findAnimation("enemyUp");
				ANIMATIONMANAGER->resume("enemyUp");
				break;
			case ENEMY_DOWN:
				ANIMATIONMANAGER->addAnimation("enemyDown", "��Ź", 0, 1, 2, false, true);
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

void Dongtak::enemyState()
{
	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);
}

void Dongtak::enemyMenu()
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

void Dongtak::enemyCollision()
{

}

void Dongtak::setPosition(RECT rc)
{
	dongtak.rc = rc;
	enemyX = dongtak.rc.left + (dongtak.rc.right - dongtak.rc.left) / 2;
	enemyY = dongtak.rc.top + (dongtak.rc.bottom - dongtak.rc.top) / 2;
}
