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
	else SOUNDMANAGER->stop("lobbySound");

	for (int i = 0; i < 4; i++)
	{
		menuRect[i] = RectMake(441 + 50 * i, 411, 50, 50);
		vMenu.push_back(menuRect[i]);
	}

	//아이템 벡터
	vWeapon = ITEMMANAGER->getWeapon();
	vArmor = ITEMMANAGER->getArmor();
	vPotion = ITEMMANAGER->getPotion();

	money = 30000;
	playerCount = 0;

	isBattle = false;
	isInventory = false;
	isBuyShop = false;
	isSellShop = false;
	isEquip = false;

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
			isBattle = true;
			isInventory = false;
			isBuyShop = false;
			isSellShop = false;

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
			isBattle = false;
			isInventory = true;
			isBuyShop = false;
			isSellShop = false;

			playerNum = 0;

			for (int i = 0; i < 5; i++)
			{
				equipRect[0] = RectMake(323, 378, 60, 20);
				equipRect[1] = RectMake(410, 378, 75, 20);
				equipRect[2] = RectMake(488, 378, 75, 20);
				equipRect[3] = RectMake(512, 122, 32, 32);
				equipRect[4] = RectMake(512, 220, 32, 32);
				vEquipment.push_back(equipRect[i]);
			}

			for (int i = 0; i < 13; i++)
			{
				itemRect[i] = RectMake(38, 99 + 20 * i, 325, 20);
				vEquipment.push_back(itemRect[i]);
			}
		}
		else if (PtInRect(&menuRect[2], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			vMenu.clear();

			SOUNDMANAGER->play("click", 1.0f);
			isBattle = false;
			isInventory = false;
			isBuyShop = true;
			isSellShop = false;

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

			for (int i = 0; i < 4; i++)
			{
				itemRect[i] = RectMake(38, 123 + 20 * i, 348, 20);
				vBuy.push_back(itemRect[i]);
			}
		}
		else if (PtInRect(&menuRect[3], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			vMenu.clear();

			SOUNDMANAGER->play("click", 1.0f);
			isBattle = false;
			isInventory = false;
			isBuyShop = false;
			isSellShop = true;

			frameX = 0;

			for (int i = 0; i < 3; i++)
			{
				sellRect[0] = RectMake(522, 382, 90, 20);
				sellRect[1] = RectMake(45, 82, 80, 20);
				sellRect[2] = RectMake(130, 82, 80, 20);
				vSell.push_back(sellRect[i]);
			}

			for (int i = 0; i < 12; i++)
			{
				itemRect[i] = RectMake(42, 125 + 20 * i, 568, 20);
				vSell.push_back(itemRect[i]);
			}
		}
	}

	battle();
	equipment();
	buyShop();
	sellShop();
}

