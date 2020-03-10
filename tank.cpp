#include "stdafx.h"
#include "tank.h"

tank::tank()
{
}

tank::~tank()
{
}

HRESULT tank::init()
{
	//탱크 방향설정
	_direction = TANKDIRECTION_LEFT;

	_image = IMAGEMANAGER->addFrameImage("player", "images/Player/아군기병.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	ANIMATIONMANAGER->addAnimation("playerLeft", "player", 4, 5, 5, false, true);
	_ani = ANIMATIONMANAGER->findAnimation("playerLeft");
	ANIMATIONMANAGER->start("playerLeft");

	//속도
	speed = 6;

	// A*
	startTile = endTile = -1;

	currentSelect = SELECT_START;

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
	mouseClick();
	animation();

	if (!isFind && !noPath && startAstar)
	{
		while (!isFind)
		{
			Astar();
		}
	}
}

void tank::render()
{
	if (isTurn)
	{
		//IMAGEMANAGER->render("range", getMemDC(), _rc.left - 48, _rc.top - 48);
	}

	_image->aniRender(getMemDC(), _rc.left, _rc.top, _ani);

	char strBlock[128];
	sprintf_s(strBlock, "open : %d, close : %d", openList.size(), closeList.size());
	SetTextColor(getMemDC(), RGB(255, 255, 0));
	TextOut(getMemDC(), 400, 10, strBlock, strlen(strBlock));
}

void tank::setTankPosition(RECT rc)
{
	_rc = rc;
	x = _rc.left + (_rc.right - _rc.left) / 2;
	y = _rc.top + (_rc.bottom - _rc.top) / 2;
}

void tank::mouseClick()
{
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		for (int i = 0; i < TILEX * TILEY; i++)
		{
			if (PtInRect(&_rc, m_ptMouse) && PtInRect(&_testMap->getMap()[i].rc, m_ptMouse))
			{
				currentSelect = SELECT_START;
				startTile = i;

				isTurn = true;
				isFind = false;
				noPath = false;
				startAstar = false;
			}

			if (!PtInRect(&_rc, m_ptMouse) && PtInRect(&_testMap->getMap()[i].rc, m_ptMouse) && isTurn)
			{
				if (_testMap->getMap()[i].obj == OBJ_MOUNTAIN ||
					_testMap->getMap()[i].obj == OBJ_ROCKMOUNTAIN ||
					_testMap->getMap()[i].obj == OBJ_CASTLEWALLS) continue;

				//이순간 Astar가 시작된다.
				//Astar에 필요한 모든것을 초기화 시켜주자.
				openList.clear();
				closeList.clear();

				currentSelect = SELECT_END;
				endTile = i;

				isTurn = false;

				if (startTile != -1 && endTile != -1)
				{
					startAstar = true;
					currentTile = startTile;

					//시작지점을 오픈리스트에 넣자
					openList.push_back(currentTile);
				}
			}
		}
	}

	if (!optimalPath.empty())
	{
		tankMove();
	}
}

