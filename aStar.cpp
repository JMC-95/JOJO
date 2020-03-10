#include "stdafx.h"
#include "aStar.h"

aStar::aStar()
{
}

aStar::~aStar()
{
}

HRESULT aStar::init()
{
	Load();

	startTile = endTile = -1;

	currentSelect = SELECT_START;

	isFind = false;
	noPath = false;
	startAstar = false;

	//���ݿ�(��ġ)
	temp = 50;

	for (int i = 0; i < 5; i++)
	{
		rc[i] = RectMake(1000, 200 + i * 25 + i * 10 + temp * 2, 25, 25);
	}

	return S_OK;
}

void aStar::release()
{
}

void aStar::update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_TAB))
	{
		openList.clear();
		closeList.clear();
		SCENEMANAGER->changeScene("GameScene");
	}

	if (KEYMANAGER->isOnceKeyDown('1'))
	{
		currentSelect = SELECT_START;
	}
	if (KEYMANAGER->isOnceKeyDown('2'))
	{
		currentSelect = SELECT_END;
	}
	if (KEYMANAGER->isOnceKeyDown('3'))
	{
		if (startTile != -1 && endTile != -1)
		{
			startAstar = true;
			currentTile = startTile;

			//���������� ���¸���Ʈ�� ����
			openList.push_back(currentTile);
		}
	}
	if (KEYMANAGER->isOnceKeyDown('4'))
	{
		openList.clear();
		closeList.clear();
		init();
	}

	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		for (int i = 0; i < TILEX * TILEY; i++)
		{
			if (PtInRect(&_tiles[i].rc, m_ptMouse))
			{
				switch (currentSelect)
				{
				case SELECT_START:
					startTile = i;
					break;
				case SELECT_END:
					endTile = i;
					break;
				}
			}
		}
	}//end of VK_LBUTTON

	if (!isFind && !noPath && startAstar)
	{
		Astar();
	}
}

void aStar::render()
{
	IMAGEMANAGER->render("map", getMemDC(), 0, 0);
	IMAGEMANAGER->render("subWin", getMemDC(), 960, 0);

	//������ ������Ʈ�� �ʿ��� �����ش�.
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		IMAGEMANAGER->frameRender("tileMap", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].terrainFrameX, _tiles[i].terrainFrameY);

		if (_tiles[i].obj == OBJ_NONE)continue;

		IMAGEMANAGER->frameRender("tileMap", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].objFrameX, _tiles[i].objFrameY);
	}

	//���콺�� ���� Ÿ���� �浹�ϸ� �� Ÿ���� �Ӽ��� �����ش�.
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		if (PtInRect(&_tiles[i].rc, m_ptMouse))
		{
			IMAGEMANAGER->render("tileCheck", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top);
		}
	}

	for (int i = 0; i < TILEX * TILEY; i++)
	{
		if (i == startTile)
		{
			BeginSolidColor(getMemDC(), &brush, RGB(0, 255, 0));
			RectangleMake(getMemDC(), _tiles[i].rc);
		}
		else if (i == endTile)
		{
			BeginSolidColor(getMemDC(), &brush, RGB(255, 0, 0));
			RectangleMake(getMemDC(), _tiles[i].rc);
		}
		else if (_tiles[i].showState == STATE_OPEN)
		{
			BeginSolidColor(getMemDC(), &brush, RGB(180, 255, 180));
			//RectangleMake(getMemDC(), _tiles[i].rc);
		}
		else if (_tiles[i].showState == STATE_CLOSE)
		{
			BeginSolidColor(getMemDC(), &brush, RGB(180, 180, 255));
			//RectangleMake(getMemDC(), _tiles[i].rc);
		}
		else if (_tiles[i].showState == STATE_PATH)
		{
			BeginSolidColor(getMemDC(), &brush, RGB(255, 180, 180));
			RectangleMake(getMemDC(), _tiles[i].rc);
		}

		DeleteObject(brush);
	}//end of for

	SetTextColor(getMemDC(), RGB(0, 0, 255));

	sprintf_s(str, "1: Start");
	TextOut(getMemDC(), 1000, 100, str, strlen(str));
	sprintf_s(str, "2: End");
	TextOut(getMemDC(), 1000, 130, str, strlen(str));
	sprintf_s(str, "3: ����");
	TextOut(getMemDC(), 1000, 160, str, strlen(str));
	sprintf_s(str, "4: �ʱ�ȭ");
	TextOut(getMemDC(), 1000, 190, str, strlen(str));

	if (isFind)
	{
		sprintf_s(str, "��ã�� ����!");
		TextOut(getMemDC(), 1000, 200 + temp, str, strlen(str));
	}
	if (noPath)
	{
		sprintf_s(str, "��ã�� ����!");
		TextOut(getMemDC(), 1000, 200 + temp, str, strlen(str));
	}

	for (int i = 0; i < 5; i++)
	{
		if (i == 0)
		{
			BeginSolidColor(getMemDC(), &brush, RGB(0, 255, 0));
			RectangleMake(getMemDC(), rc[i]);
			sprintf_s(str, "������");
			DeleteObject(brush);
		}
		if (i == 1)
		{
			BeginSolidColor(getMemDC(), &brush, RGB(255, 0, 0));
			RectangleMake(getMemDC(), rc[i]);
			sprintf_s(str, "������");
			DeleteObject(brush);
		}
		if (i == 2)
		{
			BeginSolidColor(getMemDC(), &brush, RGB(180, 255, 180));
			RectangleMake(getMemDC(), rc[i]);
			sprintf_s(str, "OpenList");
			DeleteObject(brush);
		}
		if (i == 3)
		{
			BeginSolidColor(getMemDC(), &brush, RGB(180, 180, 255));
			RectangleMake(getMemDC(), rc[i]);
			sprintf_s(str, "CloseList");
			DeleteObject(brush);
		}
		if (i == 4)
		{
			BeginSolidColor(getMemDC(), &brush, RGB(255, 180, 180));
			RectangleMake(getMemDC(), rc[i]);
			sprintf_s(str, "Path");
			DeleteObject(brush);
		}

		TextOut(getMemDC(), rc[i].right + 15, rc[i].top + 5, str, strlen(str));
	}
}

