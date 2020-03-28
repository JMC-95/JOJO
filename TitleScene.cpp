#include "stdafx.h"
#include "TitleScene.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

HRESULT TitleScene::init()
{
	//BGM
	SOUNDMANAGER->stop("mapToolSound");
	SOUNDMANAGER->play("titleSound", 1.0f);

	listRect[0] = RectMake(237, 177, 173, 27);
	listRect[1] = RectMake(237, 207, 173, 27);
	listRect[2] = RectMake(237, 237, 173, 27);
	listRect[3] = RectMake(237, 267, 173, 27);

	return S_OK;
}

void TitleScene::release()
{
}

void TitleScene::update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		if (PtInRect(&listRect[0], m_ptMouse))
		{
			SCENEMANAGER->changeScene("LobbyScene");
		}
		else if (PtInRect(&listRect[1], m_ptMouse))
		{
			SCENEMANAGER->changeScene("MapTool");
		}
		else if (PtInRect(&listRect[2], m_ptMouse))
		{
			SCENEMANAGER->changeScene("GameScene");
		}
		else if (PtInRect(&listRect[3], m_ptMouse))
		{
			PostQuitMessage(0);
		}
	}
}

void TitleScene::render()
{
	IMAGEMANAGER->render("titleScene", getMemDC());
}
