#include "stdafx.h"
#include "tank.h"

tank::tank()
{
}

tank::~tank()
{
}

HRESULT tank::init(const char* rngImageName, const char* atkRngImageName, const char* atkImageName, const char* imageName)
{
	//탱크 방향설정
	direction = TANKDIRECTION_LEFT;

	//이미지 및 애니메이션
	rngImg = IMAGEMANAGER->findImage(rngImageName);
	atkRngImg = IMAGEMANAGER->findImage(atkRngImageName);
	atkImg = IMAGEMANAGER->findImage(atkImageName);
	img = IMAGEMANAGER->findImage(imageName);
	ANIMATIONMANAGER->addAnimation("playerLeft", "player", 4, 5, 5, false, true);
	ani = ANIMATIONMANAGER->findAnimation("playerLeft");
	ANIMATIONMANAGER->start("playerLeft");

	speed = 6;					//속도
	movingCount = 6;			//이동범위
	startTile = endTile = -1;	//A*

	isTurn = true;

	return S_OK;
}

void tank::release()
{
}

void tank::update()
{
	if (isTurn)
	{
		mouseMove();

		if (startAstar && !isFind && !noPath)
		{
			while (!isFind)
			{
				aStar();
			}
		}

		if (!optimalPath.empty())
		{
			tankMove();

			if (playerX == mapX && playerY == mapY)
			{
				isTurn = false;
			}
		}
	}

	animation();

	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		isTurn = true;
		isAtk = true;
	}
}

void tank::render()
{
	if (isRange)
	{
		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			if (mainMap->getMap()[i].flood)
			{
				rngImg->render(getMemDC(), mainMap->getMap()[i].rc.left, mainMap->getMap()[i].rc.top);

				atkRngImg->render(getMemDC(), _rc.left - 48, _rc.top);
				atkRngImg->render(getMemDC(), _rc.left + 48, _rc.top);
				atkRngImg->render(getMemDC(), _rc.left, _rc.top - 48);
				atkRngImg->render(getMemDC(), _rc.left, _rc.top + 48);
			}
		}
	}

	if (isAtk)
	{
		atkImg->render(getMemDC(), _rc.left - 48, _rc.top);
		atkImg->render(getMemDC(), _rc.left + 48, _rc.top);
		atkImg->render(getMemDC(), _rc.left, _rc.top - 48);
		atkImg->render(getMemDC(), _rc.left, _rc.top + 48);
	}

	img->aniRender(getMemDC(), _rc.left, _rc.top, ani);
}

void tank::mouseMove()
{
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			if (PtInRect(&_rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse))
			{
				//선택한 타일 (시작)
				startTile = i;

				isSelect = true;
				isRange = true;
				isFind = false;
				noPath = false;
				startAstar = false;

				floodFill(startTile, movingCount);
			}

			if (!PtInRect(&_rc, m_ptMouse) && PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
			{
				//선택한 맵의 x좌표와 y좌표
				mapX = mainMap->getMap()[i].rc.left + (mainMap->getMap()[i].rc.right - mainMap->getMap()[i].rc.left) / 2;
				mapY = mainMap->getMap()[i].rc.top + (mainMap->getMap()[i].rc.bottom - mainMap->getMap()[i].rc.top) / 2;

				isSelect = false;

				if (mainMap->getMap()[i].flood)
				{
					//선택한 타일 (끝)
					endTile = i;
				}
				else
				{
					isRange = false;
					continue;
				}

				//이순간 Astar가 시작된다.
				//Astar에 필요한 모든 것을 초기화 시켜주자.
				openList.clear();
				closeList.clear();

				if (startTile != -1 && endTile != -1)
				{
					startAstar = true;
					currentTile = startTile;

					//시작 지점을 openList에 넣자
					openList.push_back(currentTile);
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

void tank::aStar()
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

void tank::tankMove()
{
	sX = optimalPath.top().rc.left + (optimalPath.top().rc.right - optimalPath.top().rc.left) / 2;
	sY = optimalPath.top().rc.top + (optimalPath.top().rc.bottom - optimalPath.top().rc.top) / 2;

	if (!isMove)
	{
		if (playerX > sX)
		{
			direction = TANKDIRECTION_LEFT;
		}
		else if (playerX < sX)
		{
			direction = TANKDIRECTION_RIGHT;
		}
		else if (playerY > sY)
		{
			direction = TANKDIRECTION_UP;
		}
		else if (playerY < sY)
		{
			direction = TANKDIRECTION_DOWN;
		}

		isMove = true;
	}

	if (_rc.left > 0 || _rc.right < WINSIZEY ||
		_rc.top > 0 || _rc.bottom < WINSIZEY)
	{
		switch (direction)
		{
		case TANKDIRECTION_LEFT:
			playerX -= speed;
			_rc = RectMakeCenter(playerX, playerY, img->getFrameWidth(), img->getFrameHeight());
			break;
		case TANKDIRECTION_RIGHT:
			playerX += speed;
			_rc = RectMakeCenter(playerX, playerY, img->getFrameWidth(), img->getFrameHeight());
			break;
		case TANKDIRECTION_UP:
			playerY -= speed;
			_rc = RectMakeCenter(playerX, playerY, img->getFrameWidth(), img->getFrameHeight());
			break;
		case TANKDIRECTION_DOWN:
			playerY += speed;
			_rc = RectMakeCenter(playerX, playerY, img->getFrameWidth(), img->getFrameHeight());
			break;
		}

		if (playerX == sX && playerY == sY)
		{
			isMove = false;
			optimalPath.pop();
		}
	}
}

void tank::animation()
{
	switch (direction)
	{
	case TANKDIRECTION_LEFT:
		ANIMATIONMANAGER->addAnimation("playerLeft", "player", 4, 5, 5, false, true);
		ani = ANIMATIONMANAGER->findAnimation("playerLeft");
		ANIMATIONMANAGER->resume("playerLeft");
		break;
	case TANKDIRECTION_RIGHT:
		ANIMATIONMANAGER->addAnimation("playerRight", "player", 6, 7, 5, false, true);
		ani = ANIMATIONMANAGER->findAnimation("playerRight");
		ANIMATIONMANAGER->resume("playerRight");
		break;
	case TANKDIRECTION_UP:
		ANIMATIONMANAGER->addAnimation("playerUp", "player", 2, 3, 5, false, true);
		ani = ANIMATIONMANAGER->findAnimation("playerUp");
		ANIMATIONMANAGER->resume("playerUp");
		break;
	case TANKDIRECTION_DOWN:
		ANIMATIONMANAGER->addAnimation("playerDown", "player", 0, 1, 5, false, true);
		ani = ANIMATIONMANAGER->findAnimation("playerDown");
		ANIMATIONMANAGER->resume("playerDown");
		break;
	}

	if (!isTurn)
	{
		ANIMATIONMANAGER->addAnimation("playerDie", "player", 12, 13, 5, false, true);
		ani = ANIMATIONMANAGER->findAnimation("playerDie");
		ANIMATIONMANAGER->resume("playerDie");
	}
}

void tank::floodFill(int tile, int moveCount)
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

void tank::setTankPosition(RECT rc)
{
	_rc = rc;
	playerX = _rc.left + (_rc.right - _rc.left) / 2;
	playerY = _rc.top + (_rc.bottom - _rc.top) / 2;
}
