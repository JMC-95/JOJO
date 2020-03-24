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
	for (int j = 0; j < PLAYERMANAGER->getPlayer().size() - 1; j++)
	{
		if (PLAYERMANAGER->getPlayer()[j]->getIsClick())
		{
			IMAGEMANAGER->frameRender("메뉴", hdc, PLAYERMANAGER->getPlayer()[j]->getPlayerInfo().rc.left - 100, PLAYERMANAGER->getPlayer()[j]->getPlayerInfo().rc.top - 35, PLAYERMANAGER->getPlayer()[j]->getFrameX(), 0);
		}
	}

	if (PLAYERMANAGER->getPlayer()[6]->getIsClick())
	{
		IMAGEMANAGER->frameRender("메뉴", hdc, PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rc.left - 100, PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rc.top - 35, PLAYERMANAGER->getPlayer()[6]->getFrameX(), 1);
	}
}

void Interface::playerInformation(HDC hdc)
{
	//악진
	if (PtInRect(&PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("악진Face", hdc, 960, 218);
		PLAYERMANAGER->getPlayer()[0]->getProgressBarHp()->render(hdc);
		PLAYERMANAGER->getPlayer()[0]->getProgressBarMp()->render(hdc);
		PLAYERMANAGER->getPlayer()[0]->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "악진");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "경보병");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getPlayer()[0]->getCurrentHp(), PLAYERMANAGER->getPlayer()[0]->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getPlayer()[0]->getCurrentMp(), PLAYERMANAGER->getPlayer()[0]->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getPlayer()[0]->getCurrentExp(), PLAYERMANAGER->getPlayer()[0]->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//하후돈
	if (PtInRect(&PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("하후돈Face", hdc, 960, 218);
		PLAYERMANAGER->getPlayer()[1]->getProgressBarHp()->render(hdc);
		PLAYERMANAGER->getPlayer()[1]->getProgressBarMp()->render(hdc);
		PLAYERMANAGER->getPlayer()[1]->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "하후돈");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "경기병");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getPlayer()[1]->getCurrentHp(), PLAYERMANAGER->getPlayer()[1]->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getPlayer()[1]->getCurrentMp(), PLAYERMANAGER->getPlayer()[1]->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getPlayer()[1]->getCurrentExp(), PLAYERMANAGER->getPlayer()[1]->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//하후연
	if (PtInRect(&PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("하후연Face", hdc, 960, 218);
		PLAYERMANAGER->getPlayer()[2]->getProgressBarHp()->render(hdc);
		PLAYERMANAGER->getPlayer()[2]->getProgressBarMp()->render(hdc);
		PLAYERMANAGER->getPlayer()[2]->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "하후연");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "경궁병");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getPlayer()[2]->getCurrentHp(), PLAYERMANAGER->getPlayer()[2]->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getPlayer()[2]->getCurrentMp(), PLAYERMANAGER->getPlayer()[2]->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getPlayer()[2]->getCurrentExp(), PLAYERMANAGER->getPlayer()[2]->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//이전
	if (PtInRect(&PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("이전Face", hdc, 960, 218);
		PLAYERMANAGER->getPlayer()[3]->getProgressBarHp()->render(hdc);
		PLAYERMANAGER->getPlayer()[3]->getProgressBarMp()->render(hdc);
		PLAYERMANAGER->getPlayer()[3]->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "이전");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "경보병");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getPlayer()[3]->getCurrentHp(), PLAYERMANAGER->getPlayer()[3]->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getPlayer()[3]->getCurrentMp(), PLAYERMANAGER->getPlayer()[3]->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getPlayer()[3]->getCurrentExp(), PLAYERMANAGER->getPlayer()[3]->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//조홍
	if (PtInRect(&PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("조홍Face", hdc, 960, 218);
		PLAYERMANAGER->getPlayer()[4]->getProgressBarHp()->render(hdc);
		PLAYERMANAGER->getPlayer()[4]->getProgressBarMp()->render(hdc);
		PLAYERMANAGER->getPlayer()[4]->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "조홍");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "경보병");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getPlayer()[4]->getCurrentHp(), PLAYERMANAGER->getPlayer()[4]->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getPlayer()[4]->getCurrentMp(), PLAYERMANAGER->getPlayer()[4]->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getPlayer()[4]->getCurrentExp(), PLAYERMANAGER->getPlayer()[4]->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//조인
	if (PtInRect(&PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("조인Face", hdc, 960, 218);
		PLAYERMANAGER->getPlayer()[5]->getProgressBarHp()->render(hdc);
		PLAYERMANAGER->getPlayer()[5]->getProgressBarMp()->render(hdc);
		PLAYERMANAGER->getPlayer()[5]->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "조인");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "경기병");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getPlayer()[5]->getCurrentHp(), PLAYERMANAGER->getPlayer()[5]->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getPlayer()[5]->getCurrentMp(), PLAYERMANAGER->getPlayer()[5]->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getPlayer()[5]->getCurrentExp(), PLAYERMANAGER->getPlayer()[5]->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//조조
	if (PtInRect(&PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("조조Face", hdc, 960, 218);
		PLAYERMANAGER->getPlayer()[6]->getProgressBarHp()->render(hdc);
		PLAYERMANAGER->getPlayer()[6]->getProgressBarMp()->render(hdc);
		PLAYERMANAGER->getPlayer()[6]->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "조조");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "군웅");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getPlayer()[6]->getCurrentHp(), PLAYERMANAGER->getPlayer()[6]->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getPlayer()[6]->getCurrentMp(), PLAYERMANAGER->getPlayer()[6]->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getPlayer()[6]->getCurrentExp(), PLAYERMANAGER->getPlayer()[6]->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}
}

