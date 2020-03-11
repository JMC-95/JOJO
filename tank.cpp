#include "stdafx.h"
#include "tank.h"

tank::tank()
{
}

tank::~tank()
{
}

HRESULT tank::init(const char* imageName)
{
	//탱크 방향설정
	_direction = TANKDIRECTION_LEFT;

	_image = IMAGEMANAGER->findImage(imageName);
	ANIMATIONMANAGER->addAnimation("playerLeft", "player", 4, 5, 5, false, true);
	_ani = ANIMATIONMANAGER->findAnimation("playerLeft");
	ANIMATIONMANAGER->start("playerLeft");

	//속도
	speed = 6;
	moveCount = 6;

	// A*
	startTile = endTile = -1;

	currentSelect = SELECT_START;

	isTurn = true;
	isMove = false;
	isFind = false;
	noPath = false;
	startAstar = false;

	return S_OK;
}

void tank::release()
{
}

void tank::update()
{
	if (isTurn)
	{
		mouseClick();

		if (!isFind && !noPath && startAstar)
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
		moveCount = 6;
	}
}

void tank::render()
{
	_image->aniRender(getMemDC(), _rc.left, _rc.top, _ani);

	char strBlock[128];
	sprintf_s(strBlock, "moveCount : %d", moveCount);
	SetTextColor(getMemDC(), RGB(255, 255, 0));
	TextOut(getMemDC(), 400, 10, strBlock, strlen(strBlock));
}

void tank::mouseClick()
{
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			if (PtInRect(&_rc, m_ptMouse) && PtInRect(&_mainMap->getMap()[i].rc, m_ptMouse))
			{
				currentSelect = SELECT_START;
				startTile = i;

				isRange = true;
				isFind = false;
				noPath = false;
				startAstar = false;
			}

			if (!PtInRect(&_rc, m_ptMouse) && PtInRect(&_mainMap->getMap()[i].rc, m_ptMouse) && isRange)
			{
				mapX = _mainMap->getMap()[i].rc.left + (_mainMap->getMap()[i].rc.right - _mainMap->getMap()[i].rc.left) / 2;
				mapY = _mainMap->getMap()[i].rc.top + (_mainMap->getMap()[i].rc.bottom - _mainMap->getMap()[i].rc.top) / 2;

				currentSelect = SELECT_END;
				endTile = i;

				isRange = false;

				if (_mainMap->getMap()[i].obj == OBJ_MOUNTAIN ||
					_mainMap->getMap()[i].obj == OBJ_ROCKMOUNTAIN ||
					_mainMap->getMap()[i].obj == OBJ_CASTLEWALLS) continue;

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
			if (_mainMap->getMap()[y * TILE_X + x].obj == OBJ_MOUNTAIN ||
				_mainMap->getMap()[y * TILE_X + x].obj == OBJ_ROCKMOUNTAIN ||
				_mainMap->getMap()[y * TILE_X + x].obj == OBJ_CASTLEWALLS) tempBlock[i] = true;
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
					_mainMap->getMap()[y * TILE_X + x].G = 10;
				}

				//abs절대값
				_mainMap->getMap()[y * TILE_X + x].H = (abs(endX - x) + abs(endY - y)) * 10;
				_mainMap->getMap()[y * TILE_X + x].F = _mainMap->getMap()[y * TILE_X + x].G + _mainMap->getMap()[y * TILE_X + x].H;

				//openList에 있으면 G 비용 비교 후 처리
				isOpen = false;

				for (int i = 0; i < openList.size(); i++)
				{
					if (openList[i] == y * TILE_X + x)
					{
						isOpen = true;

						if (_mainMap->getMap()[openList[i]].G > _mainMap->getMap()[y * TILE_X + x].G)
						{
							_mainMap->getMap()[openList[i]].H = _mainMap->getMap()[y * TILE_X + x].H;
							_mainMap->getMap()[openList[i]].G = _mainMap->getMap()[y * TILE_X + x].G;
							_mainMap->getMap()[openList[i]].F = _mainMap->getMap()[y * TILE_X + x].F;
							_mainMap->getMap()[openList[i]].node = currentTile;
						}
					}
				}

				//없으면 그냥 넣고 부모 설정
				if (!isOpen)
				{
					openList.push_back(y * TILE_X + x);
					_mainMap->getMap()[y * TILE_X + x].node = currentTile;
				}

				//find
				if (y * TILE_X + x == endTile)
				{
					isFind = true;
					optimalPath.push(_mainMap->getMap()[endTile]);
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
		//find minimum f cost in openList
		int min = _mainMap->getMap()[*openList.begin()].H;

		currentTile = *openList.begin();

		for (iter = openList.begin(); iter != openList.end(); ++iter)
		{
			if (min > _mainMap->getMap()[(*iter)].H)
			{
				min = _mainMap->getMap()[(*iter)].H;
				currentTile = *iter;
			}
		}
	}

	//길 찾기 성공시 각 타일에 길찾기 상태 저장
	int tempTile = endTile;

	while (_mainMap->getMap()[tempTile].node != startTile && isFind)
	{
		tempTile = _mainMap->getMap()[tempTile].node;

		optimalPath.push(_mainMap->getMap()[tempTile]);
	}
}

