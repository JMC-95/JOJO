#include "stdafx.h"
#include "Interface.h"

Interface::Interface()
{
}

Interface::~Interface()
{
}

void Interface::render(HDC hdc)
{
	playerMenu(hdc);
	playerInformation(hdc);
	friendInformation(hdc);
	enemyInformation(hdc);
}

void Interface::playerMenu(HDC hdc)
{
	auto& jojo = PLAYERMANAGER->getPlayer()[0];
	auto& jojoInfo = PLAYERMANAGER->getPlayer()[0]->getPlayerInfo();

	if (jojo->getIsClick()) IMAGEMANAGER->frameRender("¸Þ´º", hdc, jojoInfo.rc.left - 100, jojoInfo.rc.top - 35, jojo->getFrameX(), 1);

	if (jojo->getIsSkill())
	{
		IMAGEMANAGER->render("skillMenu", hdc, jojoInfo.rc.left - 210, jojoInfo.rc.top - 50);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "³ª´®°íµñÃ¼");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 255));
		sprintf_s(str, "%d/%d", jojo->getCurrentMp(), jojo->getMaxMp());
		TextOut(hdc, jojoInfo.rc.left - 100, jojoInfo.rc.top - 45, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	if (jojo->getIsHeal()) IMAGEMANAGER->render("¼Òº¸±Þ", hdc, 500, 400);

	for (int j = 1; j < PLAYERMANAGER->getPlayer().size(); j++)
	{
		auto playerClick = PLAYERMANAGER->getPlayer()[j]->getIsClick();
		auto playerX = PLAYERMANAGER->getPlayer()[j]->getFrameX();
		auto& playerInfo = PLAYERMANAGER->getPlayer()[j]->getPlayerInfo();

		if (playerClick)
		{
			IMAGEMANAGER->frameRender("¸Þ´º", hdc, playerInfo.rc.left - 100, playerInfo.rc.top - 35, playerX, 0);
		}
	}
}

void Interface::playerInformation(HDC hdc)
{
	for (int i = 0; i < PLAYERMANAGER->getPlayer().size(); i++)
	{
		auto& player = PLAYERMANAGER->getPlayer()[i];
		auto& playerInfo = PLAYERMANAGER->getPlayer()[i]->getPlayerInfo();

		if (PtInRect(&playerInfo.rc, m_ptMouse))
		{
			IMAGEMANAGER->render("Á¤º¸Ã¢", hdc, 960, 200);
			IMAGEMANAGER->render(playerInfo.face, hdc, 960, 218);
			player->getProgressBarHp()->render(hdc);
			player->getProgressBarMp()->render(hdc);
			player->getProgressBarExp()->render(hdc);
			playerInfo.weaponImg->render(hdc, 965, 456);
			playerInfo.armorImg->render(hdc, 1047, 456);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "³ª´®°íµñÃ¼");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(0, 0, 0));
			sprintf_s(str, playerInfo.name);
			TextOut(hdc, 970, 305, str, strlen(str));
			sprintf_s(str, playerInfo.className);
			TextOut(hdc, 1040, 220, str, strlen(str));
			sprintf_s(str, "%d", playerInfo.level);
			TextOut(hdc, 1057, 243, str, strlen(str));
			sprintf_s(str, "%d", playerInfo.atk + playerInfo.addAtk);
			TextOut(hdc, 1005, 343, str, strlen(str));
			sprintf_s(str, "%d", playerInfo.def + playerInfo.addDef);
			TextOut(hdc, 1097, 343, str, strlen(str));
			sprintf_s(str, "%d", playerInfo.ten);
			TextOut(hdc, 1170, 343, str, strlen(str));
			sprintf_s(str, "%d", playerInfo.will);
			TextOut(hdc, 1005, 378, str, strlen(str));
			sprintf_s(str, "%d", playerInfo.agi);
			TextOut(hdc, 1097, 378, str, strlen(str));
			sprintf_s(str, "%d", playerInfo.movingCount);
			TextOut(hdc, 1177, 378, str, strlen(str));
			sprintf_s(str, "%d/%d", player->getCurrentHp(), player->getMaxHp());
			TextOut(hdc, 1088, 289, str, strlen(str));
			sprintf_s(str, "%d/%d", player->getCurrentMp(), player->getMaxMp());
			TextOut(hdc, 1095, 307, str, strlen(str));
			sprintf_s(str, "%d/%d", player->getCurrentExp(), player->getMaxExp());
			TextOut(hdc, 1095, 243, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);

			HFONT myFont2 = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "³ª´®°íµñÃ¼");
			HFONT oldFont2 = (HFONT)SelectObject(hdc, myFont2);
			SetTextColor(hdc, RGB(0, 0, 0));
			sprintf_s(str, playerInfo.weaponName);
			TextOut(hdc, 960, 438, str, strlen(str));
			sprintf_s(str, playerInfo.armorName);
			TextOut(hdc, 1040, 438, str, strlen(str));
			sprintf_s(str, "¾øÀ½");
			TextOut(hdc, 1130, 438, str, strlen(str));
			sprintf_s(str, "+%d", playerInfo.addAtk);
			TextOut(hdc, 1002, 491, str, strlen(str));
			sprintf_s(str, "+%d", playerInfo.addDef);
			TextOut(hdc, 1084, 491, str, strlen(str));
			SelectObject(hdc, oldFont2);
			DeleteObject(myFont2);
		}
	}
}

