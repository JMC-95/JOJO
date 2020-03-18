#include "stdafx.h"
#include "Johong.h"

Johong::Johong()
{
}

Johong::~Johong()
{
}

HRESULT Johong::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * playerImg, const char * atkImg, const char * blockImg)
{
	//����ü ���� ����
	PlayerInfo Johong;
	//�̹��� �� �ִϸ��̼�
	Johong.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//ĳ���� Ŭ���� �̵����� �̹���
	Johong.mAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);		//ĳ���� Ŭ���� ���ݹ��� �̹���
	Johong.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//���ݹ�ư Ŭ���� ���ݹ��� �̹���
	Johong.img = IMAGEMANAGER->findImage(playerImg);			//ĳ���� �̹���
	Johong.atkImg = IMAGEMANAGER->findImage(atkImg);			//���� �̹���
	Johong.blockImg = IMAGEMANAGER->findImage(blockImg);		//��� �� �ǰ� �̹���
	ANIMATIONMANAGER->addAnimation("playerLeft", "��ȫ", 4, 5, 2, false, true);
	playerAni = ANIMATIONMANAGER->findAnimation("playerLeft");
	//�������ͽ�
	Johong.speed = 6;			//�ӵ�
	Johong.movingCount = 4;		//�̵�����
	vJohong.push_back(Johong);

	pDirection = PLAYER_LEFT;

	count = 2;
	startTile = endTile = -1;	//A*

	isTurn = true;
	isSelect = false;

	return S_OK;
}

void Johong::release()
{
}

void Johong::update()
{
	if (isTurn)
	{
		mouseMove();
	}

	playerAnimation();
}

void Johong::render(HDC hdc)
{
	for (int k = 0; k < vJohong.size(); k++)
	{
		if (isTurn)
		{
			vJohong[k].img->aniRender(hdc, vJohong[k].rc.left, vJohong[k].rc.top, playerAni);
		}
		else
		{
			vJohong[k].img->frameAlphaRender(hdc, vJohong[k].rc.left, vJohong[k].rc.top, 0, frameNumY, 100);
		}

		if (isClick)
		{
			IMAGEMANAGER->frameRender("�޴�", hdc, vJohong[k].rc.left - 100, vJohong[k].rc.top - 35, frameNumX, 0);
		}
	}
}