void tank::tankMove()
{
	RECT rcCollision;
	int tileIndex[2];

	rcCollision = _rc;

	sX = optimalPath.top().rc.left + (optimalPath.top().rc.right - optimalPath.top().rc.left) / 2;
	sY = optimalPath.top().rc.top + (optimalPath.top().rc.bottom - optimalPath.top().rc.top) / 2;

	if (!isMove)
	{
		if (playerX > sX)
		{
			_direction = TANKDIRECTION_LEFT;
		}
		else if (playerX < sX)
		{
			_direction = TANKDIRECTION_RIGHT;
		}
		else if (playerY > sY)
		{
			_direction = TANKDIRECTION_UP;
		}
		else if (playerY < sY)
		{
			_direction = TANKDIRECTION_DOWN;
		}

		isMove = true;
	}

	if (_rc.left > 0 || _rc.right < WINSIZEY ||
		_rc.top > 0 || _rc.bottom < WINSIZEY)
	{
		switch (_direction)
		{
		case TANKDIRECTION_LEFT:
			playerX -= speed;
			rcCollision = RectMakeCenter(playerX, playerY, _image->getFrameWidth(), _image->getFrameHeight());
			break;
		case TANKDIRECTION_RIGHT:
			playerX += speed;
			rcCollision = RectMakeCenter(playerX, playerY, _image->getFrameWidth(), _image->getFrameHeight());
			break;
		case TANKDIRECTION_UP:
			playerY -= speed;
			rcCollision = RectMakeCenter(playerX, playerY, _image->getFrameWidth(), _image->getFrameHeight());
			break;
		case TANKDIRECTION_DOWN:
			playerY += speed;
			rcCollision = RectMakeCenter(playerX, playerY, _image->getFrameWidth(), _image->getFrameHeight());
			break;
		}

		if (playerX == sX && playerY == sY)
		{
			isMove = false;
			optimalPath.pop();
			//moveCount -= 1;
		}
	}

	tileX = rcCollision.left / TILE_WIDTH;
	tileY = rcCollision.top / TILE_HEIGHT;

	switch (_direction)
	{
	case TANKDIRECTION_LEFT:
		tileIndex[0] = tileX + tileY * TILE_X;
		tileIndex[1] = tileX + (tileY + 1) * TILE_Y;
		break;
	case TANKDIRECTION_RIGHT:
		tileIndex[0] = (tileX + tileY * TILE_X) + 1;
		tileIndex[1] = (tileX + (tileY + 1) * TILE_Y) + 1;
		break;
	case TANKDIRECTION_UP:
		tileIndex[0] = tileX + tileY * TILE_X;
		tileIndex[1] = tileX + 1 + tileY * TILE_Y;
		break;
	case TANKDIRECTION_DOWN:
		tileIndex[0] = (tileX + tileY * TILE_X) + TILE_X;
		tileIndex[1] = (tileX + 1 + tileY * TILE_Y) + TILE_Y;
		break;
	}//end of switch(_direction)

	for (int i = 0; i < 2; i++)
	{
		RECT temp;

		if (((_mainMap->getAttribute()[tileIndex[i]] & ATTR_UNMOVABLE) == ATTR_UNMOVABLE) &&
			IntersectRect(&temp, &_mainMap->getMap()[tileIndex[i]].rc, &rcCollision))
		{
			switch (_direction)
			{
			case TANKDIRECTION_LEFT:
				_rc.left = _mainMap->getMap()[tileIndex[i]].rc.right;
				_rc.right = _rc.left + _image->getFrameWidth();
				playerX = _rc.left + (_rc.right - _rc.left) / 2;
				break;
			case TANKDIRECTION_RIGHT:
				_rc.right = _mainMap->getMap()[tileIndex[i]].rc.left;
				_rc.left = _rc.right - _image->getFrameWidth();
				playerX = _rc.left + (_rc.right - _rc.left) / 2;
				break;
			case TANKDIRECTION_UP:
				_rc.top = _mainMap->getMap()[tileIndex[i]].rc.bottom;
				_rc.bottom = _rc.top + _image->getFrameHeight();
				playerY = _rc.top + (_rc.bottom - _rc.top) / 2;
				break;
			case TANKDIRECTION_DOWN:
				_rc.bottom = _mainMap->getMap()[tileIndex[i]].rc.top;
				_rc.top = _rc.bottom - _image->getFrameHeight();
				playerY = _rc.top + (_rc.bottom - _rc.top) / 2;
				break;
			}
			return;
		}
	}//end of for

	rcCollision = RectMakeCenter(playerX, playerY, _image->getFrameWidth(), _image->getFrameHeight());
	_rc = rcCollision;
}

