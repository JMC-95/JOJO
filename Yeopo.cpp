#include "stdafx.h"
#include "Yeopo.h"

Yeopo::Yeopo()
{
}

Yeopo::~Yeopo()
{
}

HRESULT Yeopo::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * enemyImg, const char * atkImg, const char * blockImg)
{
	//����ü ���� ����
	EnemyInfo yeopo;
	//�̹��� �� �ִϸ��̼�
	yeopo.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//ĳ���� Ŭ���� �̵����� �̹���
	yeopo.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);		//ĳ���� Ŭ���� ���ݹ��� �̹���
	yeopo.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//���ݹ�ư Ŭ���� ���ݹ��� �̹���
	yeopo.img = IMAGEMANAGER->findImage(enemyImg);			//ĳ���� �̹���
	yeopo.atkImg = IMAGEMANAGER->findImage(atkImg);			//���� �̹���
	yeopo.blockImg = IMAGEMANAGER->findImage(blockImg);		//��� �� �ǰ� �̹���
	//�������ͽ�
	yeopo.level = 6;			//����
	yeopo.hp = 180;				//ü��
	yeopo.mp = 10;				//����
	yeopo.atk = 70;				//���ݷ�
	yeopo.will = 22;			//���ŷ�
	yeopo.def = 54;				//����
	yeopo.agi = 65;				//���߷�
	yeopo.ten = 65;				//���
	yeopo.movingCount = 6;		//�̵���
	vYeopo.push_back(yeopo);

	//HP ProgressBar
	_Hp = new progressBar;
	_Hp->init("images/UI/Info/HP.bmp", "images/UI/Info/Back_P.bmp", 1056, 289, 84, 12);
	_Hp->setGauge(currentHp, maxHp);
	currentHp = maxHp = 180;
	damage = 0;

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

	isTurn = true;
	isSelect = false;

	return S_OK;
}

void Yeopo::release()
{
	SAFE_DELETE(_Hp);
	SAFE_DELETE(_Mp);
	SAFE_DELETE(_Exp);
}

void Yeopo::update()
{
	if (isTurn)
	{
		//mouseMove();
	}

	enemyAnimation();
	enemyState();
}

void Yeopo::render(HDC hdc)
{
	for (int k = 0; k < vYeopo.size(); k++)
	{
		if (isTurn)
		{
			vYeopo[k].img->aniRender(hdc, vYeopo[k].rc.left, vYeopo[k].rc.top, enemyAni);
		}
		else
		{
			vYeopo[k].img->frameAlphaRender(hdc, vYeopo[k].rc.left, vYeopo[k].rc.top, 0, frameY, 100);
		}
	}
}

