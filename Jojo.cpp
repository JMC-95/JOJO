#include "stdafx.h"
#include "Jojo.h"

Jojo::Jojo()
{
}

Jojo::~Jojo()
{
}

HRESULT Jojo::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * playerImg, const char * atkImg, const char * blockImg)
{
	//����ü ���� ����
	PlayerInfo jojo;
	//�̹��� �� �ִϸ��̼�
	jojo.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//ĳ���� Ŭ���� �̵����� �̹���
	jojo.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);		//ĳ���� Ŭ���� ���ݹ��� �̹���
	jojo.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//���ݹ�ư Ŭ���� ���ݹ��� �̹���
	jojo.img = IMAGEMANAGER->findImage(playerImg);			//ĳ���� �̹���
	jojo.atkImg = IMAGEMANAGER->findImage(atkImg);			//���� �̹���
	jojo.blockImg = IMAGEMANAGER->findImage(blockImg);		//��� �� �ǰ� �̹���
	//�������ͽ�
	jojo.level = 6;				//����
	jojo.hp = 132;				//ü��
	jojo.mp = 36;				//����
	jojo.atk = 59;				//���ݷ�
	jojo.will = 64;				//���ŷ�
	jojo.def = 67;				//����
	jojo.agi = 58;				//���߷�
	jojo.ten = 60;				//���
	jojo.movingCount = 6;		//�̵���
	vJojo.push_back(jojo);

	//HP ProgressBar
	_Hp = new progressBar;
	_Hp->init("images/UI/Info/HP.bmp", "images/UI/Info/Back_P.bmp", 1056, 289, 84, 12);
	_Hp->setGauge(currentHp, maxHp);
	currentHp = maxHp = 132;
	damage = 0;

	//MP ProgressBar
	_Mp = new progressBar;
	_Mp->init("images/UI/Info/MP.bmp", "images/UI/Info/Back_P.bmp", 1056, 307, 84, 12);
	_Mp->setGauge(currentMp, maxMp);
	currentMp = maxMp = 36;

	//EXP ProgressBar
	_Exp = new progressBar;
	_Exp->init("images/UI/Info/EXP.bmp", "images/UI/Info/Back_EXP.bmp", 1095, 243, 45, 12);
	_Exp->setGauge(currentExp, maxExp);
	currentExp = 11;
	maxExp = 100;

	//ĳ���� ���� �� ��ġ
	pDirection = PLAYER_LEFT;
	startTile = endTile = -1;
	speed = 6;	//�ӵ�

	isTurn = true;
	isSelect = false;

	return S_OK;
}

void Jojo::release()
{
	SAFE_DELETE(_Hp);
	SAFE_DELETE(_Mp);
	SAFE_DELETE(_Exp);
}

void Jojo::update()
{
	if (isTurn)
	{
		mouseMove();
	}

	playerAnimation();
	playerState();

	if (KEYMANAGER->isOnceKeyDown('1'))
	{
		isTurn = true;
		isSelect = false;
	}
}

void Jojo::render(HDC hdc)
{
	for (int k = 0; k < vJojo.size(); k++)
	{
		if (isTurn)
		{
			if (isAtk)
				vJojo[k].atkImg->aniRender(hdc, vJojo[k].rc.left - 8, vJojo[k].rc.top - 8, playerAni);
			else
				vJojo[k].img->aniRender(hdc, vJojo[k].rc.left, vJojo[k].rc.top, playerAni);
		}
		else
			vJojo[k].img->frameAlphaRender(hdc, vJojo[k].rc.left, vJojo[k].rc.top, 0, frameY, 100);
	}
}

