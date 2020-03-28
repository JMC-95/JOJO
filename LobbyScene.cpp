#include "stdafx.h"
#include "LobbyScene.h"

LobbyScene::LobbyScene()
{
}

LobbyScene::~LobbyScene()
{
}

HRESULT LobbyScene::init()
{
	//BGM
	if (isSound)
	{
		SOUNDMANAGER->stop("titleSound");
		SOUNDMANAGER->play("lobbySound", 1.0f);
	}
	else
	{
		SOUNDMANAGER->stop("lobbySound");
	}

	menuRect[0] = RectMake(441, 411, 50, 50);
	menuRect[1] = RectMake(491, 411, 50, 50);
	menuRect[2] = RectMake(541, 411, 50, 50);
	menuRect[3] = RectMake(591, 411, 50, 50);

	return S_OK;
}

void LobbyScene::release()
{
}

void LobbyScene::update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		if (PtInRect(&menuRect[0], m_ptMouse))
		{
			SCENEMANAGER->changeScene("GameScene");
		}
		else if (PtInRect(&menuRect[0], m_ptMouse))
		{

		}
		else if (PtInRect(&menuRect[0], m_ptMouse))
		{

		}
		else if (PtInRect(&menuRect[0], m_ptMouse))
		{

		}
	}
}

void LobbyScene::render()
{
	IMAGEMANAGER->render("lobbyScene", getMemDC());
}
