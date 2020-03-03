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
	Load();

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

	if (KEYMANAGER->isToggleKey(VK_F2))
	{
		//지형
		for (int i = 0; i < TILEX*TILEY; i++)
		{
			IMAGEMANAGER->frameRender("tileMap", getMemDC(),
				_tiles[i].rc.left, _tiles[i].rc.top,
				_tiles[i].terrainFrameX, _tiles[i].terrainFrameY);
		}
		//오브젝트
		for (int i = 0; i < TILEX* TILEY; i++)
		{
			if (_tiles[i].obj == OBJ_NONE)continue;
			IMAGEMANAGER->frameRender("tileMap", getMemDC(), _tiles[i].rc.left,
				_tiles[i].rc.top, _tiles[i].objFrameX, _tiles[i].objFrameY);
		}
	}
}

void testMap::Load()
{
	if (isOfnCheck)
	{
		file = CreateFile(fileName[3], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else
	{
		file = CreateFile("save/saveMap4.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	CloseHandle(file);

	//속성정의를 하자.
	memset(_attribute, 0, sizeof(DWORD) * TILEX * TILEY);
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		if (_tiles[i].obj == OBJ_MOUNTAIN)
		{
			_attribute[i] |= ATTR_UNMOVABLE;
		}
		if (_tiles[i].obj == OBJ_ROCKMOUNTAIN)
		{
			_attribute[i] |= ATTR_UNMOVABLE;
		}
		if (_tiles[i].obj == OBJ_CASTLEWALLS)
		{
			_attribute[i] |= ATTR_UNMOVABLE;
		}
	}
}
