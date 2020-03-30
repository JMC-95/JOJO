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
	PLAYERMANAGER->init(); //로비에서 init을 하고 GameScene으로 넘겨준다.

	//BGM
	if (isSound)
	{
		SOUNDMANAGER->stop("titleSound");
		//SOUNDMANAGER->play("lobbySound", 1.0f);
	}
	else
	{
		SOUNDMANAGER->stop("lobbySound");
	}

	for (int i = 0; i < 4; i++)
	{
		menuRect[0] = RectMake(441, 411, 50, 50);
		menuRect[1] = RectMake(491, 411, 50, 50);
		menuRect[2] = RectMake(541, 411, 50, 50);
		menuRect[3] = RectMake(591, 411, 50, 50);
		vMenu.push_back(menuRect[i]);
	}

	playerCount = 0;

	isOut = false;
	isEquipment = false;
	isBuy = false;
	isSell = false;

	return S_OK;
}

void LobbyScene::release()
{
}

void LobbyScene::update()
{
	if (vMenu.size() != 0)
	{
		if (PtInRect(&menuRect[0], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			vMenu.clear();

			SOUNDMANAGER->play("click", 1.0f);
			isOut = true;
			isEquipment = false;
			isBuy = false;
			isSell = false;

			playerNum = 0;

			for (int i = 0; i < 9; i++)
			{
				outRect[0] = RectMake(440, 438, 80, 20);
				outRect[1] = RectMake(530, 438, 80, 20);
				outRect[2] = RectMake(30, 40, 48, 48);
				outRect[3] = RectMake(85, 40, 48, 48);
				outRect[4] = RectMake(140, 40, 48, 48);
				outRect[5] = RectMake(195, 40, 48, 48);
				outRect[6] = RectMake(250, 40, 48, 48);
				outRect[7] = RectMake(305, 40, 48, 48);
				outRect[8] = RectMake(360, 40, 48, 48);
				vOut.push_back(outRect[i]);
			}
		}
		else if (PtInRect(&menuRect[1], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			vMenu.clear();

			SOUNDMANAGER->play("click", 1.0f);
			isOut = false;
			isEquipment = true;
			isBuy = false;
			isSell = false;

			playerNum = 0;

			for (int i = 0; i < 3; i++)
			{
				equipRect[0] = RectMake(323, 378, 60, 20);
				equipRect[1] = RectMake(410, 378, 75, 20);
				equipRect[2] = RectMake(488, 378, 75, 20);
				vEquipment.push_back(equipRect[i]);
			}
		}
		else if (PtInRect(&menuRect[2], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			vMenu.clear();

			SOUNDMANAGER->play("click", 1.0f);
			isOut = false;
			isEquipment = false;
			isBuy = true;
			isSell = false;

			playerNum = 0;
			frameX = 0;

			for (int i = 0; i < 5; i++)
			{
				buyRect[0] = RectMake(330, 383, 60, 20);
				buyRect[1] = RectMake(407, 380, 75, 20);
				buyRect[2] = RectMake(485, 380, 75, 20);
				buyRect[3] = RectMake(37, 80, 70, 20);
				buyRect[4] = RectMake(108, 80, 70, 20);
				vBuy.push_back(buyRect[i]);
			}
		}
		else if (PtInRect(&menuRect[3], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			vMenu.clear();

			SOUNDMANAGER->play("click", 1.0f);
			isOut = false;
			isEquipment = false;
			isBuy = false;
			isSell = true;

			frameX = 0;

			for (int i = 0; i < 3; i++)
			{
				sellRect[0] = RectMake(522, 382, 90, 20);
				sellRect[1] = RectMake(45, 82, 80, 20);
				sellRect[2] = RectMake(130, 82, 80, 20);
				vSell.push_back(sellRect[i]);
			}
		}
	}

	if (isOut)
	{
		if (PtInRect(&outRect[0], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			if (playerCount == 7)
			{
				SOUNDMANAGER->play("start", 1.0f);
				SCENEMANAGER->changeScene("GameScene");
			}
			else SOUNDMANAGER->play("noStart", 1.0f);
		}
		else if (PtInRect(&outRect[1], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("click", 1.0f);
			isOut = false;
			vOut.clear();

			for (int i = 0; i < 4; i++)
			{
				menuRect[0] = RectMake(441, 411, 50, 50);
				menuRect[1] = RectMake(491, 411, 50, 50);
				menuRect[2] = RectMake(541, 411, 50, 50);
				menuRect[3] = RectMake(591, 411, 50, 50);
				vMenu.push_back(menuRect[i]);
			}
		}
		else if (PtInRect(&outRect[2], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))		//조조
		{
			SOUNDMANAGER->play("pSelect", 1.0f);

			playerCount += 1;
			playerNum = 0;
			outRect[2] = RectMake(22, 333, 48, 48);
		}
		else if (PtInRect(&outRect[3], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))		//하후돈
		{
			SOUNDMANAGER->play("pSelect", 1.0f);

			playerCount += 1;
			playerNum = 1;
			outRect[3] = RectMake(72, 333, 48, 48);
		}
		else if (PtInRect(&outRect[4], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))		//하후연
		{
			SOUNDMANAGER->play("pSelect", 1.0f);

			playerCount += 1;
			playerNum = 2;
			outRect[4] = RectMake(120, 333, 48, 48);
		}
		else if (PtInRect(&outRect[5], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))		//조인
		{
			SOUNDMANAGER->play("pSelect", 1.0f);

			playerCount += 1;
			playerNum = 3;
			outRect[5] = RectMake(170, 333, 48, 48);
		}
		else if (PtInRect(&outRect[6], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))		//악진
		{
			SOUNDMANAGER->play("pSelect", 1.0f);

			playerCount += 1;
			playerNum = 4;
			outRect[6] = RectMake(220, 333, 48, 48);
		}
		else if (PtInRect(&outRect[7], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))		//이전
		{
			SOUNDMANAGER->play("pSelect", 1.0f);

			playerCount += 1;
			playerNum = 5;
			outRect[7] = RectMake(270, 333, 48, 48);
		}
		else if (PtInRect(&outRect[8], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))		//조홍
		{
			SOUNDMANAGER->play("pSelect", 1.0f);

			playerCount += 1;
			playerNum = 6;
			outRect[8] = RectMake(320, 333, 48, 48);
		}
	}
	else if (isEquipment)
	{
		if (PtInRect(&equipRect[0], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("click", 1.0f);
			isEquipment = false;
			vEquipment.clear();

			for (int i = 0; i < 4; i++)
			{
				menuRect[0] = RectMake(441, 411, 50, 50);
				menuRect[1] = RectMake(491, 411, 50, 50);
				menuRect[2] = RectMake(541, 411, 50, 50);
				menuRect[3] = RectMake(591, 411, 50, 50);
				vMenu.push_back(menuRect[i]);
			}
		}
		else if (PtInRect(&equipRect[1], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("click", 1.0f);
			if (playerNum != 0) playerNum -= 1;
		}
		else if (PtInRect(&equipRect[2], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("click", 1.0f);
			if (playerNum < 6) playerNum += 1;
		}
	}
	else if (isBuy)
	{
		if (PtInRect(&buyRect[0], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("click", 1.0f);
			isBuy = false;
			vBuy.clear();

			for (int i = 0; i < 4; i++)
			{
				menuRect[0] = RectMake(441, 411, 50, 50);
				menuRect[1] = RectMake(491, 411, 50, 50);
				menuRect[2] = RectMake(541, 411, 50, 50);
				menuRect[3] = RectMake(591, 411, 50, 50);
				vMenu.push_back(menuRect[i]);
			}
		}
		else if (PtInRect(&buyRect[1], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("click", 1.0f);
			if (playerNum != 0) playerNum -= 1;
		}
		else if (PtInRect(&buyRect[2], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("click", 1.0f);
			if (playerNum < 6) playerNum += 1;
		}
		else if (PtInRect(&buyRect[3], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("click", 1.0f);
			frameX = 0;
		}
		else if (PtInRect(&buyRect[4], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("click", 1.0f);
			frameX = 1;
		}
	}
	else if (isSell)
	{
		if (PtInRect(&sellRect[0], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("click", 1.0f);
			isSell = false;
			vSell.clear();

			for (int i = 0; i < 4; i++)
			{
				menuRect[0] = RectMake(441, 411, 50, 50);
				menuRect[1] = RectMake(491, 411, 50, 50);
				menuRect[2] = RectMake(541, 411, 50, 50);
				menuRect[3] = RectMake(591, 411, 50, 50);
				vMenu.push_back(menuRect[i]);
			}
		}
		else if (PtInRect(&sellRect[1], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("click", 1.0f);
			frameX = 0;
		}
		else if (PtInRect(&sellRect[2], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("click", 1.0f);
			frameX = 1;
		}
	}
}

void LobbyScene::render()
{
	if (isOut)
	{
		auto& player = PLAYERMANAGER->getPlayer()[playerNum];
		auto& playerInfo = PLAYERMANAGER->getPlayer()[playerNum]->getPlayerInfo();

		IMAGEMANAGER->render("outgoing", getMemDC());
		IMAGEMANAGER->frameRender("조조", getMemDC(), outRect[2].left, outRect[2].top, 0, 8);
		IMAGEMANAGER->frameRender("하후돈", getMemDC(), outRect[3].left, outRect[3].top, 0, 8);
		IMAGEMANAGER->frameRender("하후연", getMemDC(), outRect[4].left, outRect[4].top, 0, 8);
		IMAGEMANAGER->frameRender("조인", getMemDC(), outRect[5].left, outRect[5].top, 0, 8);
		IMAGEMANAGER->frameRender("악진", getMemDC(), outRect[6].left, outRect[6].top, 0, 8);
		IMAGEMANAGER->frameRender("이전", getMemDC(), outRect[7].left, outRect[7].top, 0, 8);
		IMAGEMANAGER->frameRender("조홍", getMemDC(), outRect[8].left, outRect[8].top, 0, 8);

		if (playerNum == 0) IMAGEMANAGER->render("o조조", getMemDC(), 427, 3);
		else if (playerNum == 1) IMAGEMANAGER->render("o하후돈", getMemDC(), 427, 3);
		else if (playerNum == 2) IMAGEMANAGER->render("o하후연", getMemDC(), 427, 3);
		else if (playerNum == 3) IMAGEMANAGER->render("o조인", getMemDC(), 427, 3);
		else if (playerNum == 4) IMAGEMANAGER->render("o악진", getMemDC(), 427, 3);
		else if (playerNum == 5) IMAGEMANAGER->render("o이전", getMemDC(), 427, 3);
		else if (playerNum == 6) IMAGEMANAGER->render("o조홍", getMemDC(), 427, 3);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(getMemDC(), myFont);
		SetTextColor(getMemDC(), RGB(0, 0, 0));
		sprintf_s(str, "%d / 7", playerCount);
		TextOut(getMemDC(), 95, 317, str, strlen(str));
		sprintf_s(str, "%d", player->getMaxHp());
		TextOut(getMemDC(), 590, 145, str, strlen(str));
		sprintf_s(str, "%d", player->getMaxMp());
		TextOut(getMemDC(), 597, 178, str, strlen(str));
		sprintf_s(str, "%d", player->getCurrentExp());
		TextOut(getMemDC(), 597, 212, str, strlen(str));
		sprintf_s(str, "%d", playerInfo.atk);
		TextOut(getMemDC(), 597, 244, str, strlen(str));
		sprintf_s(str, "%d", playerInfo.will);
		TextOut(getMemDC(), 597, 278, str, strlen(str));
		sprintf_s(str, "%d", playerInfo.def);
		TextOut(getMemDC(), 597, 310, str, strlen(str));
		sprintf_s(str, "%d", playerInfo.agi);
		TextOut(getMemDC(), 597, 343, str, strlen(str));
		sprintf_s(str, "%d", playerInfo.ten);
		TextOut(getMemDC(), 597, 376, str, strlen(str));
		sprintf_s(str, "%d", playerInfo.movingCount);
		TextOut(getMemDC(), 603, 409, str, strlen(str));
		sprintf_s(str, "출진");
		TextOut(getMemDC(), 467, 442, str, strlen(str));
		sprintf_s(str, "취소");
		TextOut(getMemDC(), 560, 442, str, strlen(str));
		SelectObject(getMemDC(), oldFont);
		DeleteObject(myFont);
	}
	else if (isEquipment)
	{
		auto& player = PLAYERMANAGER->getPlayer()[playerNum];
		auto& playerInfo = PLAYERMANAGER->getPlayer()[playerNum]->getPlayerInfo();

		IMAGEMANAGER->render("equipment", getMemDC());
		IMAGEMANAGER->render(playerInfo.face, getMemDC(), 415, 100);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(getMemDC(), myFont);
		SetTextColor(getMemDC(), RGB(0, 0, 0));
		sprintf_s(str, "이전 장수로");
		TextOut(getMemDC(), 412, 382, str, strlen(str));
		sprintf_s(str, "다음 장수로");
		TextOut(getMemDC(), 492, 382, str, strlen(str));
		sprintf_s(str, playerInfo.name);
		TextOut(getMemDC(), 428, 76, str, strlen(str));
		sprintf_s(str, playerInfo.className);
		TextOut(getMemDC(), 493, 76, str, strlen(str));
		sprintf_s(str, "%d", playerInfo.level);
		TextOut(getMemDC(), 585, 74, str, strlen(str));
		sprintf_s(str, "%d", playerInfo.atk);
		TextOut(getMemDC(), 451, 244, str, strlen(str));
		sprintf_s(str, "%d", playerInfo.will);
		TextOut(getMemDC(), 451, 265, str, strlen(str));
		sprintf_s(str, "%d", playerInfo.def);
		TextOut(getMemDC(), 451, 286, str, strlen(str));
		sprintf_s(str, "%d", playerInfo.agi);
		TextOut(getMemDC(), 451, 308, str, strlen(str));
		sprintf_s(str, "%d", playerInfo.ten);
		TextOut(getMemDC(), 451, 329, str, strlen(str));
		sprintf_s(str, "%d", playerInfo.movingCount);
		TextOut(getMemDC(), 451, 351, str, strlen(str));
		sprintf_s(str, "%d", player->getMaxHp());
		TextOut(getMemDC(), 450, 192, str, strlen(str));
		sprintf_s(str, "%d", player->getMaxMp());
		TextOut(getMemDC(), 451, 213, str, strlen(str));
		SelectObject(getMemDC(), oldFont);
		DeleteObject(myFont);
	}
	else if (isBuy)
	{
		auto& player = PLAYERMANAGER->getPlayer()[playerNum];
		auto& playerInfo = PLAYERMANAGER->getPlayer()[playerNum]->getPlayerInfo();

		IMAGEMANAGER->frameRender("buy", getMemDC(), 0, 0, frameX, 0);
		IMAGEMANAGER->render(playerInfo.face, getMemDC(), 413, 98);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(getMemDC(), myFont);
		SetTextColor(getMemDC(), RGB(0, 0, 0));
		sprintf_s(str, "이전");
		TextOut(getMemDC(), 430, 384, str, strlen(str));
		sprintf_s(str, "다음");
		TextOut(getMemDC(), 510, 384, str, strlen(str));
		sprintf_s(str, playerInfo.name);
		TextOut(getMemDC(), 428, 74, str, strlen(str));
		sprintf_s(str, playerInfo.className);
		TextOut(getMemDC(), 493, 74, str, strlen(str));
		sprintf_s(str, "%d", playerInfo.level);
		TextOut(getMemDC(), 583, 73, str, strlen(str));
		sprintf_s(str, "%d", playerInfo.atk);
		TextOut(getMemDC(), 449, 244, str, strlen(str));
		sprintf_s(str, "%d", playerInfo.will);
		TextOut(getMemDC(), 449, 266, str, strlen(str));
		sprintf_s(str, "%d", playerInfo.def);
		TextOut(getMemDC(), 449, 287, str, strlen(str));
		sprintf_s(str, "%d", playerInfo.agi);
		TextOut(getMemDC(), 449, 309, str, strlen(str));
		sprintf_s(str, "%d", playerInfo.ten);
		TextOut(getMemDC(), 449, 330, str, strlen(str));
		sprintf_s(str, "%d", playerInfo.movingCount);
		TextOut(getMemDC(), 449, 352, str, strlen(str));
		sprintf_s(str, "%d", player->getMaxHp());
		TextOut(getMemDC(), 448, 192, str, strlen(str));
		sprintf_s(str, "%d", player->getMaxMp());
		TextOut(getMemDC(), 449, 213, str, strlen(str));
		SelectObject(getMemDC(), oldFont);
		DeleteObject(myFont);
	}
	else if (isSell)
	{
		IMAGEMANAGER->frameRender("sell", getMemDC(), 0, 0, frameX, 0);
	}
	else
	{
		IMAGEMANAGER->render("lobbyScene", getMemDC());
	}

	sprintf_s(str, "%d,%d", m_ptMouse.x, m_ptMouse.y);
	TextOut(getMemDC(), 900, 300, str, strlen(str));
}
