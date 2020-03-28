#include "stdafx.h"
#include "PreferencesScene.h"

PreferencesScene::PreferencesScene()
{
}

PreferencesScene::~PreferencesScene()
{
}

HRESULT PreferencesScene::init()
{
	rc[0] = RectMake(193, 134, 12, 12);
	rc[1] = RectMake(232, 373, 75, 21);
	rc[2] = RectMake(335, 373, 75, 21);

	return S_OK;
}

void PreferencesScene::release()
{
}

void PreferencesScene::update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		if (PtInRect(&rc[0], m_ptMouse))
		{
			if (isSound) bgmCheck = false;
			else if (!isSound) bgmCheck = true;
		}
		else if (PtInRect(&rc[1], m_ptMouse))
		{
			if (!bgmCheck) isSound = false;
			else if (bgmCheck) isSound = true;

			SCENEMANAGER->changeScene("TitleScene");
		}
		else if (PtInRect(&rc[2], m_ptMouse))
		{
			SCENEMANAGER->changeScene("TitleScene");
		}
	}
}

void PreferencesScene::render()
{
	IMAGEMANAGER->render("preferences", getMemDC());

	if (!bgmCheck && KEYMANAGER->isToggleKey(VK_LBUTTON))
	{
		IMAGEMANAGER->render("check", getMemDC(), 195, 136);
	}
}
