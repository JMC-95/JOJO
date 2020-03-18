#include "stdafx.h"
#include "Hahuyeon.h"

Hahuyeon::Hahuyeon()
{
}

Hahuyeon::~Hahuyeon()
{
}

HRESULT Hahuyeon::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * playerImg, const char * atkImg, const char * blockImg)
{
	//����ü ���� ����
	PlayerInfo Hahuyeon;
	//�̹��� �� �ִϸ��̼�
	Hahuyeon.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//ĳ���� Ŭ���� �̵����� �̹���
	Hahuyeon.mAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);		//ĳ���� Ŭ���� ���ݹ��� �̹���
	Hahuyeon.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//���ݹ�ư Ŭ���� ���ݹ��� �̹���
	Hahuyeon.img = IMAGEMANAGER->findImage(playerImg);			//ĳ���� �̹���
	Hahuyeon.atkImg = IMAGEMANAGER->findImage(atkImg);			//���� �̹���
	Hahuyeon.blockImg = IMAGEMANAGER->findImage(blockImg);		//��� �� �ǰ� �̹���
	ANIMATIONMANAGER->addAnimation("playerLeft", "���Ŀ�", 4, 5, 2, false, true);
	playerAni = ANIMATIONMANAGER->findAnimation("playerLeft");
	//�������ͽ�
	Hahuyeon.speed = 6;			//�ӵ�
	Hahuyeon.movingCount = 6;	//�̵�����
	vHahuyeon.push_back(Hahuyeon);

	pDirection = PLAYER_LEFT;

	count = 2;
	startTile = endTile = -1;	//A*

	isTurn = true;
	isSelect = false;

	return S_OK;
}

void Hahuyeon::release()
{
}

void Hahuyeon::update()
{
	if (isTurn)
	{
		mouseMove();
	}

	playerAnimation();
}

void Hahuyeon::render(HDC hdc)
{
	for (int k = 0; k < vHahuyeon.size(); k++)
	{
		if (isTurn)
		{
			vHahuyeon[k].img->aniRender(hdc, vHahuyeon[k].rc.left, vHahuyeon[k].rc.top, playerAni);
		}
		else
		{
			vHahuyeon[k].img->frameAlphaRender(hdc, vHahuyeon[k].rc.left, vHahuyeon[k].rc.top, 0, frameNumY, 100);
		}

		if (isClick)
		{
			IMAGEMANAGER->frameRender("�޴�", hdc, vHahuyeon[k].rc.left - 100, vHahuyeon[k].rc.top - 35, frameNumX, 0);
		}
	}
}

