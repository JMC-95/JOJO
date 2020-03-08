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
	ANIMATIONMANAGER->addAnimation("playerDown", "player", 0, 1, 4, false, true);
	_ani = ANIMATIONMANAGER->findAnimation("playerDown");

	//�ӵ�
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

	//ĳ���ʹ� ó������ ��ġ�� �����Ǿ� �ֱ� ������ ĳ������ ��ġ�� �̸� ������Ʈ�� �������ְ�,
	//������Ʈ�� �ƴ� �׶��� �� �� ������ �� �ְ� �Ѵ�.
	//������ �̸� ��Ƶ� ������Ʈ�� ��� ������ �� ������..?

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

	//Ÿ�� �Ŵ����� �̿��� ���
	float elpasedTime = TIMEMANAGER->getElapsedTime();
	float moveSpeed = elpasedTime * speed;

	//�ϴ� ������ �̵�
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

	//��ũ�� ���� ��� �ִ� Ÿ�� ��ȣ�� �˾ƿ���.
	tileX = rcCollision.left / TILESIZE;
	tileY = rcCollision.top / TILESIZE;

	//��ũ�� Ÿ�� ��� �ִ��� Ȯ���� �����ϹǷ�
	//��ũ ���⿡ ���� �� ���� Ÿ�� ��ȣ�� ����Ѵ�.
	//�ε��� ��ȣ�� 2���� ������ ��Ʈ�� ����Ʈ, ž ��ġ�� ��ũ�� ��ġ�� ���ϰ�
	//��Ʈ�� ����Ʈ, ž�� Ÿ�Ϲ�ȣ�� ������ ��������, �� �°� �������� �ֱ� ������
	//������ ������� ��ģ �� ���� Ÿ���� ��(ex : �������� ��� ��ũ ��ġ �ؿ� �ִ� �� �տ� �ִ� Ÿ��)�� �����´�.
	//�׷��� ��Ȯ�ϰ� ��ġ�� �ʾ����� ��� �Ҽ� �ֵ���..

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
