#include "stdafx.h"
#include "PlayerBase.h"

PlayerBase::PlayerBase()
{
}

PlayerBase::~PlayerBase()
{
}

HRESULT PlayerBase::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * playerImg, const char * atkImg, const char * blockImg)
{
	return S_OK;
}

void PlayerBase::release()
{
}

void PlayerBase::update()
{
}

void PlayerBase::render(HDC hdc)
{
}

void PlayerBase::mouseMove()
{
}

void PlayerBase::playerMove()
{
}

void PlayerBase::playerAstar()
{
}

void PlayerBase::playerMenu()
{
}

void PlayerBase::playerCollision()
{
}

void PlayerBase::playerAnimation()
{
}

void PlayerBase::playerState()
{
}

void PlayerBase::setPosition(RECT rc)
{
}

void PlayerBase::aStar()
{
	int endX = endTile % TILE_X;
	int endY = endTile / TILE_Y;

	int currentX = currentTile % TILE_X;
	int currentY = currentTile / TILE_Y;

	//Left, Right, Up, Down
	int dx[] = { -1, 1, 0, 0 };
	int dy[] = { 0, 0, -1, 1 };

	bool tempBlock[4];

	//방향을 찾는 반복문
	for (int i = 0; i < 4; i++)
	{
		int x = currentX + dx[i];
		int y = currentY + dy[i];

		tempBlock[i] = false;

		//해당 방향으로 움직인 타일이 유효한 타일인지 확인
		if (0 <= x && x < TILE_X && 0 <= y && y < TILE_Y)
		{
			bool isOpen;

			//대각선 타일의 이동 문제로 (주변에 오브젝트가 있으면 못감) 임시로 오브젝트 상태 저장
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

				//abs절대값
				mainMap->getMap()[y * TILE_X + x].H = (abs(endX - x) + abs(endY - y)) * 10;
				mainMap->getMap()[y * TILE_X + x].F = mainMap->getMap()[y * TILE_X + x].G + mainMap->getMap()[y * TILE_X + x].H;

				//openList에 있으면 G 비용 비교 후 처리
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

				//없으면 그냥 넣고 부모 설정
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

	//선택 지점을 열린 목록에서 빼기
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

	//현재 타일을 closeList에 넣기
	closeList.push_back(currentTile);

	if (openList.size() != 0)
	{
		// 오픈리스트에서 최소 f 비용 찾기
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

	//길 찾기 성공시 각 타일에 길찾기 상태 저장
	int tempTile = endTile;

	while (mainMap->getMap()[tempTile].node != startTile && isFind)
	{
		tempTile = mainMap->getMap()[tempTile].node;
		optimalPath.push(mainMap->getMap()[tempTile]);
	}
}

void PlayerBase::floodFill(int tile, int moveCount)
{
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
		}
	}
}

void PlayerBase::hitDamage(int _damage)
{
	currentHp -= _damage;
}