void Interface::friendInformation(HDC hdc)
{
	//도겸
	if (PtInRect(&FRIENDMANAGER->getFriend()[0]->getFriendInfo().rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("도겸Face", hdc, 960, 218);
		FRIENDMANAGER->getFriend()[0]->getProgressBarHp()->render(hdc);
		FRIENDMANAGER->getFriend()[0]->getProgressBarMp()->render(hdc);
		FRIENDMANAGER->getFriend()[0]->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "도겸");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "군웅");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[0]->getFriendInfo().level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[0]->getFriendInfo().atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[0]->getFriendInfo().def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[0]->getFriendInfo().ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[0]->getFriendInfo().will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[0]->getFriendInfo().agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[0]->getFriendInfo().movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getFriend()[0]->getCurrentHp(), FRIENDMANAGER->getFriend()[0]->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getFriend()[0]->getCurrentMp(), FRIENDMANAGER->getFriend()[0]->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getFriend()[0]->getCurrentExp(), FRIENDMANAGER->getFriend()[0]->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//관우
	if (PtInRect(&FRIENDMANAGER->getFriend()[1]->getFriendInfo().rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("관우Face", hdc, 960, 218);
		FRIENDMANAGER->getFriend()[1]->getProgressBarHp()->render(hdc);
		FRIENDMANAGER->getFriend()[1]->getProgressBarMp()->render(hdc);
		FRIENDMANAGER->getFriend()[1]->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "관우");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "경기병");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[1]->getFriendInfo().level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[1]->getFriendInfo().atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[1]->getFriendInfo().def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[1]->getFriendInfo().ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[1]->getFriendInfo().will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[1]->getFriendInfo().agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[1]->getFriendInfo().movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getFriend()[1]->getCurrentHp(), FRIENDMANAGER->getFriend()[1]->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getFriend()[1]->getCurrentMp(), FRIENDMANAGER->getFriend()[1]->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getFriend()[1]->getCurrentExp(), FRIENDMANAGER->getFriend()[1]->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//장비
	if (PtInRect(&FRIENDMANAGER->getFriend()[2]->getFriendInfo().rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("장비Face", hdc, 960, 218);
		FRIENDMANAGER->getFriend()[2]->getProgressBarHp()->render(hdc);
		FRIENDMANAGER->getFriend()[2]->getProgressBarMp()->render(hdc);
		FRIENDMANAGER->getFriend()[2]->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "장비");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "경기병");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[2]->getFriendInfo().level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[2]->getFriendInfo().atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[2]->getFriendInfo().def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[2]->getFriendInfo().ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[2]->getFriendInfo().will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[2]->getFriendInfo().agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[2]->getFriendInfo().movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getFriend()[2]->getCurrentHp(), FRIENDMANAGER->getFriend()[2]->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getFriend()[2]->getCurrentMp(), FRIENDMANAGER->getFriend()[2]->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getFriend()[2]->getCurrentExp(), FRIENDMANAGER->getFriend()[2]->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//원소
	if (PtInRect(&FRIENDMANAGER->getFriend()[3]->getFriendInfo().rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("원소Face", hdc, 960, 218);
		FRIENDMANAGER->getFriend()[3]->getProgressBarHp()->render(hdc);
		FRIENDMANAGER->getFriend()[3]->getProgressBarMp()->render(hdc);
		FRIENDMANAGER->getFriend()[3]->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "원소");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "군웅");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[3]->getFriendInfo().level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[3]->getFriendInfo().atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[3]->getFriendInfo().def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[3]->getFriendInfo().ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[3]->getFriendInfo().will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[3]->getFriendInfo().agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[3]->getFriendInfo().movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getFriend()[3]->getCurrentHp(), FRIENDMANAGER->getFriend()[3]->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getFriend()[3]->getCurrentMp(), FRIENDMANAGER->getFriend()[3]->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getFriend()[3]->getCurrentExp(), FRIENDMANAGER->getFriend()[3]->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//유비
	if (PtInRect(&FRIENDMANAGER->getFriend()[4]->getFriendInfo().rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("유비Face", hdc, 960, 218);
		FRIENDMANAGER->getFriend()[4]->getProgressBarHp()->render(hdc);
		FRIENDMANAGER->getFriend()[4]->getProgressBarMp()->render(hdc);
		FRIENDMANAGER->getFriend()[4]->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "유비");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "군웅");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[4]->getFriendInfo().level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[4]->getFriendInfo().atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[4]->getFriendInfo().def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[4]->getFriendInfo().ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[4]->getFriendInfo().will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[4]->getFriendInfo().agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[4]->getFriendInfo().movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getFriend()[4]->getCurrentHp(), FRIENDMANAGER->getFriend()[4]->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getFriend()[4]->getCurrentMp(), FRIENDMANAGER->getFriend()[4]->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getFriend()[4]->getCurrentExp(), FRIENDMANAGER->getFriend()[4]->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//우군보병
	for (int j = 5; j < 9; j++)
	{
		if (PtInRect(&FRIENDMANAGER->getFriend()[j]->getFriendInfo().rc, m_ptMouse))
		{
			IMAGEMANAGER->render("정보창", hdc, 960, 200);
			IMAGEMANAGER->render("우군보병Face", hdc, 960, 218);
			FRIENDMANAGER->getFriend()[j]->getProgressBarHp()->render(hdc);
			FRIENDMANAGER->getFriend()[j]->getProgressBarMp()->render(hdc);
			FRIENDMANAGER->getFriend()[j]->getProgressBarExp()->render(hdc);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(0, 0, 0));
			sprintf_s(str, "보병");
			TextOut(hdc, 970, 305, str, strlen(str));
			sprintf_s(str, "경보병");
			TextOut(hdc, 1040, 220, str, strlen(str));
			sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[j]->getFriendInfo().level);
			TextOut(hdc, 1057, 243, str, strlen(str));
			sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[j]->getFriendInfo().atk);
			TextOut(hdc, 1005, 343, str, strlen(str));
			sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[j]->getFriendInfo().def);
			TextOut(hdc, 1097, 343, str, strlen(str));
			sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[j]->getFriendInfo().ten);
			TextOut(hdc, 1170, 343, str, strlen(str));
			sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[j]->getFriendInfo().will);
			TextOut(hdc, 1005, 378, str, strlen(str));
			sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[j]->getFriendInfo().agi);
			TextOut(hdc, 1097, 378, str, strlen(str));
			sprintf_s(str, "%d", FRIENDMANAGER->getFriend()[j]->getFriendInfo().movingCount);
			TextOut(hdc, 1177, 378, str, strlen(str));
			sprintf_s(str, "%d/%d", FRIENDMANAGER->getFriend()[j]->getCurrentHp(), FRIENDMANAGER->getFriend()[j]->getMaxHp());
			TextOut(hdc, 1088, 289, str, strlen(str));
			sprintf_s(str, "%d/%d", FRIENDMANAGER->getFriend()[j]->getCurrentMp(), FRIENDMANAGER->getFriend()[j]->getMaxMp());
			TextOut(hdc, 1095, 307, str, strlen(str));
			sprintf_s(str, "%d/%d", FRIENDMANAGER->getFriend()[j]->getCurrentExp(), FRIENDMANAGER->getFriend()[j]->getMaxExp());
			TextOut(hdc, 1095, 243, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
	}
}