void aStar::Astar()
{
	int endX = endTile % TILEX;
	int endY = endTile / TILEY;

	int currentX = currentTile % TILEX;
	int currentY = currentTile / TILEY;

	// left, right, up, down, leftup, rightup, leftdown, rightdown
	int dx[] = { -1, 1, 0, 0, -1, 1, -1, 1 };
	int dy[] = { 0, 0, -1, 1, -1, -1, 1, 1 };

	bool tempBlock[8];

	// ���� ã�� �ݺ���
	for (int i = 0; i < 8; i++)
	{
		int x = currentX + dx[i];
		int y = currentY + dy[i];
		tempBlock[i] = false;

		// �ش� �������� ������ Ÿ���� ��ȿ�� Ÿ������ Ȯ��
		if (0 <= x && x < TILEX && 0 <= y && y < TILEY)
		{
			bool isOpen;
			// �밢�� Ÿ���� �̵� ������ (�ֺ��� ��������� ����) �ӽ÷� ��� ���� ����
			if (_tiles[y * TILEX + x].obj == OBJ_MOUNTAIN ||
				_tiles[y * TILEX + x].obj == OBJ_ROCKMOUNTAIN ||
				_tiles[y * TILEX + x].obj == OBJ_CASTLEWALLS) tempBlock[i] = true;
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
					_tiles[y * TILEX + x].g = 10;
				}
				else
				{
					// leftup�� ��� left�� up�� ��������� �ȵ�
					if (i == DIRECTION_LEFTUP &&
						tempBlock[DIRECTION_LEFT] || tempBlock[DIRECTION_UP]) continue;
					// rightup�� ��� right�� up�� ��������� �ȵ�
					if (i == DIRECTION_RIGHTUP &&
						tempBlock[DIRECTION_RIGHT] || tempBlock[DIRECTION_UP]) continue;
					// leftdown�� ��� left�� down�� ��������� �ȵ�
					if (i == DIRECTION_LEFTDOWN &&
						tempBlock[DIRECTION_LEFT] || tempBlock[DIRECTION_DOWN]) continue;
					// rightdown�� ��� right�� down�� ��������� �ȵ�
					if (i == DIRECTION_RIGHTDOWN &&
						tempBlock[DIRECTION_RIGHT] || tempBlock[DIRECTION_DOWN]) continue;
					_tiles[y * TILEX + x].g = 14;
				}

				//abs���밪
				_tiles[y * TILEX + x].h = (abs(endX - x) + abs(endY - y)) * 10;
				_tiles[y * TILEX + x].f = _tiles[y * TILEX + x].g + _tiles[y * TILEX + x].h;

				// ���¸���Ʈ�� ������ g ��� �� �� ó��
				isOpen = false;
				for (int i = 0; i < openList.size(); i++)
				{
					if (openList[i] == y * TILEX + x)
					{
						isOpen = true;
						if (_tiles[openList[i]].g > _tiles[y * TILEX + x].g)
						{
							_tiles[openList[i]].h = _tiles[y * TILEX + x].h;
							_tiles[openList[i]].g = _tiles[y * TILEX + x].g;
							_tiles[openList[i]].f = _tiles[y * TILEX + x].f;
							_tiles[openList[i]].node = currentTile;
						}
					}
				}

				// ������ �׳� �ְ� �θ� ����
				if (!isOpen)
				{
					openList.push_back(y * TILEX + x);
					_tiles[y * TILEX + x].node = currentTile;
				}

				// find
				if (y * TILEX + x == endTile)
					isFind = true;
			}
		}
	}

	// ���� ���� ������Ͽ��� ����
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

	// ���� Ÿ�� Ŭ�����Ʈ�� �ֱ�
	closeList.push_back(currentTile);

	if (openList.size() != 0)
	{
		// find minimum f cost in openList
		int min = _tiles[*openList.begin()].h;

		currentTile = *openList.begin();

		for (iter = openList.begin(); iter != openList.end(); ++iter)
		{
			if (min > _tiles[(*iter)].h)
			{
				min = _tiles[(*iter)].h;
				currentTile = *iter;
			}
		}
	}

	// Ÿ�� ������ ���� ���� ����
	for (int i = 0; i < openList.size(); i++)
	{
		_tiles[openList[i]].showState = STATE_OPEN;
	}
	for (int i = 0; i < closeList.size(); i++)
	{
		_tiles[closeList[i]].showState = STATE_CLOSE;
	}

	// �� ã�� ������ �� Ÿ�Ͽ� ��ã�� ���� ����
	int tempTile = endTile;

	while (_tiles[tempTile].node != startTile && isFind)
	{
		tempTile = _tiles[tempTile].node;
		_tiles[tempTile].showState = STATE_PATH;
	}
}

void aStar::Load()
{
	HANDLE file;
	DWORD read;

	if (isOfnCheck)
	{
		file = CreateFile(fileName[2], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else
	{
		file = CreateFile("save/SaveMap_3.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	CloseHandle(file);
}
