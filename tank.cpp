#include "stdafx.h"
#include "tank.h"

tank::tank()
{
}

tank::~tank()
{
}

HRESULT tank::init(const char * imageName)
{
	//탱크 방향설정
	_direction = TANKDIRECTION_LEFT;

	_image = IMAGEMANAGER->findImage(imageName);
	ANIMATIONMANAGER->addAnimation("playerDown", "player", 0, 1, 4, false, true);
	_ani = ANIMATIONMANAGER->findAnimation("playerDown");

	//속도
	speed = 100.0f;

	return S_OK;
}

void tank::release()
{
}

void tank::update()
{
	mouseClick();

	if (KEYMANAGER->isStayKeyDown(VK_LEFT) && _rc.left > 0)
	{
		_direction = TANKDIRECTION_LEFT;
		tankMove();
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && _rc.right < WINSIZEY)
	{
		_direction = TANKDIRECTION_RIGHT;
		tankMove();
	}
	if (KEYMANAGER->isStayKeyDown(VK_UP) && _rc.top > 0)
	{
		_direction = TANKDIRECTION_UP;
		tankMove();
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN) && _rc.bottom < WINSIZEY)
	{
		_direction = TANKDIRECTION_DOWN;
		tankMove();
	}
}

void tank::render()
{
	if (KEYMANAGER->isToggleKey(VK_LBUTTON) && isTurn)
	{
		IMAGEMANAGER->render("range", getMemDC(), _rc.left - 48, _rc.top - 48);
	}

	_image->aniRender(getMemDC(), _rc.left, _rc.top, _ani);

	char strBlock[128];
	sprintf_s(strBlock, "X : %d번 타일, Y : %d번 타일", tileX, tileY);
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
		if (PtInRect(&_rc, m_ptMouse))
		{
			isTurn = true;
		}

		for (int i = 0; i < TILEX * TILEY; i++)
		{
			if (_testMap->getMap()[i].terrain != TR_PLAIN)
			{
				if (PtInRect(&_testMap->getMap()[i].rc, m_ptMouse))
				{
					isTurn = false;
					isMove = true;
				}
			}
		}
	}

	//캐릭터는 처음부터 위치가 고정되어 있기 때문에 캐릭터의 위치를 미리 오브젝트로 지정해주고,
	//오브젝트가 아닌 테라인 일 때 움직일 수 있게 한다.
	//맵툴에 미리 깔아둔 오브젝트를 어떻게 움직일 수 있을까..?

	if (isMove)
	{
		tankMove();
	}
}

void tank::tankMove()
{
	RECT rcCollision;
	int tileIndex[2];	//검사용 타일

	rcCollision = _rc;	//가상의 렉트

	//타임 매니저를 이용한 방법
	float elpasedTime = TIMEMANAGER->getElapsedTime();
	float moveSpeed = elpasedTime * speed;

	//일단 무조건 이동
	switch (_direction)
	{
	case TANKDIRECTION_LEFT:
		ANIMATIONMANAGER->addAnimation("playerLeft", "player", 4, 5, 4, false, true);
		_ani = ANIMATIONMANAGER->findAnimation("playerLeft");
		ANIMATIONMANAGER->resume("playerLeft");

		x -= moveSpeed;
		rcCollision = RectMakeCenter(x, y, _image->getFrameWidth(), _image->getFrameHeight());
		break;
	case TANKDIRECTION_RIGHT:
		ANIMATIONMANAGER->addAnimation("playerRight", "player", 6, 7, 4, false, true);
		_ani = ANIMATIONMANAGER->findAnimation("playerRight");
		ANIMATIONMANAGER->resume("playerRight");

		x += moveSpeed;
		rcCollision = RectMakeCenter(x, y, _image->getFrameWidth(), _image->getFrameHeight());
		break;
	case TANKDIRECTION_UP:
		ANIMATIONMANAGER->addAnimation("playerUp", "player", 2, 3, 4, false, true);
		_ani = ANIMATIONMANAGER->findAnimation("playerUp");
		ANIMATIONMANAGER->resume("playerUp");

		y -= moveSpeed;
		rcCollision = RectMakeCenter(x, y, _image->getFrameWidth(), _image->getFrameHeight());
		break;
	case TANKDIRECTION_DOWN:
		ANIMATIONMANAGER->addAnimation("playerDown", "player", 0, 1, 4, false, true);
		_ani = ANIMATIONMANAGER->findAnimation("playerDown");
		ANIMATIONMANAGER->resume("playerDown");

		y += moveSpeed;
		rcCollision = RectMakeCenter(x, y, _image->getFrameWidth(), _image->getFrameHeight());
		break;
	}//end of switch(_direction)

	//땅크가 지금 밟고 있는 타일 번호를 알아오자.
	tileX = rcCollision.left / TILESIZE;
	tileY = rcCollision.top / TILESIZE;

	//땅크가 타일 어디에 있는지 확인이 가능하므로
	//땅크 방향에 따라서 그 앞의 타일 번호를 계산한다.
	//인덱스 번호가 2개인 이유는 렉트의 레프트, 탑 위치로 땅크의 위치를 구하고
	//렉트의 레프트, 탑이 타일번호에 걸쳐져 있을수도, 딱 맞게 있을수도 있기 때문에
	//걸쳐져 있을경우 걸친 그 밑의 타일의 값(ex : 오른쪽일 경우 땅크 위치 밑에 있는 값 앞에 있는 타일)을 가져온다.
	//그래서 정확하게 걸치지 않았을때 통과 할수 있도록..

	switch (_direction)
	{
	case TANKDIRECTION_LEFT:
		tileIndex[0] = tileX + tileY * TILEX;
		tileIndex[1] = tileX + (tileY + 1) * TILEX;
		break;
	case TANKDIRECTION_RIGHT:
		tileIndex[0] = (tileX + tileY * TILEX) + 1;
		tileIndex[1] = (tileX + (tileY + 1) * TILEX) + 1;
		break;
	case TANKDIRECTION_UP:
		tileIndex[0] = tileX + tileY * TILEX;
		tileIndex[1] = tileX + 1 + tileY * TILEX;
		break;
	case TANKDIRECTION_DOWN:
		tileIndex[0] = (tileX + tileY * TILEX) + TILEX;
		tileIndex[1] = (tileX + 1 + tileY * TILEX) + TILEX;
		break;
	}//end of switch(_direction)

	for (int i = 0; i < 2; i++)
	{
		RECT temp;

		//해당타일의 속성이 움직이지 못하는 곳이면....
		if (((_testMap->getAttribute()[tileIndex[i]] & ATTR_UNMOVABLE) == ATTR_UNMOVABLE) &&
			IntersectRect(&temp, &_testMap->getMap()[tileIndex[i]].rc, &rcCollision))
		{
			//움직이려 할때 갈수 없는 지역이면 탱크의 움직임을 고정하자
			//ex)땅크가 왼쪽으로갈때 왼쪽지역이 갈수 없으면
			switch (_direction)
			{
			case TANKDIRECTION_LEFT:
				_rc.left = _testMap->getMap()[tileIndex[i]].rc.right;	//왼쪽 타일의 라이트의 위치를 탱크의 래프트의 위치로 고정
				_rc.right = _rc.left + _image->getFrameWidth();			//탱크의 크기만큼
				x = _rc.left + (_rc.right - _rc.left) / 2;				//탱크 x의 위치를 계산한 탱크 랙트의 위치값으로 설정(중앙으로)
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

	//움직이자
	rcCollision = RectMakeCenter(x, y, _image->getFrameWidth(), _image->getFrameHeight());
	_rc = rcCollision;
}