void Johong::mouseMove()
{
	for (int k = 0; k < vJohong.size(); k++)
	{
		if (!PLAYERMANAGER->getAgjin()->getIsSelect() && !PLAYERMANAGER->getHahudon()->getIsSelect() &&
			!PLAYERMANAGER->getHahuyeon()->getIsSelect() && !PLAYERMANAGER->getIjeon()->getIsSelect() &&
			!PLAYERMANAGER->getJoin()->getIsSelect() && !PLAYERMANAGER->getJojo()->getIsSelect() &&
			KEYMANAGER->isStayKeyDown(VK_LBUTTON))
		{
			for (int i = 0; i < TILE_X * TILE_Y; i++)
			{
				if (PtInRect(&vJohong[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
				{
					//������ Ÿ�� (ĳ����)
					startTile = i;
					count -= 1;

					isSelect = true;
					isFind = false;
					noPath = false;
					startAstar = false;

					//���ݹ���
					for (int j = 0; j < 8; j++)
					{
						vJohong[k].rcAtk[0] = RectMake(vJohong[k].rc.left - 48, vJohong[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
						vJohong[k].rcAtk[1] = RectMake(vJohong[k].rc.left - 48, vJohong[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
						vJohong[k].rcAtk[2] = RectMake(vJohong[k].rc.left - 48, vJohong[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
						vJohong[k].rcAtk[3] = RectMake(vJohong[k].rc.left + 48, vJohong[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
						vJohong[k].rcAtk[4] = RectMake(vJohong[k].rc.left + 48, vJohong[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
						vJohong[k].rcAtk[5] = RectMake(vJohong[k].rc.left + 48, vJohong[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
						vJohong[k].rcAtk[6] = RectMake(vJohong[k].rc.left, vJohong[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
						vJohong[k].rcAtk[7] = RectMake(vJohong[k].rc.left, vJohong[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
						atkList.push_back(vJohong[k].rcAtk[j]);
					}

					//�̵�����
					if (!isCancel)
					{
						floodFill(startTile, vJohong[k].movingCount);
					}
				}

				if (!PtInRect(&vJohong[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
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
			if (!isCancel)
			{
				playerMove();
			}

			if (playerX == mapX && playerY == mapY)
			{
				isCancel = true;
				isClick = true;

				//���ݹ���
				for (int j = 0; j < 8; j++)
				{
					vJohong[k].rcAtk[0] = RectMake(vJohong[k].rc.left - 48, vJohong[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
					vJohong[k].rcAtk[1] = RectMake(vJohong[k].rc.left - 48, vJohong[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					vJohong[k].rcAtk[2] = RectMake(vJohong[k].rc.left - 48, vJohong[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					vJohong[k].rcAtk[3] = RectMake(vJohong[k].rc.left + 48, vJohong[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
					vJohong[k].rcAtk[4] = RectMake(vJohong[k].rc.left + 48, vJohong[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					vJohong[k].rcAtk[5] = RectMake(vJohong[k].rc.left + 48, vJohong[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					vJohong[k].rcAtk[6] = RectMake(vJohong[k].rc.left, vJohong[k].rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					vJohong[k].rcAtk[7] = RectMake(vJohong[k].rc.left, vJohong[k].rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(vJohong[k].rcAtk[j]);
				}

				//�޴����� ��Ʈ
				for (int j = 0; j < 5; j++)
				{
					rcMenu[0] = RectMake(vJohong[k].rc.left - 97, vJohong[k].rc.top - 30, 82, 20);
					rcMenu[1] = RectMake(vJohong[k].rc.left - 97, vJohong[k].rc.top - 9, 82, 20);
					rcMenu[2] = RectMake(vJohong[k].rc.left - 97, vJohong[k].rc.top + 12, 82, 20);
					rcMenu[3] = RectMake(vJohong[k].rc.left - 97, vJohong[k].rc.top + 38, 82, 20);
					rcMenu[4] = RectMake(vJohong[k].rc.left - 97, vJohong[k].rc.top + 63, 82, 20);
					menuList.push_back(rcMenu[j]);
				}
			}
		}

		if (isClick)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				if (PtInRect(&rcMenu[0], m_ptMouse) && isAtk)	//����
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

					isClick = false;
				}
				if (PtInRect(&rcMenu[2], m_ptMouse))	//����
				{
					atkList.clear();
					menuList.clear();

					isClick = false;
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

					isCancel = false;
					isClick = false;
				}
			}
		}

		RECT temp;

		if (IntersectRect(&temp, &vJohong[k].rcAtk[0], &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vJohong[k].rcAtk[1], &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vJohong[k].rcAtk[2], &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vJohong[k].rcAtk[3], &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc))
		{
			isAtk = true;
			frameNumX = 1;
		}
		else
		{
			frameNumX = 0;
		}
	}
}

void Johong::playerMove()
{
	for (int k = 0; k < vJohong.size(); k++)
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

		if (vJohong[k].rc.left > 0 || vJohong[k].rc.right < WINSIZEY ||
			vJohong[k].rc.top > 0 || vJohong[k].rc.bottom < WINSIZEY)
		{
			switch (pDirection)
			{
			case PLAYER_LEFT:
				playerX -= vJohong[k].speed;
				vJohong[k].rc = RectMakeCenter(playerX, playerY, vJohong[k].img->getFrameWidth(), vJohong[k].img->getFrameHeight());
				break;
			case PLAYER_RIGHT:
				playerX += vJohong[k].speed;
				vJohong[k].rc = RectMakeCenter(playerX, playerY, vJohong[k].img->getFrameWidth(), vJohong[k].img->getFrameHeight());
				break;
			case PLAYER_UP:
				playerY -= vJohong[k].speed;
				vJohong[k].rc = RectMakeCenter(playerX, playerY, vJohong[k].img->getFrameWidth(), vJohong[k].img->getFrameHeight());
				break;
			case PLAYER_DOWN:
				playerY += vJohong[k].speed;
				vJohong[k].rc = RectMakeCenter(playerX, playerY, vJohong[k].img->getFrameWidth(), vJohong[k].img->getFrameHeight());
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

void Johong::playerAnimation()
{
	if (isTurn)
	{
		switch (pDirection)
		{
		case PLAYER_LEFT:
			ANIMATIONMANAGER->addAnimation("playerLeft", "��ȫ", 4, 5, 2, false, true);
			playerAni = ANIMATIONMANAGER->findAnimation("playerLeft");
			ANIMATIONMANAGER->resume("playerLeft");
			break;
		case PLAYER_RIGHT:
			ANIMATIONMANAGER->addAnimation("playerRight", "��ȫ", 6, 7, 2, false, true);
			playerAni = ANIMATIONMANAGER->findAnimation("playerRight");
			ANIMATIONMANAGER->resume("playerRight");
			break;
		case PLAYER_UP:
			ANIMATIONMANAGER->addAnimation("playerUp", "��ȫ", 2, 3, 2, false, true);
			playerAni = ANIMATIONMANAGER->findAnimation("playerUp");
			ANIMATIONMANAGER->resume("playerUp");
			break;
		case PLAYER_DOWN:
			ANIMATIONMANAGER->addAnimation("playerDown", "��ȫ", 0, 1, 2, false, true);
			playerAni = ANIMATIONMANAGER->findAnimation("playerDown");
			ANIMATIONMANAGER->resume("playerDown");
			break;
		}
	}
	else
	{
		switch (pDirection)
		{
		case PLAYER_LEFT:
			frameNumY = 10;
			break;
		case PLAYER_RIGHT:
			frameNumY = 11;
			break;
		case PLAYER_UP:
			frameNumY = 9;
			break;
		case PLAYER_DOWN:
			frameNumY = 8;
			break;
		}
	}
}

void Johong::setPosition(RECT rc)
{
	for (int k = 0; k < vJohong.size(); k++)
	{
		vJohong[k].rc = rc;
		playerX = vJohong[k].rc.left + (vJohong[k].rc.right - vJohong[k].rc.left) / 2;
		playerY = vJohong[k].rc.top + (vJohong[k].rc.bottom - vJohong[k].rc.top) / 2;
	}
}
