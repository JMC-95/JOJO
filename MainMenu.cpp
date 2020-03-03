#include "stdafx.h"
#include "MainMenu.h"

MainMenu::MainMenu()
{
}

MainMenu::~MainMenu()
{
}

HRESULT MainMenu::init()
{
	// 사운드
	SOUNDMANAGER->stop("title");
	SOUNDMANAGER->stop("BGM");
	SOUNDMANAGER->stop("BossBGM");
	SOUNDMANAGER->play("main", 1.0f);

	IMAGEMANAGER->addImage("MainMenu", "images/UI/menu/MainMenu.bmp", WINSIZEX / 2, WINSIZEY / 2, 884, 572, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("MainMenuArrow", "images/UI/menu/MainMenuArrow.bmp", WINSIZEY / 2, WINSIZEY / 2, 24, 30, true, RGB(255, 0, 255));

	newRunRect = RectMake(WINSIZEX / 2 - 145, WINSIZEY / 2 - 100, 24, 30);
	mapToolRect = RectMake(WINSIZEX / 2 - 130, WINSIZEY / 2 - 5, 24, 30);
	optionRect = RectMake(WINSIZEX / 2 - 110, WINSIZEY / 2 + 85, 24, 30);
	cursorRect = RectMake(WINSIZEX / 2 - 145, WINSIZEY / 2 - 100, 24, 30);

	return S_OK;
}

void MainMenu::release()
{
}

void MainMenu::update()
{
	CursorMove();
}

void MainMenu::render()
{
	IMAGEMANAGER->render("MainMenu", getMemDC());
	if (KEYMANAGER->isToggleKey(VK_TAB))
	{
		Rectangle(getMemDC(), newRunRect.left, newRunRect.top, newRunRect.right, newRunRect.bottom);
		Rectangle(getMemDC(), mapToolRect.left, mapToolRect.top, mapToolRect.right, mapToolRect.bottom);
		Rectangle(getMemDC(), optionRect.left, optionRect.top, optionRect.right, optionRect.bottom);
	}
	IMAGEMANAGER->render("MainMenuArrow", getMemDC(), cursorRect.left, cursorRect.top);
}

void MainMenu::CursorMove()
{
	// 새 게임 선택시 게임 화면으로 전환한다.
	if (IntersectRect(&temp, &newRunRect, &cursorRect))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
		{
			cursorRect = RectMake(WINSIZEX / 2 - 130, WINSIZEY / 2 - 5, 24, 30);
		}
		if (KEYMANAGER->isOnceKeyDown(VK_RETURN) || KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			SCENEMANAGER->changeScene("CharacterSelect");
		}
	}
	// 맵툴 선택시 맵툴 화면으로 전환한다.
	if (IntersectRect(&temp, &mapToolRect, &cursorRect))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_UP))
		{
			cursorRect = RectMake(WINSIZEX / 2 - 145, WINSIZEY / 2 - 100, 24, 30);
		}
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
		{
			cursorRect = RectMake(WINSIZEX / 2 - 110, WINSIZEY / 2 + 85, 24, 30);
		}
		if (KEYMANAGER->isOnceKeyDown(VK_RETURN) || KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			SCENEMANAGER->changeScene("MapTool");
		}
	}
	// 옵션 선택시 옵션 화면으로 전환한다.
	if (IntersectRect(&temp, &optionRect, &cursorRect))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_UP))
		{
			cursorRect = RectMake(WINSIZEX / 2 - 130, WINSIZEY / 2 - 5, 24, 30);
		}
		if (KEYMANAGER->isOnceKeyDown(VK_RETURN) || KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			SCENEMANAGER->changeScene("GameScene");
		}
	}
}