void Interface::friendInformation(HDC hdc)
{
	if (FRIENDMANAGER->getFriend()[0]->getIsClick())
	{
		IMAGEMANAGER->frameRender("¸Þ´º", hdc, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rc.left - 100, FRIENDMANAGER->getFriend()[0]->getFriendInfo().rc.top - 35, FRIENDMANAGER->getFriend()[0]->getFrameX(), 1);
	}

	for (int i = 0; i < FRIENDMANAGER->getFriend().size(); i++)
	{
		auto& friendly = FRIENDMANAGER->getFriend()[i];
		auto& friendInfo = FRIENDMANAGER->getFriend()[i]->getFriendInfo();

		if (PtInRect(&friendInfo.rc, m_ptMouse))
		{
			IMAGEMANAGER->render("Á¤º¸Ã¢", hdc, 960, 200);
			IMAGEMANAGER->render(friendInfo.face, hdc, 960, 218);
			friendly->getProgressBarHp()->render(hdc);
			friendly->getProgressBarMp()->render(hdc);
			friendly->getProgressBarExp()->render(hdc);
			friendInfo.weaponImg->render(hdc, 965, 456);
			friendInfo.armorImg->render(hdc, 1047, 456);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "³ª´®°íµñÃ¼");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(0, 0, 0));
			sprintf_s(str, friendInfo.name);
			TextOut(hdc, 970, 305, str, strlen(str));
			sprintf_s(str, friendInfo.className);
			TextOut(hdc, 1040, 220, str, strlen(str));
			sprintf_s(str, "%d", friendInfo.level);
			TextOut(hdc, 1057, 243, str, strlen(str));
			sprintf_s(str, "%d", friendInfo.atk + friendInfo.addAtk);
			TextOut(hdc, 1005, 343, str, strlen(str));
			sprintf_s(str, "%d", friendInfo.def + friendInfo.addDef);
			TextOut(hdc, 1097, 343, str, strlen(str));
			sprintf_s(str, "%d", friendInfo.ten);
			TextOut(hdc, 1170, 343, str, strlen(str));
			sprintf_s(str, "%d", friendInfo.will);
			TextOut(hdc, 1005, 378, str, strlen(str));
			sprintf_s(str, "%d", friendInfo.agi);
			TextOut(hdc, 1097, 378, str, strlen(str));
			sprintf_s(str, "%d", friendInfo.movingCount);
			TextOut(hdc, 1177, 378, str, strlen(str));
			sprintf_s(str, "%d/%d", friendly->getCurrentHp(), friendly->getMaxHp());
			TextOut(hdc, 1088, 289, str, strlen(str));
			sprintf_s(str, "%d/%d", friendly->getCurrentMp(), friendly->getMaxMp());
			TextOut(hdc, 1095, 307, str, strlen(str));
			sprintf_s(str, "%d/%d", friendly->getCurrentExp(), friendly->getMaxExp());
			TextOut(hdc, 1095, 243, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);

			HFONT myFont2 = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "³ª´®°íµñÃ¼");
			HFONT oldFont2 = (HFONT)SelectObject(hdc, myFont2);
			SetTextColor(hdc, RGB(0, 0, 0));
			sprintf_s(str, friendInfo.weaponName);
			TextOut(hdc, 960, 438, str, strlen(str));
			sprintf_s(str, friendInfo.armorName);
			TextOut(hdc, 1040, 438, str, strlen(str));
			sprintf_s(str, "¾øÀ½");
			TextOut(hdc, 1130, 438, str, strlen(str));
			sprintf_s(str, "+%d", friendInfo.addAtk);
			TextOut(hdc, 1002, 491, str, strlen(str));
			sprintf_s(str, "+%d", friendInfo.addDef);
			TextOut(hdc, 1084, 491, str, strlen(str));
			SelectObject(hdc, oldFont2);
			DeleteObject(myFont2);
		}
	}
}