void Interface::enemyInformation(HDC hdc)
{
	//여포
	if (ENEMYMANAGER->getEnemy()[4]->getIsClick())
	{
		IMAGEMANAGER->frameRender("메뉴", hdc, ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rc.left - 100, ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rc.top - 35, ENEMYMANAGER->getEnemy()[4]->getFrameX(), 0);
	}

	//번주
	if (PtInRect(&ENEMYMANAGER->getEnemy()[0]->getEnemyInfo().rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("번주Face", hdc, 960, 218);
		ENEMYMANAGER->getEnemy()[0]->getProgressBarHp()->render(hdc);
		ENEMYMANAGER->getEnemy()[0]->getProgressBarMp()->render(hdc);
		ENEMYMANAGER->getEnemy()[0]->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "번주");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "경기병");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[0]->getEnemyInfo().level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[0]->getEnemyInfo().atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[0]->getEnemyInfo().def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[0]->getEnemyInfo().ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[0]->getEnemyInfo().will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[0]->getEnemyInfo().agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[0]->getEnemyInfo().movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getEnemy()[0]->getCurrentHp(), ENEMYMANAGER->getEnemy()[0]->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getEnemy()[0]->getCurrentMp(), ENEMYMANAGER->getEnemy()[0]->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getEnemy()[0]->getCurrentExp(), ENEMYMANAGER->getEnemy()[0]->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//동탁
	if (PtInRect(&ENEMYMANAGER->getEnemy()[1]->getEnemyInfo().rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("동탁Face", hdc, 960, 218);
		ENEMYMANAGER->getEnemy()[1]->getProgressBarHp()->render(hdc);
		ENEMYMANAGER->getEnemy()[1]->getProgressBarMp()->render(hdc);
		ENEMYMANAGER->getEnemy()[1]->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "동탁");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "군웅");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[1]->getEnemyInfo().level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[1]->getEnemyInfo().atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[1]->getEnemyInfo().def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[1]->getEnemyInfo().ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[1]->getEnemyInfo().will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[1]->getEnemyInfo().agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[1]->getEnemyInfo().movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getEnemy()[1]->getCurrentHp(), ENEMYMANAGER->getEnemy()[1]->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getEnemy()[1]->getCurrentMp(), ENEMYMANAGER->getEnemy()[1]->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getEnemy()[1]->getCurrentExp(), ENEMYMANAGER->getEnemy()[1]->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//이유
	if (PtInRect(&ENEMYMANAGER->getEnemy()[2]->getEnemyInfo().rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("이유Face", hdc, 960, 218);
		ENEMYMANAGER->getEnemy()[2]->getProgressBarHp()->render(hdc);
		ENEMYMANAGER->getEnemy()[2]->getProgressBarMp()->render(hdc);
		ENEMYMANAGER->getEnemy()[2]->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "이유");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "책사");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[2]->getEnemyInfo().level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[2]->getEnemyInfo().atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[2]->getEnemyInfo().def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[2]->getEnemyInfo().ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[2]->getEnemyInfo().will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[2]->getEnemyInfo().agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[2]->getEnemyInfo().movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getEnemy()[2]->getCurrentHp(), ENEMYMANAGER->getEnemy()[2]->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getEnemy()[2]->getCurrentMp(), ENEMYMANAGER->getEnemy()[2]->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getEnemy()[2]->getCurrentExp(), ENEMYMANAGER->getEnemy()[2]->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//장제
	if (PtInRect(&ENEMYMANAGER->getEnemy()[3]->getEnemyInfo().rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("장제Face", hdc, 960, 218);
		ENEMYMANAGER->getEnemy()[3]->getProgressBarHp()->render(hdc);
		ENEMYMANAGER->getEnemy()[3]->getProgressBarMp()->render(hdc);
		ENEMYMANAGER->getEnemy()[3]->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "장제");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "경기병");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[3]->getEnemyInfo().level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[3]->getEnemyInfo().atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[3]->getEnemyInfo().def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[3]->getEnemyInfo().ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[3]->getEnemyInfo().will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[3]->getEnemyInfo().agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[3]->getEnemyInfo().movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getEnemy()[3]->getCurrentHp(), ENEMYMANAGER->getEnemy()[3]->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getEnemy()[3]->getCurrentMp(), ENEMYMANAGER->getEnemy()[3]->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getEnemy()[3]->getCurrentExp(), ENEMYMANAGER->getEnemy()[3]->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//여포
	if (PtInRect(&ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("여포Face", hdc, 960, 218);
		ENEMYMANAGER->getEnemy()[4]->getProgressBarHp()->render(hdc);
		ENEMYMANAGER->getEnemy()[4]->getProgressBarMp()->render(hdc);
		ENEMYMANAGER->getEnemy()[4]->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "여포");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "서량기병");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getEnemy()[4]->getCurrentHp(), ENEMYMANAGER->getEnemy()[4]->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getEnemy()[4]->getCurrentMp(), ENEMYMANAGER->getEnemy()[4]->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getEnemy()[4]->getCurrentExp(), ENEMYMANAGER->getEnemy()[4]->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}
}