void tank::animation()
{
	switch (_direction)
	{
	case TANKDIRECTION_LEFT:
		ANIMATIONMANAGER->addAnimation("playerLeft", "player", 4, 5, 5, false, true);
		_ani = ANIMATIONMANAGER->findAnimation("playerLeft");
		ANIMATIONMANAGER->resume("playerLeft");
		break;
	case TANKDIRECTION_RIGHT:
		ANIMATIONMANAGER->addAnimation("playerRight", "player", 6, 7, 5, false, true);
		_ani = ANIMATIONMANAGER->findAnimation("playerRight");
		ANIMATIONMANAGER->resume("playerRight");
		break;
	case TANKDIRECTION_UP:
		ANIMATIONMANAGER->addAnimation("playerUp", "player", 2, 3, 5, false, true);
		_ani = ANIMATIONMANAGER->findAnimation("playerUp");
		ANIMATIONMANAGER->resume("playerUp");
		break;
	case TANKDIRECTION_DOWN:
		ANIMATIONMANAGER->addAnimation("playerDown", "player", 0, 1, 5, false, true);
		_ani = ANIMATIONMANAGER->findAnimation("playerDown");
		ANIMATIONMANAGER->resume("playerDown");
		break;
	}

	if (!isTurn)
	{
		ANIMATIONMANAGER->addAnimation("playerDie", "player", 12, 13, 5, false, true);
		_ani = ANIMATIONMANAGER->findAnimation("playerDie");
		ANIMATIONMANAGER->resume("playerDie");
	}
}

void tank::setTankPosition(RECT rc)
{
	_rc = rc;
	playerX = _rc.left + (_rc.right - _rc.left) / 2;
	playerY = _rc.top + (_rc.bottom - _rc.top) / 2;
}