void Jojo::mouseMove()
{
	for (int k = 0; k < vJojo.size(); k++)
	{
		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			if (PtInRect(&vJojo[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
			{
				if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
				{
					//������ Ÿ�� (ĳ����)
					saveTile = startTile;
					startTile = i;
					isSelect = true;
					isFind = false;
					noPath = false;
					startAstar = false;

					//���ݹ���
					for (int j = 0; j < 4; j++)
					{
						vJojo[k].rcAtk[0] = RectMake(vJojo[k].rc.left - 48, vJojo[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
						vJojo[k].rcAtk[1] = RectMake(vJojo[k].rc.left + 48, vJojo[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
						vJojo[k].rcAtk[2] = RectMake(vJojo[k].rc.left, vJojo[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
						vJojo[k].rcAtk[3] = RectMake(vJojo[k].rc.left, vJojo[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
						atkList.push_back(vJojo[k].rcAtk[j]);
					}

					//�̵�����
					if (!isStop)
					{
						floodFill(startTile, vJojo[k].movingCount);
					}
				}
			}

			if (!PtInRect(&vJojo[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
			{
				if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
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
		//}

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
				for (int j = 0; j < 4; j++)
				{
					vJojo[k].rcAtk[0] = RectMake(vJojo[k].rc.left - 48, vJojo[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
					vJojo[k].rcAtk[1] = RectMake(vJojo[k].rc.left + 48, vJojo[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
					vJojo[k].rcAtk[2] = RectMake(vJojo[k].rc.left, vJojo[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					vJojo[k].rcAtk[3] = RectMake(vJojo[k].rc.left, vJojo[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(vJojo[k].rcAtk[j]);
				}

				//�޴����� ��Ʈ
				for (int j = 0; j < 5; j++)
				{
					rcMenu[0] = RectMake(vJojo[k].rc.left - 97, vJojo[k].rc.top - 30, 82, 20);
					rcMenu[1] = RectMake(vJojo[k].rc.left - 97, vJojo[k].rc.top - 9, 82, 20);
					rcMenu[2] = RectMake(vJojo[k].rc.left - 97, vJojo[k].rc.top + 12, 82, 20);
					rcMenu[3] = RectMake(vJojo[k].rc.left - 97, vJojo[k].rc.top + 38, 82, 20);
					rcMenu[4] = RectMake(vJojo[k].rc.left - 97, vJojo[k].rc.top + 63, 82, 20);
					menuList.push_back(rcMenu[j]);
				}
			}
		}

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

					vJojo[k].rc.left = mainMap->getMap()[saveTile].rc.left;
					vJojo[k].rc.right = mainMap->getMap()[saveTile].rc.right;
					vJojo[k].rc.top = mainMap->getMap()[saveTile].rc.top;
					vJojo[k].rc.bottom = mainMap->getMap()[saveTile].rc.bottom;
					pDirection = sDirection;

					isSelect = false;
					isStop = false;
					isClick = false;
				}
			}
		}

		collision();
	}
}

void Jojo::playerMove()
{
	for (int k = 0; k < vJojo.size(); k++)
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

		if (vJojo[k].rc.left > 0 || vJojo[k].rc.right < WINSIZEY ||
			vJojo[k].rc.top > 0 || vJojo[k].rc.bottom < WINSIZEY)
		{
			switch (pDirection)
			{
			case PLAYER_LEFT:
				playerX -= speed;
				vJojo[k].rc = RectMakeCenter(playerX, playerY, vJojo[k].img->getFrameWidth(), vJojo[k].img->getFrameHeight());
				break;
			case PLAYER_RIGHT:
				playerX += speed;
				vJojo[k].rc = RectMakeCenter(playerX, playerY, vJojo[k].img->getFrameWidth(), vJojo[k].img->getFrameHeight());
				break;
			case PLAYER_UP:
				playerY -= speed;
				vJojo[k].rc = RectMakeCenter(playerX, playerY, vJojo[k].img->getFrameWidth(), vJojo[k].img->getFrameHeight());
				break;
			case PLAYER_DOWN:
				playerY += speed;
				vJojo[k].rc = RectMakeCenter(playerX, playerY, vJojo[k].img->getFrameWidth(), vJojo[k].img->getFrameHeight());
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

void Jojo::playerAnimation()
{
	if (isTurn)
	{
		if (isAtk)
		{
			switch (pDirection)
			{
			case PLAYER_LEFT:
				ANIMATIONMANAGER->addAnimation("leftAtk", "����ATK", 12, 15, 6, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("leftAtk");
				ANIMATIONMANAGER->resume("leftAtk");
				break;
			case PLAYER_RIGHT:
				ANIMATIONMANAGER->addAnimation("rightAtk", "����ATK", 8, 11, 6, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("rightAtk");
				ANIMATIONMANAGER->resume("rightAtk");
				break;
			case PLAYER_UP:
				ANIMATIONMANAGER->addAnimation("upAtk", "����ATK", 4, 7, 6, false, false);
				playerAni = ANIMATIONMANAGER->findAnimation("upAtk");
				ANIMATIONMANAGER->resume("upAtk");
				break;
			case PLAYER_DOWN:
				ANIMATIONMANAGER->addAnimation("downAtk", "����ATK", 0, 3, 6, false, false);
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

void Jojo::playerState()
{
	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);
}

void Jojo::collision()
{
	for (int k = 0; k < vJojo.size(); k++)
	{
		RECT temp;

		if (IntersectRect(&temp, &vJojo[k].rcAtk[0], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
			IntersectRect(&temp, &vJojo[k].rcAtk[1], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
			IntersectRect(&temp, &vJojo[k].rcAtk[2], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
			IntersectRect(&temp, &vJojo[k].rcAtk[3], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc))
		{
			isTarget = true;
			frameX = 1;

			if (PtInRect(&ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc, m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
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

void Jojo::setPosition(RECT rc)
{
	for (int k = 0; k < vJojo.size(); k++)
	{
		vJojo[k].rc = rc;
		playerX = vJojo[k].rc.left + (vJojo[k].rc.right - vJojo[k].rc.left) / 2;
		playerY = vJojo[k].rc.top + (vJojo[k].rc.bottom - vJojo[k].rc.top) / 2;
	}
}