void tank::tankMove()
{
	RECT rcCollision;
	int tileIndex[2];

	rcCollision = _rc;

	if (!isMove)
	{
		sX = optimalPath.top().rc.left + (optimalPath.top().rc.right - optimalPath.top().rc.left) / 2;
		sY = optimalPath.top().rc.top + (optimalPath.top().rc.bottom - optimalPath.top().rc.top) / 2;
		if (x > sX)
		{
			_direction = TANKDIRECTION_LEFT;
		}
		else if (x < sX)
		{
			_direction = TANKDIRECTION_RIGHT;
		}
		else if (y > sY)
		{
			_direction = TANKDIRECTION_UP;
		}
		else if (y < sY)
		{
			_direction = TANKDIRECTION_DOWN;
		}

		isMove = true;
	}

	switch (_direction)
	{
	case TANKDIRECTION_LEFT:
		if (_rc.left > 0)
		{
			x -= speed;
			rcCollision = RectMakeCenter(x, y, _image->getFrameWidth(), _image->getFrameHeight());
		}
		if (x == sX)
		{
			isMove = false;
			optimalPath.pop();
		}
		break;
	case TANKDIRECTION_RIGHT:
		if (_rc.right < WINSIZEY)
		{
			x += speed;
			rcCollision = RectMakeCenter(x, y, _image->getFrameWidth(), _image->getFrameHeight());
		}
		if (x == sX)
		{
			isMove = false;
			optimalPath.pop();
		}
		break;
	case TANKDIRECTION_UP:
		if (_rc.top > 0)
		{
			y -= speed;
			rcCollision = RectMakeCenter(x, y, _image->getFrameWidth(), _image->getFrameHeight());
		}
		if (y == sY)
		{
			isMove = false;
			optimalPath.pop();
		}
		break;
	case TANKDIRECTION_DOWN:
		if (_rc.bottom < WINSIZEY)
		{
			y += speed;
			rcCollision = RectMakeCenter(x, y, _image->getFrameWidth(), _image->getFrameHeight());
		}
		if (y == sY)
		{
			isMove = false;
			optimalPath.pop();
		}
		break;
	}

	tileX = rcCollision.left / TILESIZE;
	tileY = rcCollision.top / TILESIZE;

	switch (_direction)
	{
	case TANKDIRECTION_LEFT:
		tileIndex[0] = tileX + tileY * TILEX;
		tileIndex[1] = tileX + (tileY + 1) * TILEY;
		break;
	case TANKDIRECTION_RIGHT:
		tileIndex[0] = (tileX + tileY * TILEX) + 1;
		tileIndex[1] = (tileX + (tileY + 1) * TILEY) + 1;
		break;
	case TANKDIRECTION_UP:
		tileIndex[0] = tileX + tileY * TILEX;
		tileIndex[1] = tileX + 1 + tileY * TILEY;
		break;
	case TANKDIRECTION_DOWN:
		tileIndex[0] = (tileX + tileY * TILEX) + TILEX;
		tileIndex[1] = (tileX + 1 + tileY * TILEY) + TILEY;
		break;
	}//end of switch(_direction)

	for (int i = 0; i < 2; i++)
	{
		RECT temp;

		if (((_testMap->getAttribute()[tileIndex[i]] & ATTR_UNMOVABLE) == ATTR_UNMOVABLE) &&
			IntersectRect(&temp, &_testMap->getMap()[tileIndex[i]].rc, &rcCollision))
		{
			switch (_direction)
			{
			case TANKDIRECTION_LEFT:
				_rc.left = _testMap->getMap()[tileIndex[i]].rc.right;
				_rc.right = _rc.left + _image->getFrameWidth();
				x = _rc.left + (_rc.right - _rc.left) / 2;
				break;
			case TANKDIRECTION_RIGHT:
				_rc.right = _testMap->getMap()[tileIndex[i]].rc.left;
				_rc.left = _rc.right - _image->getFrameWidth();
				x = _rc.left + (_rc.right - _rc.left) / 2;
				break;
			case TANKDIRECTION_UP:
				_rc.top = _testMap->getMap()[tileIndex[i]].rc.bottom;
				_rc.bottom = _rc.top + _image->getFrameHeight();
				y = _rc.top + (_rc.bottom - _rc.top) / 2;
				break;
			case TANKDIRECTION_DOWN:
				_rc.bottom = _testMap->getMap()[tileIndex[i]].rc.top;
				_rc.top = _rc.bottom - _image->getFrameHeight();
				y = _rc.top + (_rc.bottom - _rc.top) / 2;
				break;
			}
			return;
		}
	}//end of for

	rcCollision = RectMakeCenter(x, y, _image->getFrameWidth(), _image->getFrameHeight());
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
}

