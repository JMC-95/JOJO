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
	//��ũ ���⼳��
	_direction = TANKDIRECTION_LEFT;

	_image = IMAGEMANAGER->findImage(imageName);
	ANIMATIONMANAGER->addAnimation("playerLeft", "player", 4, 5, 5, false, true);
	_ani = ANIMATIONMANAGER->findAnimation("playerLeft");
	ANIMATIONMANAGER->start("playerLeft");

	//�ӵ�
	speed = 6;

	return S_OK;
}

void tank::release()
{
}

void tank::update()
{
	mouseClick();
	animation();
}

void tank::render()
{
	if (isTurn)
	{
		IMAGEMANAGER->render("range", getMemDC(), _rc.left - 48, _rc.top - 48);
	}

	_image->aniRender(getMemDC(), _rc.left, _rc.top, _ani);

	char strBlock[128];
	sprintf_s(strBlock, "X : %d�� Ÿ��, Y : %d�� Ÿ��", tileX, tileY);
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
			if (!PtInRect(&_rc, m_ptMouse) && PtInRect(&_testMap->getMap()[i].rc, m_ptMouse) && isTurn)
			{
				mapX = _testMap->getMap()[i].rc.left + (_testMap->getMap()[i].rc.right - _testMap->getMap()[i].rc.left) / 2;
				mapY = _testMap->getMap()[i].rc.top + (_testMap->getMap()[i].rc.bottom - _testMap->getMap()[i].rc.top) / 2;

				if (x > mapX)
				{
					_direction = TANKDIRECTION_LEFT;
				}
				if (x < mapX)
				{
					_direction = TANKDIRECTION_RIGHT;
				}
				if (y > mapY)
				{
					_direction = TANKDIRECTION_UP;
				}
				if (y < mapY)
				{
					_direction = TANKDIRECTION_DOWN;
				}

				isTurn = false;
				isMove = true;
			}
		}
	}

	if (isMove)
	{
		tankMove();
	}
}

void tank::tankMove()
{
	RECT rcCollision;
	int tileIndex[2];	//�˻�� Ÿ��

	rcCollision = _rc;	//������ ��Ʈ

	//�ϴ� ������ �̵�
	switch (_direction)
	{
	case TANKDIRECTION_LEFT:
		if (_rc.left > 0)
		{
			x -= speed;
			rcCollision = RectMakeCenter(x, y, _image->getFrameWidth(), _image->getFrameHeight());
		}
		if (x == mapX)
		{
			isMove = false;
		}
		break;
	case TANKDIRECTION_RIGHT:
		if (_rc.right < WINSIZEY)
		{
			x += speed;
			rcCollision = RectMakeCenter(x, y, _image->getFrameWidth(), _image->getFrameHeight());
		}
		if (x == mapX)
		{
			isMove = false;
		}
		break;
	case TANKDIRECTION_UP:
		if (_rc.top > 0)
		{
			y -= speed;
			rcCollision = RectMakeCenter(x, y, _image->getFrameWidth(), _image->getFrameHeight());
		}
		if (y == mapY)
		{
			isMove = false;
		}
		break;
	case TANKDIRECTION_DOWN:
		if (_rc.bottom < WINSIZEY)
		{
			y += speed;
			rcCollision = RectMakeCenter(x, y, _image->getFrameWidth(), _image->getFrameHeight());
		}
		if (y == mapY)
		{
			isMove = false;
		}
		break;
	}//end of switch(_direction)

	//��ũ�� ���� ��� �ִ� Ÿ�� ��ȣ�� �˾ƿ���.
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

		//�ش�Ÿ���� �Ӽ��� �������� ���ϴ� ���̸�....
		if (((_testMap->getAttribute()[tileIndex[i]] & ATTR_UNMOVABLE) == ATTR_UNMOVABLE) &&
			IntersectRect(&temp, &_testMap->getMap()[tileIndex[i]].rc, &rcCollision))
		{
			//�����̷� �Ҷ� ���� ���� �����̸� ��ũ�� �������� ��������
			//ex)��ũ�� �������ΰ��� ���������� ���� ������
			switch (_direction)
			{
			case TANKDIRECTION_LEFT:
				_rc.left = _testMap->getMap()[tileIndex[i]].rc.right;	//���� Ÿ���� ����Ʈ�� ��ġ�� ��ũ�� ����Ʈ�� ��ġ�� ����
				_rc.right = _rc.left + _image->getFrameWidth();			//��ũ�� ũ�⸸ŭ
				x = _rc.left + (_rc.right - _rc.left) / 2;				//��ũ x�� ��ġ�� ����� ��ũ ��Ʈ�� ��ġ������ ����(�߾�����)
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

	//��������
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