void Interface::enemyInformation(HDC hdc)
{
	//¿©Æ÷
	if (ENEMYMANAGER->getEnemy()[4]->getIsClick())
	{
		IMAGEMANAGER->frameRender("¸Þ´º", hdc, ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rc.left - 100, ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rc.top - 35, ENEMYMANAGER->getEnemy()[4]->getFrameX(), 0);
	}

	for (int i = 0; i < ENEMYMANAGER->getEnemy().size(); i++)
	{
		auto& enemy = ENEMYMANAGER->getEnemy()[i];
		auto& enemyInfo = ENEMYMANAGER->getEnemy()[i]->getEnemyInfo();

		if (PtInRect(&enemyInfo.rc, m_ptMouse))
		{
			IMAGEMANAGER->render("Á¤º¸Ã¢", hdc, 960, 200);
			IMAGEMANAGER->render(enemyInfo.face, hdc, 960, 218);
			enemy->getProgressBarHp()->render(hdc);
			enemy->getProgressBarMp()->render(hdc);
			enemy->getProgressBarExp()->render(hdc);
			enemyInfo.weaponImg->render(hdc, 965, 456);
			enemyInfo.armorImg->render(hdc, 1047, 456);
			if (i == 4) enemyInfo.asstImg->render(hdc, 1134, 453);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "³ª´®°íµñÃ¼");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(0, 0, 0));
			sprintf_s(str, enemyInfo.name);
			TextOut(hdc, 970, 305, str, strlen(str));
			sprintf_s(str, enemyInfo.className);
			TextOut(hdc, 1040, 220, str, strlen(str));
			sprintf_s(str, "%d", enemyInfo.level);
			TextOut(hdc, 1057, 243, str, strlen(str));
			sprintf_s(str, "%d", enemyInfo.atk + enemyInfo.addAtk);
			TextOut(hdc, 1005, 343, str, strlen(str));
			sprintf_s(str, "%d", enemyInfo.def + enemyInfo.addDef);
			TextOut(hdc, 1097, 343, str, strlen(str));
			sprintf_s(str, "%d", enemyInfo.ten);
			TextOut(hdc, 1170, 343, str, strlen(str));
			sprintf_s(str, "%d", enemyInfo.will);
			TextOut(hdc, 1005, 378, str, strlen(str));
			sprintf_s(str, "%d", enemyInfo.agi);
			TextOut(hdc, 1097, 378, str, strlen(str));
			sprintf_s(str, "%d", enemyInfo.movingCount);
			TextOut(hdc, 1177, 378, str, strlen(str));
			sprintf_s(str, "%d/%d", enemy->getCurrentHp(), enemy->getMaxHp());
			TextOut(hdc, 1088, 289, str, strlen(str));
			sprintf_s(str, "%d/%d", enemy->getCurrentMp(), enemy->getMaxMp());
			TextOut(hdc, 1095, 307, str, strlen(str));
			sprintf_s(str, "%d/%d", enemy->getCurrentExp(), enemy->getMaxExp());
			TextOut(hdc, 1095, 243, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);

			HFONT myFont2 = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "³ª´®°íµñÃ¼");
			HFONT oldFont2 = (HFONT)SelectObject(hdc, myFont2);
			SetTextColor(hdc, RGB(0, 0, 0));
			sprintf_s(str, enemyInfo.weaponName);
			TextOut(hdc, 960, 438, str, strlen(str));
			sprintf_s(str, enemyInfo.armorName);
			TextOut(hdc, 1040, 438, str, strlen(str));
			if (i == 4)
			{
				sprintf_s(str, enemyInfo.asstName);
				TextOut(hdc, 1128, 438, str, strlen(str));
				sprintf_s(str, "µ¹ÆÄ°ø°Ý +35");
				TextOut(hdc, 1125, 491, str, strlen(str));
			}
			else
			{
				sprintf_s(str, "¾øÀ½");
				TextOut(hdc, 1130, 438, str, strlen(str));
			}
			sprintf_s(str, "+%d", enemyInfo.addAtk);
			TextOut(hdc, 1002, 491, str, strlen(str));
			sprintf_s(str, "+%d", enemyInfo.addDef);
			TextOut(hdc, 1084, 491, str, strlen(str));
			SelectObject(hdc, oldFont2);
			DeleteObject(myFont2);
		}
	}
}