void tank::Astar()
{
	int endX = endTile % TILEX;
	int endY = endTile / TILEY;

	int currentX = currentTile % TILEX;
	int currentY = currentTile / TILEY;

	// Left, Right, Up, Down
	int dx[] = { -1, 1, 0, 0 };
	int dy[] = { 0, 0, -1, 1 };

	bool tempBlock[4];

	// 방향 찾는 반복문
	for (int i = 0; i < 4; i++)
	{
		int x = currentX + dx[i];
		int y = currentY + dy[i];

		tempBlock[i] = false;

		// 해당 방향으로 움직인 타일이 유효한 타일인지 확인
		if (0 <= x && x < TILEX && 0 <= y && y < TILEY)
		{
			bool isOpen;

			// 대각선 타일의 이동 문제로 (주변에 블락있으면 못감) 임시로 블락 상태 저장
			if (_testMap->getMap()[y * TILEX + x].obj == OBJ_MOUNTAIN ||
				_testMap->getMap()[y * TILEX + x].obj == OBJ_ROCKMOUNTAIN ||
				_testMap->getMap()[y * TILEX + x].obj == OBJ_CASTLEWALLS) tempBlock[i] = true;
			else
			{
				// check closeList z
				bool isClose = false;

				for (int j = 0; j < closeList.size(); j++)
				{
					if (closeList[j] == y * TILEX + x)
					{
						isClose = true;
						break;
					}
				}

				if (isClose) continue;

				if (i < 4)
				{
					_testMap->getMap()[y * TILEX + x].g = 10;
				}

				//abs절대값
				_testMap->getMap()[y * TILEX + x].h = (abs(endX - x) + abs(endY - y)) * 10;
				_testMap->getMap()[y * TILEX + x].f = _testMap->getMap()[y * TILEX + x].g + _testMap->getMap()[y * TILEX + x].h;

				// 오픈리스트에 있으면 g 비용 비교 후 처리
				isOpen = false;

				for (int i = 0; i < openList.size(); i++)
				{
					if (openList[i] == y * TILEX + x)
					{
						isOpen = true;

						if (_testMap->getMap()[openList[i]].g > _testMap->getMap()[y * TILEX + x].g)
						{
							_testMap->getMap()[openList[i]].h = _testMap->getMap()[y * TILEX + x].h;
							_testMap->getMap()[openList[i]].g = _testMap->getMap()[y * TILEX + x].g;
							_testMap->getMap()[openList[i]].f = _testMap->getMap()[y * TILEX + x].f;
							_testMap->getMap()[openList[i]].node = currentTile;
						}
					}
				}

				// 없으면 그냥 넣고 부모 설정
				if (!isOpen)
				{
					openList.push_back(y * TILEX + x);
					_testMap->getMap()[y * TILEX + x].node = currentTile;
				}

				// find
				if (y * TILEX + x == endTile)
				{
					isFind = true;
					optimalPath.push(_testMap->getMap()[endTile]);
				}
			}
		}
	}

	// 선택 지점 열린목록에서 빼기
	for (iter = openList.begin(); iter != openList.end(); ++iter)
	{
		if ((*iter) == currentTile)
		{
			iter = openList.erase(iter);
			break;
		}
	}

	// not Find
	if (openList.size() == 0)
	{
		noPath = true;
	}

	// 현재 타일 클로즈리스트에 넣기
	closeList.push_back(currentTile);

	if (openList.size() != 0)
	{
		// find minimum f cost in openList
		int min = _testMap->getMap()[*openList.begin()].h;

		currentTile = *openList.begin();

		for (iter = openList.begin(); iter != openList.end(); ++iter)
		{
			if (min > _testMap->getMap()[(*iter)].h)
			{
				min = _testMap->getMap()[(*iter)].h;
				currentTile = *iter;
			}
		}
	}

	// 길 찾기 성공시 각 타일에 길찾기 상태 저장
	int tempTile = endTile;

	while (_testMap->getMap()[tempTile].node != startTile && isFind)
	{
		tempTile = _testMap->getMap()[tempTile].node;

		optimalPath.push(_testMap->getMap()[tempTile]);
	}
}