void Hahuyeon::mouseMove()
{
	for (int k = 0; k < vHahuyeon.size(); k++)
	{
		if (!PLAYERMANAGER->getAgjin()->getIsSelect() && !PLAYERMANAGER->getHahudon()->getIsSelect() &&
			!PLAYERMANAGER->getIjeon()->getIsSelect() && !PLAYERMANAGER->getJohong()->getIsSelect() &&
			!PLAYERMANAGER->getJoin()->getIsSelect() && !PLAYERMANAGER->getJojo()->getIsSelect() &&
			KEYMANAGER->isStayKeyDown(VK_LBUTTON))
		{
			for (int i = 0; i < TILE_X * TILE_Y; i++)
			{
				if (PtInRect(&vHahuyeon[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
				{
					//������ Ÿ�� (ĳ����)
					startTile = i;
					count -= 1;

					isSelect = true;
					isFind = false;
					noPath = false;
					startAstar = false;

					//���ݹ���
					for (int j = 0; j < 4; j++)
					{
						vHahuyeon[k].rcAtk[0] = RectMake(vHahuyeon[k].rc.left - 96, vHahuyeon[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
						vHahuyeon[k].rcAtk[1] = RectMake(vHahuyeon[k].rc.left + 96, vHahuyeon[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
						vHahuyeon[k].rcAtk[2] = RectMake(vHahuyeon[k].rc.left, vHahuyeon[k].rc.top - 96, TILE_WIDTH, TILE_HEIGHT);
						vHahuyeon[k].rcAtk[3] = RectMake(vHahuyeon[k].rc.left, vHahuyeon[k].rc.top + 96, TILE_WIDTH, TILE_HEIGHT);
						atkList.push_back(vHahuyeon[k].rcAtk[j]);
					}

					//�̵�����
					if (!isCancel)
					{
						floodFill(startTile, vHahuyeon[k].movingCount);
					}
				}

				if (!PtInRect(&vHahuyeon[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
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
				for (int j = 0; j < 4; j++)
				{
					vHahuyeon[k].rcAtk[0] = RectMake(vHahuyeon[k].rc.left - 96, vHahuyeon[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
					vHahuyeon[k].rcAtk[1] = RectMake(vHahuyeon[k].rc.left + 96, vHahuyeon[k].rc.top, TILE_WIDTH, TILE_HEIGHT);
					vHahuyeon[k].rcAtk[2] = RectMake(vHahuyeon[k].rc.left, vHahuyeon[k].rc.top - 96, TILE_WIDTH, TILE_HEIGHT);
					vHahuyeon[k].rcAtk[3] = RectMake(vHahuyeon[k].rc.left, vHahuyeon[k].rc.top + 96, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(vHahuyeon[k].rcAtk[j]);
				}

				//�޴����� ��Ʈ
				for (int j = 0; j < 5; j++)
				{
					rcMenu[0] = RectMake(vHahuyeon[k].rc.left - 97, vHahuyeon[k].rc.top - 30, 82, 20);
					rcMenu[1] = RectMake(vHahuyeon[k].rc.left - 97, vHahuyeon[k].rc.top - 9, 82, 20);
					rcMenu[2] = RectMake(vHahuyeon[k].rc.left - 97, vHahuyeon[k].rc.top + 12, 82, 20);
					rcMenu[3] = RectMake(vHahuyeon[k].rc.left - 97, vHahuyeon[k].rc.top + 38, 82, 20);
					rcMenu[4] = RectMake(vHahuyeon[k].rc.left - 97, vHahuyeon[k].rc.top + 63, 82, 20);
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

		if (IntersectRect(&temp, &vHahuyeon[k].rcAtk[0], &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vHahuyeon[k].rcAtk[1], &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vHahuyeon[k].rcAtk[2], &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc) ||
			IntersectRect(&temp, &vHahuyeon[k].rcAtk[3], &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc))
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

void Hahuyeon::playerMove()
{
	for (int k = 0; k < vHahuyeon.size(); k++)
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

		if (vHahuyeon[k].rc.left > 0 || vHahuyeon[k].rc.right < WINSIZEY ||
			vHahuyeon[k].rc.top > 0 || vHahuyeon[k].rc.bottom < WINSIZEY)
		{
			switch (pDirection)
			{
			case PLAYER_LEFT:
				playerX -= vHahuyeon[k].speed;
				vHahuyeon[k].rc = RectMakeCenter(playerX, playerY, vHahuyeon[k].img->getFrameWidth(), vHahuyeon[k].img->getFrameHeight());
				break;
			case PLAYER_RIGHT:
				playerX += vHahuyeon[k].speed;
				vHahuyeon[k].rc = RectMakeCenter(playerX, playerY, vHahuyeon[k].img->getFrameWidth(), vHahuyeon[k].img->getFrameHeight());
				break;
			case PLAYER_UP:
				playerY -= vHahuyeon[k].speed;
				vHahuyeon[k].rc = RectMakeCenter(playerX, playerY, vHahuyeon[k].img->getFrameWidth(), vHahuyeon[k].img->getFrameHeight());
				break;
			case PLAYER_DOWN:
				playerY += vHahuyeon[k].speed;
				vHahuyeon[k].rc = RectMakeCenter(playerX, playerY, vHahuyeon[k].img->getFrameWidth(), vHahuyeon[k].img->getFrameHeight());
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

void Hahuyeon::playerAnimation()
{
	if (isTurn)
	{
		switch (pDirection)
		{
		case PLAYER_LEFT:
			ANIMATIONMANAGER->addAnimation("playerLeft", "���Ŀ�", 4, 5, 2, false, true);
			playerAni = ANIMATIONMANAGER->findAnimation("playerLeft");
			ANIMATIONMANAGER->resume("playerLeft");
			break;
		case PLAYER_RIGHT:
			ANIMATIONMANAGER->addAnimation("playerRight", "���Ŀ�", 6, 7, 2, false, true);
			playerAni = ANIMATIONMANAGER->findAnimation("playerRight");
			ANIMATIONMANAGER->resume("playerRight");
			break;
		case PLAYER_UP:
			ANIMATIONMANAGER->addAnimation("playerUp", "���Ŀ�", 2, 3, 2, false, true);
			playerAni = ANIMATIONMANAGER->findAnimation("playerUp");
			ANIMATIONMANAGER->resume("playerUp");
			break;
		case PLAYER_DOWN:
			ANIMATIONMANAGER->addAnimation("playerDown", "���Ŀ�", 0, 1, 2, false, true);
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

void Hahuyeon::setPosition(RECT rc)
{
	for (int k = 0; k < vHahuyeon.size(); k++)
	{
		vHahuyeon[k].rc = rc;
		playerX = vHahuyeon[k].rc.left + (vHahuyeon[k].rc.right - vHahuyeon[k].rc.left) / 2;
		playerY = vHahuyeon[k].rc.top + (vHahuyeon[k].rc.bottom - vHahuyeon[k].rc.top) / 2;
	}
}
