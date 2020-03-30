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
	//BGM
	if (isSound)
	{
		SOUNDMANAGER->stop("titleSound");
		SOUNDMANAGER->play("mapToolSound", 1.0f);
	}
	else
	{
		SOUNDMANAGER->stop("mapToolSound");
	}

	mapToolSetup();

	currentTile.x = 0;
	currentTile.y = 0;

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

	//지형과 오브젝트를 맵에서 보여준다.
	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			IMAGEMANAGER->frameRender("tileMap", getMemDC(), tiles[i].rc.left, tiles[i].rc.top, tiles[i].terrainFrameX, tiles[i].terrainFrameY);

			if (tiles[i].obj == OBJ_NONE)continue;

			IMAGEMANAGER->frameRender("tileMap", getMemDC(), tiles[i].rc.left, tiles[i].rc.top, tiles[i].objFrameX, tiles[i].objFrameY);
		}
	}

	//마우스와 맵의 타일이 충돌하면 그 타일의 속성을 보여준다.
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&tiles[i].rc, m_ptMouse))
		{
			IMAGEMANAGER->render("tileCheck", getMemDC(), tiles[i].rc.left, tiles[i].rc.top);
		}
	}

	//선택한 버튼을 알려준다.
	switch (ctrSelect)
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

void MapTool::mapToolSetup()
{
	//버튼 RECT
	btnTerrainDraw = RectMake(982, 503, 90, 45);
	btnObjectDraw = RectMake(1100, 503, 90, 45);
	btnEraser = RectMake(982, 613, 90, 45);
	btnData = RectMake(1100, 613, 90, 45);
	btnSave = RectMake(1010, 735, 135, 35);
	btnLoad = RectMake(1010, 795, 135, 35);
	btnExit = RectMake(1010, 850, 135, 35);

	ctrSelect = CTRL_TERRAIN;

	//오른쪽 샘플 맵 셋팅
	for (int i = 0; i < SAMPLE_TILE_Y; i++)
	{
		for (int j = 0; j < SAMPLE_TILE_X; j++)
		{
			sampleTiles[i * SAMPLE_TILE_X + j].sTileFrameX = j;
			sampleTiles[i * SAMPLE_TILE_X + j].sTileFrameY = i;

			//좌표값 설정
			SetRect(&sampleTiles[i * SAMPLE_TILE_X + j].rcTile,
				(WINSIZEX - IMAGEMANAGER->findImage("tileMap")->getWidth()) + j * TILE_WIDTH,
				i * TILE_HEIGHT, (WINSIZEX - IMAGEMANAGER->findImage("tileMap")->getWidth()) + j * TILE_WIDTH + TILE_WIDTH,
				i * TILE_HEIGHT + TILE_HEIGHT);
		}
	}

	//왼쪽 맵 셋팅
	for (int i = 0; i < TILE_Y; i++)
	{
		for (int j = 0; j < TILE_X; j++)
		{
			SetRect(&tiles[i * TILE_X + j].rc,
				j * TILE_WIDTH,
				i * TILE_HEIGHT,
				j * TILE_WIDTH + TILE_WIDTH,
				i * TILE_HEIGHT + TILE_HEIGHT);
		}
	}

	//처음 깔려있는 타일
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		tiles[i].terrainFrameX = 3;
		tiles[i].terrainFrameY = 0;

		tiles[i].objFrameX = 3;
		tiles[i].objFrameY = 0;

		tiles[i].terrain = terrainSelect(tiles[i].terrainFrameX, tiles[i].terrainFrameY);
		tiles[i].obj = OBJ_NONE;
	}
}