void LobbyScene::render()
{
	if (isBattle)
	{
		auto& player = PLAYERMANAGER->getPlayer()[playerNum];
		auto& playerInfo = PLAYERMANAGER->getPlayer()[playerNum]->getPlayerInfo();

		IMAGEMANAGER->render("outgoing", getMemDC());
		IMAGEMANAGER->frameRender("조조", getMemDC(), outRect[2].left, outRect[2].top, 0, 8);
		IMAGEMANAGER->render("n조조", getMemDC(), 30, 90);
		IMAGEMANAGER->frameRender("하후돈", getMemDC(), outRect[3].left, outRect[3].top, 0, 8);
		IMAGEMANAGER->render("n하후돈", getMemDC(), 85, 90);
		IMAGEMANAGER->frameRender("하후연", getMemDC(), outRect[4].left, outRect[4].top, 0, 8);
		IMAGEMANAGER->render("n하후연", getMemDC(), 140, 90);
		IMAGEMANAGER->frameRender("조인", getMemDC(), outRect[5].left, outRect[5].top, 0, 8);
		IMAGEMANAGER->render("n조인", getMemDC(), 197, 90);
		IMAGEMANAGER->frameRender("악진", getMemDC(), outRect[6].left, outRect[6].top, 0, 8);
		IMAGEMANAGER->render("n악진", getMemDC(), 253, 90);
		IMAGEMANAGER->frameRender("이전", getMemDC(), outRect[7].left, outRect[7].top, 0, 8);
		IMAGEMANAGER->render("n이전", getMemDC(), 307, 90);
		IMAGEMANAGER->frameRender("조홍", getMemDC(), outRect[8].left, outRect[8].top, 0, 8);
		IMAGEMANAGER->render("n조홍", getMemDC(), 360, 90);

		if (playerNum == 0) IMAGEMANAGER->render("o조조", getMemDC(), 427, 5);
		else if (playerNum == 1) IMAGEMANAGER->render("o하후돈", getMemDC(), 427, 5);
		else if (playerNum == 2) IMAGEMANAGER->render("o하후연", getMemDC(), 427, 5);
		else if (playerNum == 3) IMAGEMANAGER->render("o조인", getMemDC(), 427, 5);
		else if (playerNum == 4) IMAGEMANAGER->render("o악진", getMemDC(), 427, 5);
		else if (playerNum == 5) IMAGEMANAGER->render("o이전", getMemDC(), 427, 5);
		else if (playerNum == 6) IMAGEMANAGER->render("o조홍", getMemDC(), 427, 5);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(getMemDC(), myFont);
		SetTextColor(getMemDC(), RGB(0, 0, 0));
		sprintf_s(str, "%d / 7", playerCount);
		TextOut(getMemDC(), 95, 317, str, strlen(str));
		sprintf_s(str, "%d", player->getMaxHp());
		TextOut(getMemDC(), 590, 134, str, strlen(str));
		sprintf_s(str, "%d", player->getMaxMp());
		TextOut(getMemDC(), 597, 168, str, strlen(str));
		sprintf_s(str, "%d", player->getCurrentExp());
		TextOut(getMemDC(), 597, 204, str, strlen(str));
		if (isEquip) sprintf_s(str, "%d", playerInfo.atk + playerInfo.addAtk);
		else sprintf_s(str, "%d", playerInfo.atk);
		TextOut(getMemDC(), 597, 238, str, strlen(str));
		sprintf_s(str, "%d", playerInfo.will);
		TextOut(getMemDC(), 597, 273, str, strlen(str));
		if (isEquip) sprintf_s(str, "%d", playerInfo.def + playerInfo.addDef);
		else sprintf_s(str, "%d", playerInfo.def + playerInfo.addDef);
		TextOut(getMemDC(), 597, 307, str, strlen(str));
		sprintf_s(str, "%d", playerInfo.agi);
		TextOut(getMemDC(), 597, 342, str, strlen(str));
		sprintf_s(str, "%d", playerInfo.ten);
		TextOut(getMemDC(), 597, 376, str, strlen(str));
		sprintf_s(str, "%d", playerInfo.movingCount);
		TextOut(getMemDC(), 603, 411, str, strlen(str));
		sprintf_s(str, "출진");
		TextOut(getMemDC(), 467, 442, str, strlen(str));
		sprintf_s(str, "취소");
		TextOut(getMemDC(), 560, 442, str, strlen(str));
		SelectObject(getMemDC(), oldFont);
		DeleteObject(myFont);
	}
	else if (isInventory)
	{
		auto& player = PLAYERMANAGER->getPlayer()[playerNum];
		auto& playerInfo = PLAYERMANAGER->getPlayer()[playerNum]->getPlayerInfo();

		IMAGEMANAGER->render("equipment", getMemDC());
		IMAGEMANAGER->render(playerInfo.face, getMemDC(), 415, 100);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(getMemDC(), myFont);
		SetTextColor(getMemDC(), RGB(0, 0, 0));
		//인터페이스
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
		sprintf_s(str, "없음");
		TextOut(getMemDC(), 544, 102, str, strlen(str));
		sprintf_s(str, "없음");
		TextOut(getMemDC(), 556, 199, str, strlen(str));
		sprintf_s(str, "없음");
		TextOut(getMemDC(), 544, 297, str, strlen(str));

		//장비 목록
		for (int i = 0; i < vAllItem.size(); i++)
		{
			if (PtInRect(&itemRect[i], m_ptMouse))
			{
				IMAGEMANAGER->alphaRender("equipSelect", getMemDC(), itemRect[i].left, itemRect[i].top, 100);
			}

			if (vAllItem[i].name == "단검") vAllItem[i].itemImage->render(getMemDC(), itemRect[i].left - 2, itemRect[i].top - 2, 24, 24);
			else if (vAllItem[i].name == "단창") vAllItem[i].itemImage->render(getMemDC(), itemRect[i].left, itemRect[i].top, 22, 22);
			else if (vAllItem[i].name == "반궁") vAllItem[i].itemImage->render(getMemDC(), itemRect[i].left, itemRect[i].top - 1, 22, 22);
			else if (vAllItem[i].name == "가죽갑옷") vAllItem[i].itemImage->render(getMemDC(), itemRect[i].left + 1, itemRect[i].top, 20, 20);

			sprintf_s(str, vAllItem[i].name);
			TextOut(getMemDC(), itemRect[i].left + 22, itemRect[i].top + 4, str, strlen(str));
			if (vAllItem[i].itemKind == ITEM_ARMOR)
			{
				sprintf_s(str, vAllItem[i].attribute);
				TextOut(getMemDC(), itemRect[i].left + 135, itemRect[i].top + 4, str, strlen(str));
			}
			else
			{
				sprintf_s(str, vAllItem[i].attribute);
				TextOut(getMemDC(), itemRect[i].left + 140, itemRect[i].top + 4, str, strlen(str));
			}
			sprintf_s(str, "%d", vAllItem[i].level);
			TextOut(getMemDC(), itemRect[i].left + 190, itemRect[i].top + 4, str, strlen(str));
			sprintf_s(str, "%d", vAllItem[i].exp);
			TextOut(getMemDC(), itemRect[i].left + 223, itemRect[i].top + 4, str, strlen(str));
			sprintf_s(str, "+%d", vAllItem[i].power);
			TextOut(getMemDC(), itemRect[i].left + 253, itemRect[i].top + 4, str, strlen(str));
			if (vAllItem[i].name == "단검")
			{
				if (playerNum == 0 || playerNum == 4 || playerNum == 5 || playerNum == 6)
				{
					sprintf_s(str, "O");
					TextOut(getMemDC(), itemRect[i].left + 303, itemRect[i].top + 4, str, strlen(str));

					if (PtInRect(&itemRect[i], m_ptMouse))
					{
						sprintf_s(str, "+10");
						TextOut(getMemDC(), 477, 244, str, strlen(str));
					}
				}
				else
				{
					sprintf_s(str, "X");
					TextOut(getMemDC(), itemRect[i].left + 303, itemRect[i].top + 4, str, strlen(str));
				}
			}
			if (vAllItem[i].name == "단창")
			{
				if (playerNum == 1 || playerNum == 3)
				{
					sprintf_s(str, "O");
					TextOut(getMemDC(), itemRect[i].left + 303, itemRect[i].top + 4, str, strlen(str));

					if (PtInRect(&itemRect[i], m_ptMouse))
					{
						sprintf_s(str, "+12");
						TextOut(getMemDC(), 477, 244, str, strlen(str));
					}
				}
				else
				{
					sprintf_s(str, "X");
					TextOut(getMemDC(), itemRect[i].left + 303, itemRect[i].top + 4, str, strlen(str));
				}
			}
			if (vAllItem[i].name == "반궁")
			{
				if (playerNum == 2)
				{
					sprintf_s(str, "O");
					TextOut(getMemDC(), itemRect[i].left + 303, itemRect[i].top + 4, str, strlen(str));

					if (PtInRect(&itemRect[i], m_ptMouse))
					{
						sprintf_s(str, "+8");
						TextOut(getMemDC(), 477, 244, str, strlen(str));
					}
				}
				else
				{
					sprintf_s(str, "X");
					TextOut(getMemDC(), itemRect[i].left + 303, itemRect[i].top + 4, str, strlen(str));
				}
			}
			if (vAllItem[i].name == "가죽갑옷")
			{
				sprintf_s(str, "O");
				TextOut(getMemDC(), itemRect[i].left + 303, itemRect[i].top + 4, str, strlen(str));

				if (PtInRect(&itemRect[i], m_ptMouse))
				{
					sprintf_s(str, "+10");
					TextOut(getMemDC(), 477, 286, str, strlen(str));
				}
			}
		}

		//착용중인 장비
		if (playerNum == 0)
		{
			for (int j = 0; j < vEquipJj.size(); j++)
			{
				if (vEquipJj[j].itemKind == ITEM_WEAPON)
				{
					IMAGEMANAGER->render("empty", getMemDC(), 542, 100);
					IMAGEMANAGER->render("erase", getMemDC(), 451, 244);
					vEquipJj[j].itemImage->render(getMemDC(), equipRect[3].left, equipRect[3].top);
					sprintf_s(str, vEquipJj[j].name);
					TextOut(getMemDC(), 544, 102, str, strlen(str));
					sprintf_s(str, "공격력");
					TextOut(getMemDC(), 511, 160, str, strlen(str));
					sprintf_s(str, "+%d", vEquipJj[j].power);
					TextOut(getMemDC(), 560, 159, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.atk + vEquipJj[j].power);
					TextOut(getMemDC(), 451, 244, str, strlen(str));
				}
				else
				{
					IMAGEMANAGER->render("empty", getMemDC(), 554, 197);
					IMAGEMANAGER->render("erase", getMemDC(), 451, 287);
					vEquipJj[j].itemImage->render(getMemDC(), equipRect[4].left, equipRect[4].top - 1);
					sprintf_s(str, vEquipJj[j].name);
					TextOut(getMemDC(), 556, 199, str, strlen(str));
					sprintf_s(str, "방어력");
					TextOut(getMemDC(), 511, 260, str, strlen(str));
					sprintf_s(str, "+%d", vEquipJj[j].power);
					TextOut(getMemDC(), 560, 259, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.def + vEquipJj[j].power);
					TextOut(getMemDC(), 451, 286, str, strlen(str));
				}
			}
		}
		else if (playerNum == 1)
		{
			for (int j = 0; j < vEquipHd.size(); j++)
			{
				if (vEquipHd[j].itemKind == ITEM_WEAPON)
				{
					IMAGEMANAGER->render("empty", getMemDC(), 542, 100);
					IMAGEMANAGER->render("erase", getMemDC(), 451, 244);
					vEquipHd[j].itemImage->render(getMemDC(), equipRect[3].left, equipRect[3].top);
					sprintf_s(str, vEquipHd[j].name);
					TextOut(getMemDC(), 544, 102, str, strlen(str));
					sprintf_s(str, "공격력");
					TextOut(getMemDC(), 511, 160, str, strlen(str));
					sprintf_s(str, "+%d", vEquipHd[j].power);
					TextOut(getMemDC(), 560, 159, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.atk + vEquipHd[j].power);
					TextOut(getMemDC(), 451, 244, str, strlen(str));
				}
				else
				{
					IMAGEMANAGER->render("empty", getMemDC(), 554, 197);
					IMAGEMANAGER->render("erase", getMemDC(), 451, 287);
					vEquipHd[j].itemImage->render(getMemDC(), equipRect[4].left, equipRect[4].top);
					sprintf_s(str, vEquipHd[j].name);
					TextOut(getMemDC(), 556, 199, str, strlen(str));
					sprintf_s(str, "방어력");
					TextOut(getMemDC(), 511, 260, str, strlen(str));
					sprintf_s(str, "+%d", vEquipHd[j].power);
					TextOut(getMemDC(), 560, 259, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.def + vEquipHd[j].power);
					TextOut(getMemDC(), 451, 286, str, strlen(str));
				}
			}
		}
		else if (playerNum == 2)
		{
			for (int j = 0; j < vEquipHy.size(); j++)
			{
				if (vEquipHy[j].itemKind == ITEM_WEAPON)
				{
					IMAGEMANAGER->render("empty", getMemDC(), 542, 100);
					IMAGEMANAGER->render("erase", getMemDC(), 451, 244);
					vEquipHy[j].itemImage->render(getMemDC(), equipRect[3].left, equipRect[3].top);
					sprintf_s(str, vEquipHy[j].name);
					TextOut(getMemDC(), 544, 102, str, strlen(str));
					sprintf_s(str, "공격력");
					TextOut(getMemDC(), 511, 160, str, strlen(str));
					sprintf_s(str, "+%d", vEquipHy[j].power);
					TextOut(getMemDC(), 560, 159, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.atk + vEquipHy[j].power);
					TextOut(getMemDC(), 451, 244, str, strlen(str));
				}
				else
				{
					IMAGEMANAGER->render("empty", getMemDC(), 554, 197);
					IMAGEMANAGER->render("erase", getMemDC(), 451, 287);
					vEquipHy[j].itemImage->render(getMemDC(), equipRect[4].left, equipRect[4].top);
					sprintf_s(str, vEquipHy[j].name);
					TextOut(getMemDC(), 556, 199, str, strlen(str));
					sprintf_s(str, "방어력");
					TextOut(getMemDC(), 511, 260, str, strlen(str));
					sprintf_s(str, "+%d", vEquipHy[j].power);
					TextOut(getMemDC(), 560, 259, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.def + vEquipHy[j].power);
					TextOut(getMemDC(), 451, 286, str, strlen(str));
				}
			}
		}
		else if (playerNum == 3)
		{
			for (int j = 0; j < vEquipJi.size(); j++)
			{
				if (vEquipJi[j].itemKind == ITEM_WEAPON)
				{
					IMAGEMANAGER->render("empty", getMemDC(), 542, 100);
					IMAGEMANAGER->render("erase", getMemDC(), 451, 244);
					vEquipJi[j].itemImage->render(getMemDC(), equipRect[3].left, equipRect[3].top);
					sprintf_s(str, vEquipJi[j].name);
					TextOut(getMemDC(), 544, 102, str, strlen(str));
					sprintf_s(str, "공격력");
					TextOut(getMemDC(), 511, 160, str, strlen(str));
					sprintf_s(str, "+%d", vEquipJi[j].power);
					TextOut(getMemDC(), 560, 159, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.atk + vEquipJi[j].power);
					TextOut(getMemDC(), 451, 244, str, strlen(str));
				}
				else
				{
					IMAGEMANAGER->render("empty", getMemDC(), 554, 197);
					IMAGEMANAGER->render("erase", getMemDC(), 451, 287);
					vEquipJi[j].itemImage->render(getMemDC(), equipRect[4].left, equipRect[4].top);
					sprintf_s(str, vEquipJi[j].name);
					TextOut(getMemDC(), 556, 199, str, strlen(str));
					sprintf_s(str, "방어력");
					TextOut(getMemDC(), 511, 260, str, strlen(str));
					sprintf_s(str, "+%d", vEquipJi[j].power);
					TextOut(getMemDC(), 560, 259, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.def + vEquipJi[j].power);
					TextOut(getMemDC(), 451, 286, str, strlen(str));
				}
			}
		}
		else if (playerNum == 4)
		{
			for (int j = 0; j < vEquipAj.size(); j++)
			{
				if (vEquipAj[j].itemKind == ITEM_WEAPON)
				{
					IMAGEMANAGER->render("empty", getMemDC(), 542, 100);
					IMAGEMANAGER->render("erase", getMemDC(), 451, 244);
					vEquipAj[j].itemImage->render(getMemDC(), equipRect[3].left, equipRect[3].top);
					sprintf_s(str, vEquipAj[j].name);
					TextOut(getMemDC(), 544, 102, str, strlen(str));
					sprintf_s(str, "공격력");
					TextOut(getMemDC(), 511, 160, str, strlen(str));
					sprintf_s(str, "+%d", vEquipAj[j].power);
					TextOut(getMemDC(), 560, 159, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.atk + vEquipAj[j].power);
					TextOut(getMemDC(), 451, 244, str, strlen(str));
				}
				else
				{
					IMAGEMANAGER->render("empty", getMemDC(), 554, 197);
					IMAGEMANAGER->render("erase", getMemDC(), 451, 287);
					vEquipAj[j].itemImage->render(getMemDC(), equipRect[4].left, equipRect[4].top);
					sprintf_s(str, vEquipAj[j].name);
					TextOut(getMemDC(), 556, 199, str, strlen(str));
					sprintf_s(str, "방어력");
					TextOut(getMemDC(), 511, 260, str, strlen(str));
					sprintf_s(str, "+%d", vEquipAj[j].power);
					TextOut(getMemDC(), 560, 259, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.def + vEquipAj[j].power);
					TextOut(getMemDC(), 451, 286, str, strlen(str));
				}
			}
		}
		else if (playerNum == 5)
		{
			for (int j = 0; j < vEquipIj.size(); j++)
			{
				if (vEquipIj[j].itemKind == ITEM_WEAPON)
				{
					IMAGEMANAGER->render("empty", getMemDC(), 542, 100);
					IMAGEMANAGER->render("erase", getMemDC(), 451, 244);
					vEquipIj[j].itemImage->render(getMemDC(), equipRect[3].left, equipRect[3].top);
					sprintf_s(str, vEquipIj[j].name);
					TextOut(getMemDC(), 544, 102, str, strlen(str));
					sprintf_s(str, "공격력");
					TextOut(getMemDC(), 511, 160, str, strlen(str));
					sprintf_s(str, "+%d", vEquipIj[j].power);
					TextOut(getMemDC(), 560, 159, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.atk + vEquipIj[j].power);
					TextOut(getMemDC(), 451, 244, str, strlen(str));
				}
				else
				{
					IMAGEMANAGER->render("empty", getMemDC(), 554, 197);
					IMAGEMANAGER->render("erase", getMemDC(), 451, 287);
					vEquipIj[j].itemImage->render(getMemDC(), equipRect[4].left, equipRect[4].top);
					sprintf_s(str, vEquipIj[j].name);
					TextOut(getMemDC(), 556, 199, str, strlen(str));
					sprintf_s(str, "방어력");
					TextOut(getMemDC(), 511, 260, str, strlen(str));
					sprintf_s(str, "+%d", vEquipIj[j].power);
					TextOut(getMemDC(), 560, 259, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.def + vEquipIj[j].power);
					TextOut(getMemDC(), 451, 286, str, strlen(str));
				}
			}
		}
		else if (playerNum == 6)
		{
			for (int j = 0; j < vEquipJh.size(); j++)
			{
				if (vEquipJh[j].itemKind == ITEM_WEAPON)
				{
					IMAGEMANAGER->render("empty", getMemDC(), 542, 100);
					IMAGEMANAGER->render("erase", getMemDC(), 451, 244);
					vEquipJh[j].itemImage->render(getMemDC(), equipRect[3].left, equipRect[3].top);
					sprintf_s(str, vEquipJh[j].name);
					TextOut(getMemDC(), 544, 102, str, strlen(str));
					sprintf_s(str, "공격력");
					TextOut(getMemDC(), 511, 160, str, strlen(str));
					sprintf_s(str, "+%d", vEquipJh[j].power);
					TextOut(getMemDC(), 560, 159, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.atk + vEquipJh[j].power);
					TextOut(getMemDC(), 451, 244, str, strlen(str));
				}
				else
				{
					IMAGEMANAGER->render("empty", getMemDC(), 554, 197);
					IMAGEMANAGER->render("erase", getMemDC(), 451, 287);
					vEquipJh[j].itemImage->render(getMemDC(), equipRect[4].left, equipRect[4].top);
					sprintf_s(str, vEquipJh[j].name);
					TextOut(getMemDC(), 556, 199, str, strlen(str));
					sprintf_s(str, "방어력");
					TextOut(getMemDC(), 511, 260, str, strlen(str));
					sprintf_s(str, "+%d", vEquipJh[j].power);
					TextOut(getMemDC(), 560, 259, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.def + vEquipJh[j].power);
					TextOut(getMemDC(), 451, 286, str, strlen(str));
				}
			}
		}

		SelectObject(getMemDC(), oldFont);
		DeleteObject(myFont);
	}
	else if (isBuyShop)
	{
		auto& player = PLAYERMANAGER->getPlayer()[playerNum];
		auto& playerInfo = PLAYERMANAGER->getPlayer()[playerNum]->getPlayerInfo();

		IMAGEMANAGER->frameRender("buy", getMemDC(), 0, 0, frameX, 0);
		IMAGEMANAGER->render(playerInfo.face, getMemDC(), 413, 98);

		if (frameX == 0)
		{
			for (int i = 0; i < 4; i++)
			{
				if (PtInRect(&itemRect[i], m_ptMouse)) IMAGEMANAGER->alphaRender("buySelect", getMemDC(), itemRect[i].left, itemRect[i].top, 100);
			}

			vWeapon[0].itemImage->render(getMemDC(), itemRect[0].left - 2, itemRect[0].top - 2, 24, 24);
			vWeapon[1].itemImage->render(getMemDC(), itemRect[1].left, itemRect[1].top, 22, 22);
			vWeapon[2].itemImage->render(getMemDC(), itemRect[2].left, itemRect[2].top - 1, 22, 22);
			vArmor[0].itemImage->render(getMemDC(), itemRect[3].left + 1, itemRect[3].top, 20, 20);
		}
		else
		{
			if (PtInRect(&itemRect[0], m_ptMouse)) IMAGEMANAGER->alphaRender("buySelect", getMemDC(), itemRect[0].left, itemRect[0].top, 100);
			vPotion[0].itemImage->render(getMemDC(), itemRect[0].left + 1, itemRect[0].top, 20, 20);
		}

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(getMemDC(), myFont);
		SetTextColor(getMemDC(), RGB(0, 0, 0));
		//인터페이스
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
		sprintf_s(str, "%d", money);
		TextOut(getMemDC(), 277, 388, str, strlen(str));
		sprintf_s(str, "없음");
		TextOut(getMemDC(), 544, 101, str, strlen(str));
		sprintf_s(str, "없음");
		TextOut(getMemDC(), 556, 199, str, strlen(str));
		sprintf_s(str, "없음");
		TextOut(getMemDC(), 544, 297, str, strlen(str));

		//아이템
		if (frameX == 0)
		{
			//단검
			sprintf_s(str, vWeapon[0].name);
			TextOut(getMemDC(), itemRect[0].left + 20, itemRect[0].top + 4, str, strlen(str));
			sprintf_s(str, vWeapon[0].attribute);
			TextOut(getMemDC(), itemRect[0].left + 135, itemRect[0].top + 4, str, strlen(str));
			if (playerNum == 0 || playerNum == 4 || playerNum == 5 || playerNum == 6)
			{
				sprintf_s(str, "O");
				TextOut(getMemDC(), itemRect[0].left + 170, itemRect[0].top + 4, str, strlen(str));
			}
			else
			{
				sprintf_s(str, "X");
				TextOut(getMemDC(), itemRect[0].left + 170, itemRect[0].top + 4, str, strlen(str));
			}
			sprintf_s(str, "+%d", vWeapon[0].power);
			TextOut(getMemDC(), itemRect[0].left + 206, itemRect[0].top + 4, str, strlen(str));
			sprintf_s(str, "%d", vWeapon[0].stock);
			TextOut(getMemDC(), itemRect[0].left + 253, itemRect[0].top + 4, str, strlen(str));
			sprintf_s(str, "%d", vWeapon[0].count);
			TextOut(getMemDC(), itemRect[0].left + 293, itemRect[0].top + 4, str, strlen(str));
			sprintf_s(str, "%d", vWeapon[0].price);
			TextOut(getMemDC(), itemRect[0].left + 315, itemRect[0].top + 4, str, strlen(str));
			//단창
			sprintf_s(str, vWeapon[1].name);
			TextOut(getMemDC(), itemRect[1].left + 21, itemRect[1].top + 4, str, strlen(str));
			sprintf_s(str, vWeapon[1].attribute);
			TextOut(getMemDC(), itemRect[1].left + 135, itemRect[1].top + 4, str, strlen(str));
			if (playerNum == 1 || playerNum == 3)
			{
				sprintf_s(str, "O");
				TextOut(getMemDC(), itemRect[1].left + 170, itemRect[1].top + 4, str, strlen(str));
			}
			else
			{
				sprintf_s(str, "X");
				TextOut(getMemDC(), itemRect[1].left + 170, itemRect[1].top + 4, str, strlen(str));
			}
			sprintf_s(str, "+%d", vWeapon[1].power);
			TextOut(getMemDC(), itemRect[1].left + 206, itemRect[1].top + 4, str, strlen(str));
			sprintf_s(str, "%d", vWeapon[1].stock);
			TextOut(getMemDC(), itemRect[1].left + 253, itemRect[1].top + 4, str, strlen(str));
			sprintf_s(str, "%d", vWeapon[1].count);
			TextOut(getMemDC(), itemRect[1].left + 293, itemRect[1].top + 4, str, strlen(str));
			sprintf_s(str, "%d", vWeapon[1].price);
			TextOut(getMemDC(), itemRect[1].left + 315, itemRect[1].top + 4, str, strlen(str));
			//반궁
			sprintf_s(str, vWeapon[2].name);
			TextOut(getMemDC(), itemRect[2].left + 22, itemRect[2].top + 4, str, strlen(str));
			sprintf_s(str, vWeapon[2].attribute);
			TextOut(getMemDC(), itemRect[2].left + 135, itemRect[2].top + 4, str, strlen(str));
			if (playerNum == 2)
			{
				sprintf_s(str, "O");
				TextOut(getMemDC(), itemRect[2].left + 170, itemRect[2].top + 4, str, strlen(str));
			}
			else
			{
				sprintf_s(str, "X");
				TextOut(getMemDC(), itemRect[2].left + 170, itemRect[2].top + 4, str, strlen(str));
			}
			sprintf_s(str, "+%d", vWeapon[2].power);
			TextOut(getMemDC(), itemRect[2].left + 206, itemRect[2].top + 4, str, strlen(str));
			sprintf_s(str, "%d", vWeapon[2].stock);
			TextOut(getMemDC(), itemRect[2].left + 253, itemRect[2].top + 4, str, strlen(str));
			sprintf_s(str, "%d", vWeapon[2].count);
			TextOut(getMemDC(), itemRect[2].left + 293, itemRect[2].top + 4, str, strlen(str));
			sprintf_s(str, "%d", vWeapon[2].price);
			TextOut(getMemDC(), itemRect[2].left + 315, itemRect[2].top + 4, str, strlen(str));
			//가죽갑옷
			sprintf_s(str, vArmor[0].name);
			TextOut(getMemDC(), itemRect[3].left + 22, itemRect[3].top + 4, str, strlen(str));
			sprintf_s(str, vArmor[0].attribute);
			TextOut(getMemDC(), itemRect[3].left + 122, itemRect[3].top + 4, str, strlen(str));
			sprintf_s(str, "O");
			TextOut(getMemDC(), itemRect[3].left + 170, itemRect[3].top + 4, str, strlen(str));
			sprintf_s(str, "+%d", vArmor[0].power);
			TextOut(getMemDC(), itemRect[3].left + 206, itemRect[3].top + 4, str, strlen(str));
			sprintf_s(str, "%d", vArmor[0].stock);
			TextOut(getMemDC(), itemRect[3].left + 253, itemRect[3].top + 4, str, strlen(str));
			sprintf_s(str, "%d", vArmor[0].count);
			TextOut(getMemDC(), itemRect[3].left + 293, itemRect[3].top + 4, str, strlen(str));
			sprintf_s(str, "%d", vArmor[0].price);
			TextOut(getMemDC(), itemRect[3].left + 315, itemRect[3].top + 4, str, strlen(str));
		}
		else
		{
			//회복의 콩
			sprintf_s(str, vPotion[0].name);
			TextOut(getMemDC(), itemRect[0].left + 22, itemRect[0].top + 4, str, strlen(str));
			sprintf_s(str, vPotion[0].attribute);
			TextOut(getMemDC(), itemRect[0].left + 150, itemRect[0].top + 4, str, strlen(str));
			sprintf_s(str, "+%d", vPotion[0].power);
			TextOut(getMemDC(), itemRect[0].left + 245, itemRect[0].top + 4, str, strlen(str));
			sprintf_s(str, "%d", vPotion[0].stock);
			TextOut(getMemDC(), itemRect[0].left + 290, itemRect[0].top + 4, str, strlen(str));
			sprintf_s(str, "%d", vPotion[0].price);
			TextOut(getMemDC(), itemRect[0].left + 320, itemRect[0].top + 4, str, strlen(str));
		}

		//착용중인 장비
		if (playerNum == 0)
		{
			for (int j = 0; j < vEquipJj.size(); j++)
			{
				if (vEquipJj[j].itemKind == ITEM_WEAPON)
				{
					IMAGEMANAGER->render("empty", getMemDC(), 542, 100);
					IMAGEMANAGER->render("erase", getMemDC(), 450, 244);
					vEquipJj[j].itemImage->render(getMemDC(), equipRect[3].left, equipRect[3].top);
					sprintf_s(str, vEquipJj[j].name);
					TextOut(getMemDC(), 544, 102, str, strlen(str));
					sprintf_s(str, "공격력");
					TextOut(getMemDC(), 511, 160, str, strlen(str));
					sprintf_s(str, "+%d", vEquipJj[j].power);
					TextOut(getMemDC(), 560, 159, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.atk + vEquipJj[j].power);
					TextOut(getMemDC(), 449, 244, str, strlen(str));
				}
				else
				{
					IMAGEMANAGER->render("empty", getMemDC(), 554, 197);
					IMAGEMANAGER->render("erase", getMemDC(), 450, 287);
					vEquipJj[j].itemImage->render(getMemDC(), equipRect[4].left, equipRect[4].top - 1);
					sprintf_s(str, vEquipJj[j].name);
					TextOut(getMemDC(), 556, 199, str, strlen(str));
					sprintf_s(str, "방어력");
					TextOut(getMemDC(), 511, 260, str, strlen(str));
					sprintf_s(str, "+%d", vEquipJj[j].power);
					TextOut(getMemDC(), 560, 259, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.def + vEquipJj[j].power);
					TextOut(getMemDC(), 449, 287, str, strlen(str));
				}
			}
		}
		else if (playerNum == 1)
		{
			for (int j = 0; j < vEquipHd.size(); j++)
			{
				if (vEquipHd[j].itemKind == ITEM_WEAPON)
				{
					IMAGEMANAGER->render("empty", getMemDC(), 542, 100);
					IMAGEMANAGER->render("erase", getMemDC(), 450, 244);
					vEquipHd[j].itemImage->render(getMemDC(), equipRect[3].left, equipRect[3].top);
					sprintf_s(str, vEquipHd[j].name);
					TextOut(getMemDC(), 544, 102, str, strlen(str));
					sprintf_s(str, "공격력");
					TextOut(getMemDC(), 511, 160, str, strlen(str));
					sprintf_s(str, "+%d", vEquipHd[j].power);
					TextOut(getMemDC(), 560, 159, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.atk + vEquipHd[j].power);
					TextOut(getMemDC(), 449, 244, str, strlen(str));
				}
				else
				{
					IMAGEMANAGER->render("empty", getMemDC(), 554, 197);
					IMAGEMANAGER->render("erase", getMemDC(), 450, 287);
					vEquipHd[j].itemImage->render(getMemDC(), equipRect[4].left, equipRect[4].top);
					sprintf_s(str, vEquipHd[j].name);
					TextOut(getMemDC(), 556, 199, str, strlen(str));
					sprintf_s(str, "방어력");
					TextOut(getMemDC(), 511, 260, str, strlen(str));
					sprintf_s(str, "+%d", vEquipHd[j].power);
					TextOut(getMemDC(), 560, 259, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.def + vEquipHd[j].power);
					TextOut(getMemDC(), 449, 287, str, strlen(str));
				}
			}
		}
		else if (playerNum == 2)
		{
			for (int j = 0; j < vEquipHy.size(); j++)
			{
				if (vEquipHy[j].itemKind == ITEM_WEAPON)
				{
					IMAGEMANAGER->render("empty", getMemDC(), 542, 100);
					IMAGEMANAGER->render("erase", getMemDC(), 450, 244);
					vEquipHy[j].itemImage->render(getMemDC(), equipRect[3].left, equipRect[3].top);
					sprintf_s(str, vEquipHy[j].name);
					TextOut(getMemDC(), 544, 102, str, strlen(str));
					sprintf_s(str, "공격력");
					TextOut(getMemDC(), 511, 160, str, strlen(str));
					sprintf_s(str, "+%d", vEquipHy[j].power);
					TextOut(getMemDC(), 560, 159, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.atk + vEquipHy[j].power);
					TextOut(getMemDC(), 449, 244, str, strlen(str));
				}
				else
				{
					IMAGEMANAGER->render("empty", getMemDC(), 554, 197);
					IMAGEMANAGER->render("erase", getMemDC(), 450, 287);
					vEquipHy[j].itemImage->render(getMemDC(), equipRect[4].left, equipRect[4].top);
					sprintf_s(str, vEquipHy[j].name);
					TextOut(getMemDC(), 556, 199, str, strlen(str));
					sprintf_s(str, "방어력");
					TextOut(getMemDC(), 511, 260, str, strlen(str));
					sprintf_s(str, "+%d", vEquipHy[j].power);
					TextOut(getMemDC(), 560, 259, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.def + vEquipHy[j].power);
					TextOut(getMemDC(), 449, 287, str, strlen(str));
				}
			}
		}
		else if (playerNum == 3)
		{
			for (int j = 0; j < vEquipJi.size(); j++)
			{
				if (vEquipJi[j].itemKind == ITEM_WEAPON)
				{
					IMAGEMANAGER->render("empty", getMemDC(), 542, 100);
					IMAGEMANAGER->render("erase", getMemDC(), 450, 244);
					vEquipJi[j].itemImage->render(getMemDC(), equipRect[3].left, equipRect[3].top);
					sprintf_s(str, vEquipJi[j].name);
					TextOut(getMemDC(), 544, 102, str, strlen(str));
					sprintf_s(str, "공격력");
					TextOut(getMemDC(), 511, 160, str, strlen(str));
					sprintf_s(str, "+%d", vEquipJi[j].power);
					TextOut(getMemDC(), 560, 159, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.atk + vEquipJi[j].power);
					TextOut(getMemDC(), 449, 244, str, strlen(str));
				}
				else
				{
					IMAGEMANAGER->render("empty", getMemDC(), 554, 197);
					IMAGEMANAGER->render("erase", getMemDC(), 450, 287);
					vEquipJi[j].itemImage->render(getMemDC(), equipRect[4].left, equipRect[4].top);
					sprintf_s(str, vEquipJi[j].name);
					TextOut(getMemDC(), 556, 199, str, strlen(str));
					sprintf_s(str, "방어력");
					TextOut(getMemDC(), 511, 260, str, strlen(str));
					sprintf_s(str, "+%d", vEquipJi[j].power);
					TextOut(getMemDC(), 560, 259, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.def + vEquipJi[j].power);
					TextOut(getMemDC(), 449, 287, str, strlen(str));
				}
			}
		}
		else if (playerNum == 4)
		{
			for (int j = 0; j < vEquipAj.size(); j++)
			{
				if (vEquipAj[j].itemKind == ITEM_WEAPON)
				{
					IMAGEMANAGER->render("empty", getMemDC(), 542, 100);
					IMAGEMANAGER->render("erase", getMemDC(), 450, 244);
					vEquipAj[j].itemImage->render(getMemDC(), equipRect[3].left, equipRect[3].top);
					sprintf_s(str, vEquipAj[j].name);
					TextOut(getMemDC(), 544, 102, str, strlen(str));
					sprintf_s(str, "공격력");
					TextOut(getMemDC(), 511, 160, str, strlen(str));
					sprintf_s(str, "+%d", vEquipAj[j].power);
					TextOut(getMemDC(), 560, 159, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.atk + vEquipAj[j].power);
					TextOut(getMemDC(), 449, 244, str, strlen(str));
				}
				else
				{
					IMAGEMANAGER->render("empty", getMemDC(), 554, 197);
					IMAGEMANAGER->render("erase", getMemDC(), 450, 287);
					vEquipAj[j].itemImage->render(getMemDC(), equipRect[4].left, equipRect[4].top);
					sprintf_s(str, vEquipAj[j].name);
					TextOut(getMemDC(), 556, 199, str, strlen(str));
					sprintf_s(str, "방어력");
					TextOut(getMemDC(), 511, 260, str, strlen(str));
					sprintf_s(str, "+%d", vEquipAj[j].power);
					TextOut(getMemDC(), 560, 259, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.def + vEquipAj[j].power);
					TextOut(getMemDC(), 449, 287, str, strlen(str));
				}
			}
		}
		else if (playerNum == 5)
		{
			for (int j = 0; j < vEquipIj.size(); j++)
			{
				if (vEquipIj[j].itemKind == ITEM_WEAPON)
				{
					IMAGEMANAGER->render("empty", getMemDC(), 542, 100);
					IMAGEMANAGER->render("erase", getMemDC(), 450, 244);
					vEquipIj[j].itemImage->render(getMemDC(), equipRect[3].left, equipRect[3].top);
					sprintf_s(str, vEquipIj[j].name);
					TextOut(getMemDC(), 544, 102, str, strlen(str));
					sprintf_s(str, "공격력");
					TextOut(getMemDC(), 511, 160, str, strlen(str));
					sprintf_s(str, "+%d", vEquipIj[j].power);
					TextOut(getMemDC(), 560, 159, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.atk + vEquipIj[j].power);
					TextOut(getMemDC(), 449, 244, str, strlen(str));
				}
				else
				{
					IMAGEMANAGER->render("empty", getMemDC(), 554, 197);
					IMAGEMANAGER->render("erase", getMemDC(), 450, 287);
					vEquipIj[j].itemImage->render(getMemDC(), equipRect[4].left, equipRect[4].top);
					sprintf_s(str, vEquipIj[j].name);
					TextOut(getMemDC(), 556, 199, str, strlen(str));
					sprintf_s(str, "방어력");
					TextOut(getMemDC(), 511, 260, str, strlen(str));
					sprintf_s(str, "+%d", vEquipIj[j].power);
					TextOut(getMemDC(), 560, 259, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.def + vEquipIj[j].power);
					TextOut(getMemDC(), 449, 287, str, strlen(str));
				}
			}
		}
		else if (playerNum == 6)
		{
			for (int j = 0; j < vEquipJh.size(); j++)
			{
				if (vEquipJh[j].itemKind == ITEM_WEAPON)
				{
					IMAGEMANAGER->render("empty", getMemDC(), 542, 100);
					IMAGEMANAGER->render("erase", getMemDC(), 450, 244);
					vEquipJh[j].itemImage->render(getMemDC(), equipRect[3].left, equipRect[3].top);
					sprintf_s(str, vEquipJh[j].name);
					TextOut(getMemDC(), 544, 102, str, strlen(str));
					sprintf_s(str, "공격력");
					TextOut(getMemDC(), 511, 160, str, strlen(str));
					sprintf_s(str, "+%d", vEquipJh[j].power);
					TextOut(getMemDC(), 560, 159, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.atk + vEquipJh[j].power);
					TextOut(getMemDC(), 449, 244, str, strlen(str));
				}
				else
				{
					IMAGEMANAGER->render("empty", getMemDC(), 554, 197);
					IMAGEMANAGER->render("erase", getMemDC(), 450, 287);
					vEquipJh[j].itemImage->render(getMemDC(), equipRect[4].left, equipRect[4].top);
					sprintf_s(str, vEquipJh[j].name);
					TextOut(getMemDC(), 556, 199, str, strlen(str));
					sprintf_s(str, "방어력");
					TextOut(getMemDC(), 511, 260, str, strlen(str));
					sprintf_s(str, "+%d", vEquipJh[j].power);
					TextOut(getMemDC(), 560, 259, str, strlen(str));
					sprintf_s(str, "%d", playerInfo.def + vEquipJh[j].power);
					TextOut(getMemDC(), 449, 287, str, strlen(str));
				}
			}
		}

		SelectObject(getMemDC(), oldFont);
		DeleteObject(myFont);
	}
	else if (isSellShop)
	{
		IMAGEMANAGER->frameRender("sell", getMemDC(), 0, 0, frameX, 0);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(getMemDC(), myFont);
		SetTextColor(getMemDC(), RGB(0, 0, 0));
		//인터페이스
		sprintf_s(str, "%d", money);
		TextOut(getMemDC(), 450, 387, str, strlen(str));
		if (frameX == 0)
		{
			for (int i = 0; i < vAllItem.size(); i++)
			{
				if (PtInRect(&itemRect[i], m_ptMouse))
				{
					IMAGEMANAGER->alphaRender("sellSelect", getMemDC(), itemRect[i].left - 23, itemRect[i].top, 100);
				}

				if (vAllItem[i].name == "단검") vAllItem[i].itemImage->render(getMemDC(), itemRect[i].left - 2, itemRect[i].top - 2, 24, 24);
				else if (vAllItem[i].name == "단창") vAllItem[i].itemImage->render(getMemDC(), itemRect[i].left, itemRect[i].top, 22, 22);
				else if (vAllItem[i].name == "반궁") vAllItem[i].itemImage->render(getMemDC(), itemRect[i].left, itemRect[i].top - 1, 22, 22);
				else if (vAllItem[i].name == "가죽갑옷") vAllItem[i].itemImage->render(getMemDC(), itemRect[i].left + 1, itemRect[i].top, 20, 20);

				sprintf_s(str, vAllItem[i].name);
				TextOut(getMemDC(), itemRect[i].left + 22, itemRect[i].top + 4, str, strlen(str));
				if (vAllItem[i].itemKind == ITEM_ARMOR)
				{
					sprintf_s(str, vAllItem[i].attribute);
					TextOut(getMemDC(), itemRect[i].left + 232, itemRect[i].top + 4, str, strlen(str));
				}
				else
				{
					sprintf_s(str, vAllItem[i].attribute);
					TextOut(getMemDC(), itemRect[i].left + 237, itemRect[i].top + 4, str, strlen(str));
				}
				sprintf_s(str, "%d", vAllItem[i].level);
				TextOut(getMemDC(), itemRect[i].left + 320, itemRect[i].top + 4, str, strlen(str));
				sprintf_s(str, "%d", vAllItem[i].exp);
				TextOut(getMemDC(), itemRect[i].left + 377, itemRect[i].top + 4, str, strlen(str));
				sprintf_s(str, "+%d", vAllItem[i].power);
				TextOut(getMemDC(), itemRect[i].left + 438, itemRect[i].top + 4, str, strlen(str));
				sprintf_s(str, "%d", vAllItem[i].price / 2);
				TextOut(getMemDC(), itemRect[i].left + 515, itemRect[i].top + 4, str, strlen(str));
			}
		}
		else
		{
			if (!vSellPotion.empty())
			{
				if (PtInRect(&itemRect[0], m_ptMouse))
				{
					IMAGEMANAGER->alphaRender("sellSelect", getMemDC(), itemRect[0].left, itemRect[0].top, 100);
				}

				vSellPotion[0].itemImage->render(getMemDC(), itemRect[0].left + 1, itemRect[0].top, 20, 20);
				sprintf_s(str, vSellPotion[0].name);
				TextOut(getMemDC(), itemRect[0].left + 22, itemRect[0].top + 4, str, strlen(str));
				sprintf_s(str, vSellPotion[0].attribute);
				TextOut(getMemDC(), itemRect[0].left + 260, itemRect[0].top + 4, str, strlen(str));
				sprintf_s(str, "+%d", vSellPotion[0].power);
				TextOut(getMemDC(), itemRect[0].left + 415, itemRect[0].top + 4, str, strlen(str));
				sprintf_s(str, "%d", vPotion[0].stock);
				TextOut(getMemDC(), itemRect[0].left + 490, itemRect[0].top + 4, str, strlen(str));
				sprintf_s(str, "%d", vSellPotion[0].price);
				TextOut(getMemDC(), itemRect[0].left + 540, itemRect[0].top + 4, str, strlen(str));
			}
		}
		SelectObject(getMemDC(), oldFont);
		DeleteObject(myFont);
	}
	else
	{
		IMAGEMANAGER->render("lobbyScene", getMemDC());
	}
}

