#include "stdafx.h"
#include "MapTool.h"

MapTool::MapTool()
{
}

MapTool::~MapTool()
{
}

HRESULT MapTool::init()
{
	//������ �̹���
	IMAGEMANAGER->addImage("map", "images/MapTool/Map.bmp", 960, 960, true, RGB(255, 0, 255));				//���� ���� �̹���
	IMAGEMANAGER->addImage("subMap", "images/MapTool/SubMap.bmp", 240, 960, true, RGB(255, 0, 255));		//���� �̹���
	IMAGEMANAGER->addImage("select", "images/MapTool/Select.bmp", 30, 30, true, RGB(255, 0, 255));			//���� ���� �̹���
	IMAGEMANAGER->addImage("tileCheck", "images/UI/InGame/TileCheck.bmp", 48, 48, true, RGB(247, 0, 255));	//Ÿ�� üũ �̹���

	//���� Ÿ�� �̹���
	IMAGEMANAGER->addFrameImage("tileMap", "images/MapTool/TileMap.bmp", 144, 480, SAMPLETILEX, SAMPLETILEY, true, RGB(255, 0, 255));

	MapToolSetup();

	_currentTile.x = 0;
	_currentTile.y = 0;

	return S_OK;
}

void MapTool::release()
{
}

void MapTool::update()
{
}

void MapTool::render()
{
	IMAGEMANAGER->render("map", getMemDC(), 0, 0);
	IMAGEMANAGER->render("subMap", getMemDC(), 960, 0);

	//���� ��ư RECT
	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		Rectangle(getMemDC(), _btnTerrainDraw.left, _btnTerrainDraw.top, _btnTerrainDraw.right, _btnTerrainDraw.bottom);
		Rectangle(getMemDC(), _btnObjectDraw.left, _btnObjectDraw.top, _btnObjectDraw.right, _btnObjectDraw.bottom);
		Rectangle(getMemDC(), _btnEraser.left, _btnEraser.top, _btnEraser.right, _btnEraser.bottom);
		Rectangle(getMemDC(), _btnData.left, _btnData.top, _btnData.right, _btnData.bottom);
		Rectangle(getMemDC(), _btnSave.left, _btnSave.top, _btnSave.right, _btnSave.bottom);
		Rectangle(getMemDC(), _btnLoad.left, _btnLoad.top, _btnLoad.right, _btnLoad.bottom);
		Rectangle(getMemDC(), _btnExit.left, _btnExit.top, _btnExit.right, _btnExit.bottom);
	}

	//������ ������Ʈ�� �ʿ��� �����ش�.
	if (KEYMANAGER->isToggleKey(VK_F2))
	{
		for (int i = 0; i < TILEX * TILEY; i++)
		{
			IMAGEMANAGER->frameRender("tileMap", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].terrainFrameX, _tiles[i].terrainFrameY);

			if (_tiles[i].obj == OBJ_NONE)continue;

			IMAGEMANAGER->frameRender("tileMap", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].objFrameX, _tiles[i].objFrameY);
		}
	}

	//���콺�� ���� Ÿ���� �浹�ϸ� �� Ÿ���� �Ӽ��� �����ش�.
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		if (PtInRect(&_tiles[i].rc, m_ptMouse))
		{
			IMAGEMANAGER->render("tileCheck", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top);
		}
	}

	//������ ��ư�� �˷��ش�.
	switch (_ctrSelect)
	{
	case CTRL_TERRAIN:
		IMAGEMANAGER->render("select", getMemDC(), 1052, 532);
		break;
	case CTRL_OBJECT:
		IMAGEMANAGER->render("select", getMemDC(), 1172, 532);
		break;
	case CTRL_ERASER:
		IMAGEMANAGER->render("select", getMemDC(), 1052, 642);
		break;
	case CTRL_DATA:
		IMAGEMANAGER->render("select", getMemDC(), 1172, 642);
		break;
	case CTRL_SAVE:
		IMAGEMANAGER->render("select", getMemDC(), 1128, 760);
		break;
	case CTRL_LOAD:
		IMAGEMANAGER->render("select", getMemDC(), 1125, 817);
		break;
	}
}

