#include "stdafx.h"
#include "Hahudon.h"

Hahudon::Hahudon()
{
}

Hahudon::~Hahudon()
{
}

HRESULT Hahudon::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * playerImg, const char * atkImg, const char * blockImg)
{
	//����ü ���� ����
	PlayerInfo hahudon;
	//�̹��� �� �ִϸ��̼�
	hahudon.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//ĳ���� Ŭ���� �̵����� �̹���
	hahudon.mAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);		//ĳ���� Ŭ���� ���ݹ��� �̹���
	hahudon.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//���ݹ�ư Ŭ���� ���ݹ��� �̹���
	hahudon.img = IMAGEMANAGER->findImage(playerImg);			//ĳ���� �̹���
	hahudon.atkImg = IMAGEMANAGER->findImage(atkImg);			//���� �̹���
	hahudon.blockImg = IMAGEMANAGER->findImage(blockImg);		//��� �� �ǰ� �̹���
	ANIMATIONMANAGER->addAnimation("playerLeft", "���ĵ�", 4, 5, 2, false, true);
	playerAni = ANIMATIONMANAGER->findAnimation("playerLeft");
	//�������ͽ�
	hahudon.speed = 6;			//�ӵ�
	hahudon.movingCount = 6;	//�̵�����
	vHahudon.push_back(hahudon);

	pDirection = PLAYER_LEFT;

	startTile = endTile = -1;	//A*

	isTurn = true;

	return S_OK;
}

void Hahudon::release()
{
}

void Hahudon::update()
{
	if (isTurn)
	{
		mouseMove();
	}

	playerAnimation();

	if (KEYMANAGER->isOnceKeyDown('2'))
	{
		isTurn = true;
	}
}

void Hahudon::render(HDC hdc)
{
	for (int k = 0; k < vHahudon.size(); k++)
	{
		vHahudon[k].img->aniRender(hdc, vHahudon[k].rc.left, vHahudon[k].rc.top, playerAni);
	}
}

void Hahudon::mouseMove()
{
	KEYMANAGER->reset();
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			for (int k = 0; k < vHahudon.size(); k++)
			{
				if (PtInRect(&vHahudon[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
				{
					//������ Ÿ�� (����)
					startTile = i;

					isSelect = true;
					isFind = false;
					noPath = false;
					startAstar = false;

					floodFill(startTile, vHahudon[k].movingCount);
				}

				if (!PtInRect(&vHahudon[k].rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
				{
					//������ ���� x��ǥ�� y��ǥ
					mapX = mainMap->getMap()[i].rc.left + (mainMap->getMap()[i].rc.right - mainMap->getMap()[i].rc.left) / 2;
					mapY = mainMap->getMap()[i].rc.top + (mainMap->getMap()[i].rc.bottom - mainMap->getMap()[i].rc.top) / 2;

					isSelect = false;

					if (mainMap->getMap()[i].flood)
					{
						//������ Ÿ�� (��)
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
		playerMove();

		if (playerX == mapX && playerY == mapY)
		{
			//isTurn = false;
		}
	}
}

void Hahudon::playerMove()
{
	for (int k = 0; k < vHahudon.size(); k++)
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

		if (vHahudon[k].rc.left > 0 || vHahudon[k].rc.right < WINSIZEY ||
			vHahudon[k].rc.top > 0 || vHahudon[k].rc.bottom < WINSIZEY)
		{
			switch (pDirection)
			{
			case PLAYER_LEFT:
				playerX -= vHahudon[k].speed;
				vHahudon[k].rc = RectMakeCenter(playerX, playerY, vHahudon[k].img->getFrameWidth(), vHahudon[k].img->getFrameHeight());
				break;
			case PLAYER_RIGHT:
				playerX += vHahudon[k].speed;
				vHahudon[k].rc = RectMakeCenter(playerX, playerY, vHahudon[k].img->getFrameWidth(), vHahudon[k].img->getFrameHeight());
				break;
			case PLAYER_UP:
				playerY -= vHahudon[k].speed;
				vHahudon[k].rc = RectMakeCenter(playerX, playerY, vHahudon[k].img->getFrameWidth(), vHahudon[k].img->getFrameHeight());
				break;
			case PLAYER_DOWN:
				playerY += vHahudon[k].speed;
				vHahudon[k].rc = RectMakeCenter(playerX, playerY, vHahudon[k].img->getFrameWidth(), vHahudon[k].img->getFrameHeight());
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

void Hahudon::playerAnimation()
{
	switch (pDirection)
	{
	case PLAYER_LEFT:
		ANIMATIONMANAGER->addAnimation("playerLeft", "���ĵ�", 4, 5, 2, false, true);
		playerAni = ANIMATIONMANAGER->findAnimation("playerLeft");
		ANIMATIONMANAGER->resume("playerLeft");
		break;
	case PLAYER_RIGHT:
		ANIMATIONMANAGER->addAnimation("playerRight", "���ĵ�", 6, 7, 2, false, true);
		playerAni = ANIMATIONMANAGER->findAnimation("playerRight");
		ANIMATIONMANAGER->resume("playerRight");
		break;
	case PLAYER_UP:
		ANIMATIONMANAGER->addAnimation("playerUp", "���ĵ�", 2, 3, 2, false, true);
		playerAni = ANIMATIONMANAGER->findAnimation("playerUp");
		ANIMATIONMANAGER->resume("playerUp");
		break;
	case PLAYER_DOWN:
		ANIMATIONMANAGER->addAnimation("playerDown", "���ĵ�", 0, 1, 2, false, true);
		playerAni = ANIMATIONMANAGER->findAnimation("playerDown");
		ANIMATIONMANAGER->resume("playerDown");
		break;
	}

	if (!isTurn)
	{
		ANIMATIONMANAGER->addAnimation("playerDie", "���ĵ�", 12, 13, 2, false, true);
		playerAni = ANIMATIONMANAGER->findAnimation("playerDie");
		ANIMATIONMANAGER->resume("playerDie");
	}
}

void Hahudon::setPosition(RECT rc)
{
	for (int k = 0; k < vHahudon.size(); k++)
	{
		vHahudon[k].rc = rc;
		playerX = vHahudon[k].rc.left + (vHahudon[k].rc.right - vHahudon[k].rc.left) / 2;
		playerY = vHahudon[k].rc.top + (vHahudon[k].rc.bottom - vHahudon[k].rc.top) / 2;
	}
}
