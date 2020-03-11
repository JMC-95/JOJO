#include "stdafx.h"
#include "testMap.h"

testMap::testMap()
{
}

testMap::~testMap()
{
}

HRESULT testMap::init()
{
	load();

	return S_OK;
}

void testMap::release()
{
}

void testMap::update()
{
}

void testMap::render()
{
	IMAGEMANAGER->render("map", getMemDC(), 0, 0);
	IMAGEMANAGER->render("subWin", getMemDC(), 960, 0);

	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		//지형과 오브젝트를 맵에서 보여준다.
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
}

void testMap::load()
{
	if (isOfnCheck)
	{
		file = CreateFile(fileName[2], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else
	{
		file = CreateFile("save/SaveMap_3.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	ReadFile(file, tiles, sizeof(tagTile) * TILE_X * TILE_Y, &read, NULL);
	CloseHandle(file);

	//속성 정의를 하자.
	memset(attribute, 0, sizeof(DWORD) * TILE_X * TILE_Y);

	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (tiles[i].obj == OBJ_MOUNTAIN)
		{
			attribute[i] |= ATTR_UNMOVABLE;
		}
		if (tiles[i].obj == OBJ_ROCKMOUNTAIN)
		{
			attribute[i] |= ATTR_UNMOVABLE;
		}
		if (tiles[i].obj == OBJ_CASTLEWALLS)
		{
			attribute[i] |= ATTR_UNMOVABLE;
		}
	}
}