void MapTool::MapToolSetup()
{
	//��ư RECT
	_btnTerrainDraw = RectMake(982, 503, 90, 45);
	_btnObjectDraw = RectMake(1100, 503, 90, 45);
	_btnEraser = RectMake(982, 613, 90, 45);
	_btnData = RectMake(1100, 613, 90, 45);
	_btnSave = RectMake(1010, 735, 135, 35);
	_btnLoad = RectMake(1010, 795, 135, 35);
	_btnExit = RectMake(1010, 850, 135, 35);

	_ctrSelect = CTRL_TERRAIN;

	//������ ���� �� ����
	for (int i = 0; i < SAMPLETILEY; i++)
	{
		for (int j = 0; j < SAMPLETILEX; j++)
		{
			_sampleTiles[i * SAMPLETILEX + j].terrainFrameX = j;
			_sampleTiles[i * SAMPLETILEX + j].terrainFrameY = i;

			//��ǥ�� ����
			SetRect(&_sampleTiles[i * SAMPLETILEX + j].rcTile,
				(WINSIZEX - IMAGEMANAGER->findImage("tileMap")->getWidth()) + j * TILESIZE,
				i * TILESIZE, (WINSIZEX - IMAGEMANAGER->findImage("tileMap")->getWidth()) + j * TILESIZE + TILESIZE,
				i * TILESIZE + TILESIZE);
		}
	}

	//���� �� ����
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			SetRect(&_tiles[i * TILEX + j].rc,
				j * TILESIZE,
				i * TILESIZE,
				j * TILESIZE + TILESIZE,
				i * TILESIZE + TILESIZE);
		}
	}

	//ó�� ����ִ� Ÿ��
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		_tiles[i].terrainFrameX = 3;
		_tiles[i].terrainFrameY = 0;

		_tiles[i].objFrameX = 3;
		_tiles[i].objFrameY = 0;

		_tiles[i].terrain = terrainSelect(_tiles[i].terrainFrameX, _tiles[i].terrainFrameY);
		_tiles[i].obj = OBJ_NONE;
	}
}

void MapTool::MapButton()
{
	if (PtInRect(&_btnTerrainDraw, m_ptMouse))
	{
		_ctrSelect = CTRL_TERRAIN;
	}
	if (PtInRect(&_btnObjectDraw, m_ptMouse))
	{
		_ctrSelect = CTRL_OBJECT;
	}
	if (PtInRect(&_btnEraser, m_ptMouse))
	{
		_ctrSelect = CTRL_ERASER;
	}
	if (PtInRect(&_btnData, m_ptMouse))
	{
		_ctrSelect = CTRL_DATA;

		OPENFILENAME ofn;

		char filePathSize[1024] = "";
		char temp[1024];
		char* context = NULL;
		char* token = strtok_s(temp, "\\", &context);

		ZeroMemory(&ofn, sizeof(OPENFILENAME));

		ofn.lStructSize = sizeof(OPENFILENAME);		//����ü ������ ����
		ofn.hwndOwner = NULL;						//��ȭ���� �����ڸ� ����
		ofn.lpstrFile = filePathSize;				//�����̸� ����Ʈ�� ó�� ��Ÿ�� ���ϸ��� ����. ����� ���۰� �־�� �Ѵ�.
		ofn.nMaxFile = sizeof(filePathSize);
		ofn.nFilterIndex = true;					//���� ���� �޺� �ڽ����� ����� ������ �ε����� ����
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = NULL;
		ofn.lpstrInitialDir = NULL;
		ofn.lpstrFilter = ("Data");					//�̸�(��������)
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn) == FALSE)return;

		strncpy_s(temp, strlen(ofn.lpstrFile) + 1, ofn.lpstrFile, strlen(ofn.lpstrFile));

		while (strlen(context))
		{
			token = strtok_s(NULL, "\\", &context);
		}
	}
	if (PtInRect(&_btnSave, m_ptMouse))
	{
		_ctrSelect = CTRL_SAVE;
		Save();
	}
	if (PtInRect(&_btnLoad, m_ptMouse))
	{
		_ctrSelect = CTRL_LOAD;
		Load();
	}
	if (PtInRect(&_btnExit, m_ptMouse))
	{
		PostQuitMessage(0);
	}
}

