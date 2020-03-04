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

	//맵툴 버튼 RECT
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

	//지형과 오브젝트를 맵에서 보여준다.
	if (KEYMANAGER->isToggleKey(VK_F2))
	{
		for (int i = 0; i < TILEX * TILEY; i++)
		{
			IMAGEMANAGER->frameRender("tileMap", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].terrainFrameX, _tiles[i].terrainFrameY);

			if (_tiles[i].obj == OBJ_NONE)continue;

			IMAGEMANAGER->frameRender("tileMap", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].objFrameX, _tiles[i].objFrameY);
		}
	}

	//마우스와 맵의 타일이 충돌하면 그 타일의 속성을 보여준다.
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		if (PtInRect(&_tiles[i].rc, m_ptMouse))
		{
			IMAGEMANAGER->render("tileCheck", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top);
		}
	}

	//선택한 버튼을 알려준다.
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
	//버튼 RECT
	_btnTerrainDraw = RectMake(982, 503, 90, 45);
	_btnObjectDraw = RectMake(1100, 503, 90, 45);
	_btnEraser = RectMake(982, 613, 90, 45);
	_btnData = RectMake(1100, 613, 90, 45);
	_btnSave = RectMake(1010, 735, 135, 35);
	_btnLoad = RectMake(1010, 795, 135, 35);
	_btnExit = RectMake(1010, 850, 135, 35);

	_ctrSelect = CTRL_TERRAIN;

	//오른쪽 샘플 맵 셋팅
	for (int i = 0; i < SAMPLETILEY; i++)
	{
		for (int j = 0; j < SAMPLETILEX; j++)
		{
			_sampleTiles[i * SAMPLETILEX + j].terrainFrameX = j;
			_sampleTiles[i * SAMPLETILEX + j].terrainFrameY = i;

			//좌표값 설정
			SetRect(&_sampleTiles[i * SAMPLETILEX + j].rcTile,
				(WINSIZEX - IMAGEMANAGER->findImage("tileMap")->getWidth()) + j * TILESIZE,
				i * TILESIZE, (WINSIZEX - IMAGEMANAGER->findImage("tileMap")->getWidth()) + j * TILESIZE + TILESIZE,
				i * TILESIZE + TILESIZE);
		}
	}

	//왼쪽 맵 셋팅
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

	//처음 깔려있는 타일
	for (int i = 0; i < TILEX * TILEY; i++)
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

		ZeroMemory(&ofn, sizeof(OPENFILENAME));

		ofn.lStructSize = sizeof(OPENFILENAME);		//구조체 사이즈 지정
		ofn.hwndOwner = NULL;						//대화상자 소유자를 지정
		ofn.lpstrFile = filePathSize;				//파일이름 에디트에 처음 나타낼 파일명을 지정. 충분한 버퍼가 있어야 한다.
		ofn.nMaxFile = sizeof(filePathSize);
		ofn.nFilterIndex = true;					//파일 형식 콤보 박스에서 사용할 필터의 인덱스를 지정
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = NULL;
		ofn.lpstrInitialDir = NULL;
		ofn.lpstrFilter = ("SaveData");				//이름(파일형식)
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn) == FALSE)return;
	}
	if (PtInRect(&_btnSave, m_ptMouse))
	{
		_ctrSelect = CTRL_SAVE;

		Save();
	}
	if (PtInRect(&_btnLoad, m_ptMouse))
	{
		_ctrSelect = CTRL_LOAD;
		isOfnCheck = true;

		Load();
	}
	if (PtInRect(&_btnExit, m_ptMouse))
	{
		SCENEMANAGER->changeScene("GameScene");
	}
}

void MapTool::SetMap()
{
	//오른쪽 샘플
	for (int i = 0; i < SAMPLETILEX * SAMPLETILEY; i++)
	{
		if (PtInRect(&_sampleTiles[i].rcTile, m_ptMouse))
		{
			_currentTile.x = _sampleTiles[i].terrainFrameX;
			_currentTile.y = _sampleTiles[i].terrainFrameY;
			break;
		}
	}

	//왼쪽 타일
	for (int i = 0; i < TILEX * TILEY; i++)
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
	file = CreateFile(fileName[2], GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &write, NULL);
	CloseHandle(file);
}

void MapTool::Load()
{
	file = CreateFile(fileName[2], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	CloseHandle(file);
}

TERRAIN MapTool::terrainSelect(int frameX, int frameY)
{
	//황무지
	if (frameX == 1 && frameY == 5)
	{
		return TR_BADLANDS;
	}
	//성내
	else if (frameX == 1 && frameY == 8)
	{
		return TR_CASTLERAND;
	}
	//성문
	else if (frameX == 2 && frameY == 9)
	{
		return TR_CASTLEGATE;
	}
	//병영
	else if (frameX == 0 && frameY == 3)
	{
		return TR_BARRACKS;
	}
}

OBJECT MapTool::objSelect(int frameX, int frameY)
{
	//산지
	if (frameX == 2 && frameY == 5)
	{
		return OBJ_MOUNTAIN;
	}
	//성벽
	else if (frameX == 0 && frameY == 8)
	{
		return OBJ_CASTLEWALLS;
	}
	//바위산
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
