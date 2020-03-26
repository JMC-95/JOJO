#include "stdafx.h"
#include "FriendlyBase.h"

FriendlyBase::FriendlyBase()
{
}

FriendlyBase::~FriendlyBase()
{
}

HRESULT FriendlyBase::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * playerImg, const char * atkImg, const char * blockImg)
{
	return S_OK;
}

void FriendlyBase::release()
{
}

void FriendlyBase::update()
{
}

void FriendlyBase::render(HDC hdc)
{
}

void FriendlyBase::mouseMove()
{
}

void FriendlyBase::friendAi()
{
}

void FriendlyBase::friendMove()
{
}

void FriendlyBase::friendAstar()
{
}

void FriendlyBase::friendMenu()
{
}

void FriendlyBase::friendCollision()
{
}

void FriendlyBase::friendAnimation()
{
}

void FriendlyBase::friendState()
{
}

void FriendlyBase::setPosition(RECT rc)
{
}

void FriendlyBase::aStar()
{
	int endX = endTile % TILE_X;
	int endY = endTile / TILE_Y;

	int currentX = currentTile % TILE_X;
	int currentY = currentTile / TILE_Y;

	//Left, Right, Up, Down
	int dx[] = { -1, 1, 0, 0 };
	int dy[] = { 0, 0, -1, 1 };

	bool tempBlock[4];

	//������ ã�� �ݺ���
	for (int i = 0; i < 4; i++)
	{
		int x = currentX + dx[i];
		int y = currentY + dy[i];

		tempBlock[i] = false;

		//�ش� �������� ������ Ÿ���� ��ȿ�� Ÿ������ Ȯ��
		if (0 <= x && x < TILE_X && 0 <= y && y < TILE_Y)
		{
			bool isOpen;

			//�밢�� Ÿ���� �̵� ������ (�ֺ��� ������Ʈ�� ������ ����) �ӽ÷� ������Ʈ ���� ����
			if (mainMap->getMap()[y * TILE_X + x].obj == OBJ_MOUNTAIN ||
				mainMap->getMap()[y * TILE_X + x].obj == OBJ_ROCKMOUNTAIN ||
				mainMap->getMap()[y * TILE_X + x].obj == OBJ_CASTLEWALLS ||
				mainMap->getMap()[y * TILE_X + x].obj == OBJ_CASTLEGATE) tempBlock[i] = true;
			else
			{
				//check closeList
				bool isClose = false;

				for (int j = 0; j < closeList.size(); j++)
				{
					if (closeList[j] == y * TILE_X + x)
					{
						isClose = true;
						break;
					}
				}

				if (isClose) continue;

				if (i < 4)
				{
					mainMap->getMap()[y * TILE_X + x].G = 10;
				}

				//abs���밪
				mainMap->getMap()[y * TILE_X + x].H = (abs(endX - x) + abs(endY - y)) * 10;
				mainMap->getMap()[y * TILE_X + x].F = mainMap->getMap()[y * TILE_X + x].G + mainMap->getMap()[y * TILE_X + x].H;

				//openList�� ������ G ��� �� �� ó��
				isOpen = false;

				for (int i = 0; i < openList.size(); i++)
				{
					if (openList[i] == y * TILE_X + x)
					{
						isOpen = true;

						if (mainMap->getMap()[openList[i]].G > mainMap->getMap()[y * TILE_X + x].G)
						{
							mainMap->getMap()[openList[i]].H = mainMap->getMap()[y * TILE_X + x].H;
							mainMap->getMap()[openList[i]].G = mainMap->getMap()[y * TILE_X + x].G;
							mainMap->getMap()[openList[i]].F = mainMap->getMap()[y * TILE_X + x].F;
							mainMap->getMap()[openList[i]].node = currentTile;
						}
					}
				}

				//������ �׳� �ְ� �θ� ����
				if (!isOpen)
				{
					openList.push_back(y * TILE_X + x);
					mainMap->getMap()[y * TILE_X + x].node = currentTile;
				}

				//find
				if (y * TILE_X + x == endTile)
				{
					isFind = true;
					optimalPath.push(mainMap->getMap()[endTile]);
				}
			}
		}
	}

	//���� ������ ���� ��Ͽ��� ����
	for (iter = openList.begin(); iter != openList.end(); ++iter)
	{
		if ((*iter) == currentTile)
		{
			iter = openList.erase(iter);
			break;
		}
	}

	//not Find
	if (openList.size() == 0)
	{
		noPath = true;
	}

	//���� Ÿ���� closeList�� �ֱ�
	closeList.push_back(currentTile);

	if (openList.size() != 0)
	{
		// ���¸���Ʈ���� �ּ� f ��� ã��
		int min = mainMap->getMap()[*openList.begin()].H;

		currentTile = *openList.begin();

		for (iter = openList.begin(); iter != openList.end(); ++iter)
		{
			if (min > mainMap->getMap()[(*iter)].H)
			{
				min = mainMap->getMap()[(*iter)].H;
				currentTile = *iter;
			}
		}
	}

	//�� ã�� ������ �� Ÿ�Ͽ� ��ã�� ���� ����
	int tempTile = endTile;

	while (mainMap->getMap()[tempTile].node != startTile && isFind)
	{
		tempTile = mainMap->getMap()[tempTile].node;
		optimalPath.push(mainMap->getMap()[tempTile]);
	}
}

void FriendlyBase::floodFill(int tile, int moveCount)
{
	RECT temp;

	if (mainMap->getMap()[tile].obj != OBJ_CASTLEWALLS &&
		mainMap->getMap()[tile].obj != OBJ_ROCKMOUNTAIN &&
		mainMap->getMap()[tile].obj != OBJ_MOUNTAIN &&
		mainMap->getMap()[tile].obj != OBJ_CASTLEGATE)
	{
		if (moveCount >= 0 && tile % TILE_X)
		{
			if (mainMap->getMap()[tile].terrain == TR_PLAIN)
			{
				floodFill(tile - 1, moveCount - 1);
				floodFill(tile + 1, moveCount - 1);
				floodFill(tile - 20, moveCount - 1);
				floodFill(tile + 20, moveCount - 1);
			}
			else
			{
				floodFill(tile - 1, moveCount - 2);
				floodFill(tile + 1, moveCount - 2);
				floodFill(tile - 20, moveCount - 2);
				floodFill(tile + 20, moveCount - 2);
			}

			mainMap->getMap()[tile].flood = true;

			for (int k = 4; k < 18; ++k)
			{
				auto& rc = mainMap->getMap()[tile].rc;
				auto& enemyRect = ENEMYMANAGER->getEnemy()[k]->getEnemyInfo().rc;

				if (IntersectRect(&temp, &rc, &enemyRect))
				{
					positionX = ENEMYMANAGER->getEnemy()[k]->getEnemyInfo().rc.left / TILE_WIDTH;
					positionY = ENEMYMANAGER->getEnemy()[k]->getEnemyInfo().rc.top / TILE_HEIGHT;
					enemyTile = positionX + (positionY * TILE_Y);

					mainMap->getMap()[tile].flood = false;
				}
			}
		}
	}
}

void FriendlyBase::hitDamage(int _damage)
{
	currentHp -= _damage;
}