void LobbyScene::battle()
{
	if (isBattle)
	{
		if (PtInRect(&outRect[0], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			if (playerCount == 7)
			{
				vMenu.clear();
				vOut.clear();
				vEquipment.clear();
				vBuy.clear();
				vSell.clear();
				vWeapon.clear();
				vArmor.clear();
				vPotion.clear();
				vAllItem.clear();
				vEquipJj.clear();
				vEquipHd.clear();
				vEquipHy.clear();
				vEquipJi.clear();
				vEquipAj.clear();
				vEquipIj.clear();
				vEquipJh.clear();

				SOUNDMANAGER->play("start", 1.0f);
				SCENEMANAGER->changeScene("GameScene");
			}
			else SOUNDMANAGER->play("noStart", 1.0f);
		}
		else if (PtInRect(&outRect[1], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("click", 1.0f);
			isBattle = false;
			vOut.clear();
			playerCount = 0;

			for (int i = 0; i < 4; i++)
			{
				menuRect[i] = RectMake(441 + 50 * i, 411, 50, 50);
				vMenu.push_back(menuRect[i]);
			}
		}
		else if (PtInRect(&outRect[2], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))		//조조
		{
			playerNum = 0;

			if (playerCount < 7)
			{
				SOUNDMANAGER->play("pSelect", 1.0f);
				playerCount += 1;
				outRect[2] = RectMake(22, 333, 48, 48);
			}
			else SOUNDMANAGER->play("click", 1.0f);
		}
		else if (PtInRect(&outRect[3], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))		//하후돈
		{
			playerNum = 1;

			if (playerCount < 7)
			{
				SOUNDMANAGER->play("pSelect", 1.0f);
				playerCount += 1;
				outRect[3] = RectMake(72, 333, 48, 48);
			}
			else SOUNDMANAGER->play("click", 1.0f);
		}
		else if (PtInRect(&outRect[4], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))		//하후연
		{
			playerNum = 2;

			if (playerCount < 7)
			{
				SOUNDMANAGER->play("pSelect", 1.0f);
				playerCount += 1;
				outRect[4] = RectMake(120, 333, 48, 48);
			}
			else SOUNDMANAGER->play("click", 1.0f);
		}
		else if (PtInRect(&outRect[5], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))		//조인
		{
			playerNum = 3;

			if (playerCount < 7)
			{
				SOUNDMANAGER->play("pSelect", 1.0f);
				playerCount += 1;
				outRect[5] = RectMake(170, 333, 48, 48);
			}
			else SOUNDMANAGER->play("click", 1.0f);
		}
		else if (PtInRect(&outRect[6], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))		//악진
		{
			playerNum = 4;

			if (playerCount < 7)
			{
				SOUNDMANAGER->play("pSelect", 1.0f);
				playerCount += 1;
				outRect[6] = RectMake(220, 333, 48, 48);
			}
			else SOUNDMANAGER->play("click", 1.0f);
		}
		else if (PtInRect(&outRect[7], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))		//이전
		{
			playerNum = 5;

			if (playerCount < 7)
			{
				SOUNDMANAGER->play("pSelect", 1.0f);
				playerCount += 1;
				outRect[7] = RectMake(270, 333, 48, 48);
			}
			else SOUNDMANAGER->play("click", 1.0f);
		}
		else if (PtInRect(&outRect[8], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))		//조홍
		{
			playerNum = 6;

			if (playerCount < 7)
			{
				SOUNDMANAGER->play("pSelect", 1.0f);
				playerCount += 1;
				outRect[8] = RectMake(320, 333, 48, 48);
			}
			else SOUNDMANAGER->play("click", 1.0f);
		}
	}
}

void LobbyScene::equipment()
{
	if (isInventory)
	{
		if (PtInRect(&equipRect[0], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("click", 1.0f);
			isInventory = false;
			vEquipment.clear();

			for (int i = 0; i < 4; i++)
			{
				menuRect[i] = RectMake(441 + 50 * i, 411, 50, 50);
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

		equip();
		equipRelease();
	}
}

void LobbyScene::buyShop()
{
	if (isBuyShop)
	{
		if (PtInRect(&buyRect[0], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("click", 1.0f);
			isBuyShop = false;
			vBuy.clear();

			for (int i = 0; i < 4; i++)
			{
				menuRect[i] = RectMake(441 + 50 * i, 411, 50, 50);
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

		if (frameX == 0)
		{
			if (money >= 1000)
			{
				if (PtInRect(&itemRect[0], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
				{
					SOUNDMANAGER->play("buy", 1.0f);
					money -= 1000;
					vWeapon[0].stock += 1;
					vWeapon[0].count += 1;
					vAllItem.push_back(vWeapon[0]);
				}
				else if (PtInRect(&itemRect[1], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
				{
					SOUNDMANAGER->play("buy", 1.0f);
					money -= 1000;
					vWeapon[1].stock += 1;
					vWeapon[1].count += 1;
					vAllItem.push_back(vWeapon[1]);
				}
				else if (PtInRect(&itemRect[2], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
				{
					SOUNDMANAGER->play("buy", 1.0f);
					money -= 1000;
					vWeapon[2].stock += 1;
					vWeapon[2].count += 1;
					vAllItem.push_back(vWeapon[2]);
				}
				else if (PtInRect(&itemRect[3], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
				{
					SOUNDMANAGER->play("buy", 1.0f);
					money -= 1000;
					vArmor[0].stock += 1;
					vArmor[0].count += 1;
					vAllItem.push_back(vArmor[0]);
				}
			}
		}
		else
		{
			if (money >= 100)
			{
				if (PtInRect(&itemRect[0], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
				{
					SOUNDMANAGER->play("buy", 1.0f);
					money -= 100;
					vPotion[0].stock += 1;
					vSellPotion.push_back(vPotion[0]);
				}
			}
		}
	}
}

void LobbyScene::sellShop()
{
	if (isSellShop)
	{
		if (PtInRect(&sellRect[0], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("click", 1.0f);
			isSellShop = false;
			vSell.clear();

			for (int i = 0; i < 4; i++)
			{
				menuRect[i] = RectMake(441 + 50 * i, 411, 50, 50);
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

		sellItem();
	}
}

void LobbyScene::equip()
{
	if (vAllItem.size() != 0)
	{
		for (int i = 0; i < vAllItem.size(); i++)
		{
			if (PtInRect(&itemRect[i], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				isEquip = true;

				if (vAllItem[i].itemKind == ITEM_WEAPON)
				{
					if (vAllItem[i].name == "단검")
					{
						if (playerNum == 0)
						{
							SOUNDMANAGER->play("buy", 1.0f);
							vEquipJj.push_back(vAllItem[i]);
							vAllItem.erase(vAllItem.begin() + i);
						}
						else if (playerNum == 4)
						{
							SOUNDMANAGER->play("buy", 1.0f);
							vEquipAj.push_back(vAllItem[i]);
							vAllItem.erase(vAllItem.begin() + i);
						}
						else if (playerNum == 5)
						{
							SOUNDMANAGER->play("buy", 1.0f);
							vEquipIj.push_back(vAllItem[i]);
							vAllItem.erase(vAllItem.begin() + i);
						}
						else if (playerNum == 6)
						{
							SOUNDMANAGER->play("buy", 1.0f);
							vEquipJh.push_back(vAllItem[i]);
							vAllItem.erase(vAllItem.begin() + i);
						}
					}
					else if (vAllItem[i].name == "단창")
					{
						if (playerNum == 1)
						{
							SOUNDMANAGER->play("buy", 1.0f);
							vEquipHd.push_back(vAllItem[i]);
							vAllItem.erase(vAllItem.begin() + i);
						}
						else if (playerNum == 3)
						{
							SOUNDMANAGER->play("buy", 1.0f);
							vEquipJi.push_back(vAllItem[i]);
							vAllItem.erase(vAllItem.begin() + i);
						}
					}
					else
					{
						if (playerNum == 2)
						{
							SOUNDMANAGER->play("buy", 1.0f);
							vEquipHy.push_back(vAllItem[i]);
							vAllItem.erase(vAllItem.begin() + i);
						}
					}
				}
				else
				{
					if (playerNum == 0)
					{
						SOUNDMANAGER->play("buy", 1.0f);
						vEquipJj.push_back(vAllItem[i]);
						vAllItem.erase(vAllItem.begin() + i);
					}
					else if (playerNum == 1)
					{
						SOUNDMANAGER->play("buy", 1.0f);
						vEquipHd.push_back(vAllItem[i]);
						vAllItem.erase(vAllItem.begin() + i);
					}
					else if (playerNum == 2)
					{
						SOUNDMANAGER->play("buy", 1.0f);
						vEquipHy.push_back(vAllItem[i]);
						vAllItem.erase(vAllItem.begin() + i);
					}
					else if (playerNum == 3)
					{
						SOUNDMANAGER->play("buy", 1.0f);
						vEquipJi.push_back(vAllItem[i]);
						vAllItem.erase(vAllItem.begin() + i);
					}
					else if (playerNum == 4)
					{
						SOUNDMANAGER->play("buy", 1.0f);
						vEquipAj.push_back(vAllItem[i]);
						vAllItem.erase(vAllItem.begin() + i);
					}
					else if (playerNum == 5)
					{
						SOUNDMANAGER->play("buy", 1.0f);
						vEquipIj.push_back(vAllItem[i]);
						vAllItem.erase(vAllItem.begin() + i);
					}
					else if (playerNum == 6)
					{
						SOUNDMANAGER->play("buy", 1.0f);
						vEquipJh.push_back(vAllItem[i]);
						vAllItem.erase(vAllItem.begin() + i);
					}
				}
			}
		}
	}
}

void LobbyScene::equipRelease()
{
	if (playerNum == 0)
	{
		if (PtInRect(&equipRect[3], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("noStart", 1.0f);

			for (int i = 0; i < vEquipJj.size(); i++)
			{
				if (vEquipJj[i].itemKind == ITEM_WEAPON)
				{
					vAllItem.push_back(vEquipJj[i]);
					vEquipJj.erase(vEquipJj.begin() + i);
				}
			}
		}
		else if (PtInRect(&equipRect[4], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("noStart", 1.0f);

			for (int i = 0; i < vEquipJj.size(); i++)
			{
				if (vEquipJj[i].itemKind == ITEM_ARMOR)
				{
					vAllItem.push_back(vEquipJj[i]);
					vEquipJj.erase(vEquipJj.begin() + i);
				}
			}
		}
	}
	else if (playerNum == 1)
	{
		if (PtInRect(&equipRect[3], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("noStart", 1.0f);

			for (int i = 0; i < vEquipHd.size(); i++)
			{
				if (vEquipHd[i].itemKind == ITEM_WEAPON)
				{
					vAllItem.push_back(vEquipHd[i]);
					vEquipHd.erase(vEquipHd.begin() + i);
				}
			}
		}
		else if (PtInRect(&equipRect[4], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("noStart", 1.0f);

			for (int i = 0; i < vEquipHd.size(); i++)
			{
				if (vEquipHd[i].itemKind == ITEM_ARMOR)
				{
					vAllItem.push_back(vEquipHd[i]);
					vEquipHd.erase(vEquipHd.begin() + i);
				}
			}
		}
	}
	else if (playerNum == 2)
	{
		if (PtInRect(&equipRect[3], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("noStart", 1.0f);

			for (int i = 0; i < vEquipHy.size(); i++)
			{
				if (vEquipHy[i].itemKind == ITEM_WEAPON)
				{
					vAllItem.push_back(vEquipHy[i]);
					vEquipHy.erase(vEquipHy.begin() + i);
				}
			}
		}
		else if (PtInRect(&equipRect[4], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("noStart", 1.0f);

			for (int i = 0; i < vEquipHy.size(); i++)
			{
				if (vEquipHy[i].itemKind == ITEM_ARMOR)
				{
					vAllItem.push_back(vEquipHy[i]);
					vEquipHy.erase(vEquipHy.begin() + i);
				}
			}
		}
	}
	else if (playerNum == 3)
	{
		if (PtInRect(&equipRect[3], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("noStart", 1.0f);

			for (int i = 0; i < vEquipJi.size(); i++)
			{
				if (vEquipJi[i].itemKind == ITEM_WEAPON)
				{
					vAllItem.push_back(vEquipJi[i]);
					vEquipJi.erase(vEquipJi.begin() + i);
				}
			}
		}
		else if (PtInRect(&equipRect[4], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("noStart", 1.0f);

			for (int i = 0; i < vEquipJi.size(); i++)
			{
				if (vEquipJi[i].itemKind == ITEM_ARMOR)
				{
					vAllItem.push_back(vEquipJi[i]);
					vEquipJi.erase(vEquipJi.begin() + i);
				}
			}
		}
	}
	else if (playerNum == 4)
	{
		if (PtInRect(&equipRect[3], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("noStart", 1.0f);

			for (int i = 0; i < vEquipAj.size(); i++)
			{
				if (vEquipAj[i].itemKind == ITEM_WEAPON)
				{
					vAllItem.push_back(vEquipAj[i]);
					vEquipAj.erase(vEquipAj.begin() + i);
				}
			}
		}
		else if (PtInRect(&equipRect[4], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("noStart", 1.0f);

			for (int i = 0; i < vEquipAj.size(); i++)
			{
				if (vEquipAj[i].itemKind == ITEM_ARMOR)
				{
					vAllItem.push_back(vEquipAj[i]);
					vEquipAj.erase(vEquipAj.begin() + i);
				}
			}
		}
	}
	else if (playerNum == 5)
	{
		if (PtInRect(&equipRect[3], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("noStart", 1.0f);

			for (int i = 0; i < vEquipIj.size(); i++)
			{
				if (vEquipIj[i].itemKind == ITEM_WEAPON)
				{
					vAllItem.push_back(vEquipIj[i]);
					vEquipIj.erase(vEquipIj.begin() + i);
				}
			}
		}
		else if (PtInRect(&equipRect[4], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("noStart", 1.0f);

			for (int i = 0; i < vEquipIj.size(); i++)
			{
				if (vEquipIj[i].itemKind == ITEM_ARMOR)
				{
					vAllItem.push_back(vEquipIj[i]);
					vEquipIj.erase(vEquipIj.begin() + i);
				}
			}
		}
	}
	else if (playerNum == 6)
	{
		if (PtInRect(&equipRect[3], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("noStart", 1.0f);

			for (int i = 0; i < vEquipJh.size(); i++)
			{
				if (vEquipJh[i].itemKind == ITEM_WEAPON)
				{
					vAllItem.push_back(vEquipJh[i]);
					vEquipJh.erase(vEquipJh.begin() + i);
				}
			}
		}
		else if (PtInRect(&equipRect[4], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("noStart", 1.0f);

			for (int i = 0; i < vEquipJh.size(); i++)
			{
				if (vEquipJh[i].itemKind == ITEM_ARMOR)
				{
					vAllItem.push_back(vEquipJh[i]);
					vEquipJh.erase(vEquipJh.begin() + i);
				}
			}
		}
	}
}

void LobbyScene::sellItem()
{
	if (frameX == 0)
	{
		if (vAllItem.size() != 0)
		{
			for (int i = 0; i < vAllItem.size(); i++)
			{
				if (PtInRect(&itemRect[i], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
				{
					SOUNDMANAGER->play("sell", 1.0f);
					money += vAllItem[i].price * 0.5;
					vAllItem.erase(vAllItem.begin() + i);
				}
			}
		}
	}
	else
	{
		if (vSellPotion.size() != 0)
		{
			for (int i = 0; i < vSellPotion.size(); i++)
			{
				if (PtInRect(&itemRect[0], m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
				{
					SOUNDMANAGER->play("sell", 1.0f);
					vPotion[0].stock -= 1;
					money += vSellPotion[i].price * 0.5;
					vSellPotion.erase(vSellPotion.begin() + i);
				}
			}
		}
	}
}