void MapTool::SetMap()
{
	//������ ����
	for (int i = 0; i < SAMPLETILEX * SAMPLETILEY; i++)
	{
		if (PtInRect(&_sampleTiles[i].rcTile, m_ptMouse))
		{
			_currentTile.x = _sampleTiles[i].terrainFrameX;
			_currentTile.y = _sampleTiles[i].terrainFrameY;
			break;
		}
	}

	//���� Ÿ��
	for (int i = 0; i < TILEX* TILEY; i++)
	{
		if (PtInRect(&_tiles[i].rc, m_ptMouse))
		{
			if (_ctrSelect == CTRL_TERRAIN)
			{
				_tiles[i].terrainFrameX = _currentTile.x;
				_tiles[i].terrainFrameY = _currentTile.y;

				_tiles[i].terrain = terrainSelect(_currentTile.x, _currentTile.y);
			}
			else if (_ctrSelect == CTRL_OBJECT)
			{
				_tiles[i].objFrameX = _currentTile.x;
				_tiles[i].objFrameY = _currentTile.y;

				_tiles[i].obj = objSelect(_currentTile.x, _currentTile.y);
			}
			else if (_ctrSelect == CTRL_ERASER)
			{
				_tiles[i].objFrameX = 0;
				_tiles[i].objFrameY = 0;
				_tiles[i].obj = OBJ_NONE;
			}

			InvalidateRect(m_hWnd, NULL, false);
			break;
		}
	}

	MapButton();
}

void MapTool::Save()
{
	file = CreateFile
	(fileName[0],				//������ ���϶Ǵ� �� ��ġ�� �����̸�
		GENERIC_WRITE,			//�����̳� ��ġ�� ����ų� ���� ����� ����
		0,						//���� ���� ����Է�
		NULL,					//���϶Ǵ� ��ġ�� ���� ���� �� Ư��
		CREATE_ALWAYS,			//�����̳� ��ġ�� ���� ���� �ൿ
		FILE_ATTRIBUTE_NORMAL,	//�����̳� ��ġ�� ���� ���� �� Ư��
		NULL);					//������� ������ ���� �� Ư�� Ȯ�� Ư���� ���� ����

	WriteFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &write, NULL);
	CloseHandle(file);
}

void MapTool::Load()
{
	file = CreateFile
	(fileName[0],				//������ ���϶Ǵ� �� ��ġ�� �����̸�
		GENERIC_READ,			//�����̳� ��ġ�� ����ų� ���� ����� ����
		0,						//���� ���� ����Է�
		NULL,					//���϶Ǵ� ��ġ�� ���� ���� �� Ư��
		OPEN_EXISTING,			//�����̳� ��ġ�� ���� ���� �ൿ
		FILE_ATTRIBUTE_NORMAL,	//�����̳� ��ġ�� ���� ���� �� Ư��
		NULL);					//������� ������ ���� �� Ư�� Ȯ�� Ư���� ���� ����

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	CloseHandle(file);
}

TERRAIN MapTool::terrainSelect(int frameX, int frameY)
{
	//Ȳ����
	if (frameX == 1 && frameY == 5)
	{
		return TR_BADLANDS;
	}
	//����
	else if (frameX == 1 && frameY == 8)
	{
		return TR_CASTLERAND;
	}
	//����
	else if (frameX == 2 && frameY == 9)
	{
		return TR_CASTLEGATE;
	}
	//����
	else if (frameX == 0 && frameY == 3)
	{
		return TR_BARRACKS;
	}
}

OBJECT MapTool::objSelect(int frameX, int frameY)
{
	//����
	if (frameX == 2 && frameY == 5)
	{
		return OBJ_MOUNTAIN;
	}
	//����
	else if (frameX == 0 && frameY == 8)
	{
		return OBJ_CASTLEWALLS;
	}
	//������
	else if (frameX == 1 && frameY == 7)
	{
		return OBJ_ROCKMOUNTAIN;
	}
}

LRESULT MapTool::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_LBUTTONDOWN:
		this->SetMap();
		break;
	case WM_MOUSEMOVE:
		m_ptMouse.x = LOWORD(lParam);
		m_ptMouse.y = HIWORD(lParam);
		if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))this->SetMap();
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}//end of switch

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