void MapTool::mapToolButton()
{
	if (PtInRect(&btnTerrainDraw, m_ptMouse))
	{
		ctrSelect = CTRL_TERRAIN;
	}
	if (PtInRect(&btnObjectDraw, m_ptMouse))
	{
		ctrSelect = CTRL_OBJECT;
	}
	if (PtInRect(&btnEraser, m_ptMouse))
	{
		ctrSelect = CTRL_ERASER;
	}
	if (PtInRect(&btnData, m_ptMouse))
	{
		ctrSelect = CTRL_DATA;

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

		//isOfnCheck = true;
	}
	if (PtInRect(&btnSave, m_ptMouse))
	{
		ctrSelect = CTRL_SAVE;
		save();
	}
	if (PtInRect(&btnLoad, m_ptMouse))
	{
		ctrSelect = CTRL_LOAD;
		load();
	}
	if (PtInRect(&btnExit, m_ptMouse))
	{
		SCENEMANAGER->changeScene("TitleScene");
	}
}

void MapTool::setMap()
{
	//오른쪽 샘플
	for (int i = 0; i < SAMPLE_TILE_X * SAMPLE_TILE_Y; i++)
	{
		if (PtInRect(&sampleTiles[i].rcTile, m_ptMouse))
		{
			currentTile.x = sampleTiles[i].sTileFrameX;
			currentTile.y = sampleTiles[i].sTileFrameY;
			break;
		}
	}

	//왼쪽 타일
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&tiles[i].rc, m_ptMouse))
		{
			if (ctrSelect == CTRL_TERRAIN)
			{
				tiles[i].terrainFrameX = currentTile.x;
				tiles[i].terrainFrameY = currentTile.y;

				tiles[i].terrain = terrainSelect(currentTile.x, currentTile.y);
			}
			else if (ctrSelect == CTRL_OBJECT)
			{
				tiles[i].objFrameX = currentTile.x;
				tiles[i].objFrameY = currentTile.y;

				tiles[i].obj = objSelect(currentTile.x, currentTile.y);
			}
			else if (ctrSelect == CTRL_ERASER)
			{
				tiles[i].objFrameX = 0;
				tiles[i].objFrameY = 0;
				tiles[i].obj = OBJ_NONE;
			}

			InvalidateRect(m_hWnd, NULL, false);
			break;
		}
	}

	mapToolButton();
}

void MapTool::save()
{
	file = CreateFile(fileName[2], GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(file, tiles, sizeof(tagTile) * TILE_X * TILE_Y, &write, NULL);
	CloseHandle(file);
}

void MapTool::load()
{
	file = CreateFile(fileName[2], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, tiles, sizeof(tagTile) * TILE_X * TILE_Y, &read, NULL);
	CloseHandle(file);
}

TERRAIN MapTool::terrainSelect(int frameX, int frameY)
{
	//병영
	if (frameX == 0 && frameY == 3)
	{
		return TR_BARRACKS;
	}
	//평지
	else if (frameX == 0 && frameY == 5)
	{
		return TR_PLAIN;
	}
	//황무지
	else if (frameX == 1 && frameY == 5)
	{
		return TR_BADLANDS;
	}
	//성내
	else if (frameX == 1 && frameY == 8)
	{
		return TR_CASTLERAND;
	}
}

OBJECT MapTool::objSelect(int frameX, int frameY)
{
	//성벽
	if (frameX == 0 && frameY == 8)
	{
		return OBJ_CASTLEWALLS;
	}
	//바위산
	else if (frameX == 1 && frameY == 7)
	{
		return OBJ_ROCKMOUNTAIN;
	}
	//산지
	else if (frameX == 2 && frameY == 5)
	{
		return OBJ_MOUNTAIN;
	}
	//성문
	else if (frameX == 2 && frameY == 9)
	{
		return OBJ_CASTLEGATE;
	}
}

LRESULT MapTool::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_LBUTTONDOWN:
		SOUNDMANAGER->play("click", 1.0f);
		this->setMap();
		break;
	case WM_MOUSEMOVE:
		m_ptMouse.x = LOWORD(lParam);
		m_ptMouse.y = HIWORD(lParam);
		if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))this->setMap();
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