void Yeopo::mouseMove()
{
	for (int k = 0; k < vYeopo.size(); k++)
	{
		if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
		{
			for (int i = 0; i < TILE_X * TILE_Y; i++)
			{
				if (PtInRect(&vYeopo[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
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
						vYeopo[k].rcAtk[0] = RectMake(vYeopo[k].rc.left - 48, vYeopo[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
						vYeopo[k].rcAtk[1] = RectMake(vYeopo[k].rc.left - 48, vYeopo[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
						vYeopo[k].rcAtk[2] = RectMake(vYeopo[k].rc.left - 48, vYeopo[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
						vYeopo[k].rcAtk[3] = RectMake(vYeopo[k].rc.left + 48, vYeopo[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
						vYeopo[k].rcAtk[4] = RectMake(vYeopo[k].rc.left + 48, vYeopo[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
						vYeopo[k].rcAtk[5] = RectMake(vYeopo[k].rc.left + 48, vYeopo[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
						vYeopo[k].rcAtk[6] = RectMake(vYeopo[k].rc.left, vYeopo[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
						vYeopo[k].rcAtk[7] = RectMake(vYeopo[k].rc.left, vYeopo[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
						atkList.push_back(vYeopo[k].rcAtk[j]);
					}

					//�̵�����
					if (!isStop)
					{
						floodFill(startTile, vYeopo[k].movingCount);
					}
				}

				if (!PtInRect(&vYeopo[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
				{
					//������ ���� x��ǥ�� y��ǥ
					mapX = mainMap->getMap()[i].rc.left + (mainMap->getMap()[i].rc.right - mainMap->getMap()[i].rc.left) / 2;
					mapY = mainMap->getMap()[i].rc.top + (mainMap->getMap()[i].rc.bottom - mainMap->getMap()[i].rc.top) / 2;

					if (mainMap->getMap()[i].flood)
					{
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
					vYeopo[k].rcAtk[0] = RectMake(vYeopo[k].rc.left - 48, vYeopo[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
					vYeopo[k].rcAtk[1] = RectMake(vYeopo[k].rc.left - 48, vYeopo[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					vYeopo[k].rcAtk[2] = RectMake(vYeopo[k].rc.left - 48, vYeopo[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					vYeopo[k].rcAtk[3] = RectMake(vYeopo[k].rc.left + 48, vYeopo[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
					vYeopo[k].rcAtk[4] = RectMake(vYeopo[k].rc.left + 48, vYeopo[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					vYeopo[k].rcAtk[5] = RectMake(vYeopo[k].rc.left + 48, vYeopo[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					vYeopo[k].rcAtk[6] = RectMake(vYeopo[k].rc.left, vYeopo[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					vYeopo[k].rcAtk[7] = RectMake(vYeopo[k].rc.left, vYeopo[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(vYeopo[k].rcAtk[j]);
				}

				//�޴����� ��Ʈ
				for (int j = 0; j < 5; j++)
				{
					rcMenu[0] = RectMake(vYeopo[k].rc.left - 97, vYeopo[k].rc.top - 30, 82, 20);
					rcMenu[1] = RectMake(vYeopo[k].rc.left - 97, vYeopo[k].rc.top - 9, 82, 20);
					rcMenu[2] = RectMake(vYeopo[k].rc.left - 97, vYeopo[k].rc.top + 12, 82, 20);
					rcMenu[3] = RectMake(vYeopo[k].rc.left - 97, vYeopo[k].rc.top + 38, 82, 20);
					rcMenu[4] = RectMake(vYeopo[k].rc.left - 97, vYeopo[k].rc.top + 63, 82, 20);
					menuList.push_back(rcMenu[j]);
				}
			}
		}

		if (isClick)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
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
					isClick = false;
				}
				if (PtInRect(&rcMenu[4], m_ptMouse))	//���
				{
					atkList.clear();
					menuList.clear();

					isStop = false;
					isClick = false;
				}
			}
		}
	}
}

void Yeopo::enemyMove()
{
	for (int k = 0; k < vYeopo.size(); k++)
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

		if (vYeopo[k].rc.left > 0 || vYeopo[k].rc.right < WINSIZEY ||
			vYeopo[k].rc.top > 0 || vYeopo[k].rc.bottom < WINSIZEY)
		{
			switch (eDirection)
			{
			case ENEMY_LEFT:
				enemyX -= speed;
				vYeopo[k].rc = RectMakeCenter(enemyX, enemyY, vYeopo[k].img->getFrameWidth(), vYeopo[k].img->getFrameHeight());
				break;
			case ENEMY_RIGHT:
				enemyX += speed;
				vYeopo[k].rc = RectMakeCenter(enemyX, enemyY, vYeopo[k].img->getFrameWidth(), vYeopo[k].img->getFrameHeight());
				break;
			case ENEMY_UP:
				enemyY -= speed;
				vYeopo[k].rc = RectMakeCenter(enemyX, enemyY, vYeopo[k].img->getFrameWidth(), vYeopo[k].img->getFrameHeight());
				break;
			case ENEMY_DOWN:
				enemyY += speed;
				vYeopo[k].rc = RectMakeCenter(enemyX, enemyY, vYeopo[k].img->getFrameWidth(), vYeopo[k].img->getFrameHeight());
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

void Yeopo::enemyAnimation()
{
	if (isTurn)
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

void Yeopo::enemyState()
{
	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);
}

void Yeopo::setPosition(RECT rc)
{
	for (int k = 0; k < vYeopo.size(); k++)
	{
		vYeopo[k].rc = rc;
		enemyX = vYeopo[k].rc.left + (vYeopo[k].rc.right - vYeopo[k].rc.left) / 2;
		enemyY = vYeopo[k].rc.top + (vYeopo[k].rc.bottom - vYeopo[k].rc.top) / 2;
	}
}
