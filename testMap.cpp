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
	IMAGEMANAGER->render("subWin", getMemDC(), 960, 0);

	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		//������ ������Ʈ�� �ʿ��� �����ش�.
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
}

void testMap::Load()
{
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

	//ĳ���� ��ġ�� �ش��ϴ� Ÿ�� �̹����� �ɷ�����.
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		if (_tiles[i].obj == OBJ_CHARACTER)
		{
			_tiles[i].obj = OBJ_NONE;
		}
	}

	//�Ӽ� ���Ǹ� ����.
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
