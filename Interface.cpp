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
	enemyInformation(hdc);
}

void Interface::playerMenu(HDC hdc)
{
	//악진
	if (PLAYERMANAGER->getAgjin()->getIsClick())
	{
		IMAGEMANAGER->frameRender("메뉴", hdc, PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rc.left - 100, PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rc.top - 35, PLAYERMANAGER->getAgjin()->getFrameX(), 0);
	}

	//하후돈
	if (PLAYERMANAGER->getHahudon()->getIsClick())
	{
		IMAGEMANAGER->frameRender("메뉴", hdc, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc.left - 100, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc.top - 35, PLAYERMANAGER->getHahudon()->getFrameX(), 0);
	}

	//하후연
	if (PLAYERMANAGER->getHahuyeon()->getIsClick())
	{
		IMAGEMANAGER->frameRender("메뉴", hdc, PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rc.left - 100, PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rc.top - 35, PLAYERMANAGER->getHahuyeon()->getFrameX(), 0);
	}

	//이전
	if (PLAYERMANAGER->getIjeon()->getIsClick())
	{
		IMAGEMANAGER->frameRender("메뉴", hdc, PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rc.left - 100, PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rc.top - 35, PLAYERMANAGER->getIjeon()->getFrameX(), 0);
	}

	//조홍
	if (PLAYERMANAGER->getJohong()->getIsClick())
	{
		IMAGEMANAGER->frameRender("메뉴", hdc, PLAYERMANAGER->getJohong()->getPlayerVector()[0].rc.left - 100, PLAYERMANAGER->getJohong()->getPlayerVector()[0].rc.top - 35, PLAYERMANAGER->getJohong()->getFrameX(), 0);
	}

	//조인
	if (PLAYERMANAGER->getJoin()->getIsClick())
	{
		IMAGEMANAGER->frameRender("메뉴", hdc, PLAYERMANAGER->getJoin()->getPlayerVector()[0].rc.left - 100, PLAYERMANAGER->getJoin()->getPlayerVector()[0].rc.top - 35, PLAYERMANAGER->getJoin()->getFrameX(), 0);
	}

	//조조
	if (PLAYERMANAGER->getJojo()->getIsClick())
	{
		IMAGEMANAGER->frameRender("메뉴", hdc, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rc.left - 100, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rc.top - 35, PLAYERMANAGER->getJojo()->getFrameX(), 0);
	}
}

void Interface::playerInformation(HDC hdc)
{
	//악진
	if (PtInRect(&PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("악진Face", hdc, 960, 218);
		PLAYERMANAGER->getAgjin()->getProgressBarHp()->render(hdc);
		PLAYERMANAGER->getAgjin()->getProgressBarMp()->render(hdc);
		PLAYERMANAGER->getAgjin()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "악진");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "경보병");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getAgjin()->getPlayerVector()[0].level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getAgjin()->getPlayerVector()[0].atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getAgjin()->getPlayerVector()[0].def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getAgjin()->getPlayerVector()[0].ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getAgjin()->getPlayerVector()[0].will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getAgjin()->getPlayerVector()[0].agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getAgjin()->getPlayerVector()[0].movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getAgjin()->getCurrentHp(), PLAYERMANAGER->getAgjin()->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getAgjin()->getCurrentMp(), PLAYERMANAGER->getAgjin()->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getAgjin()->getCurrentExp(), PLAYERMANAGER->getAgjin()->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//하후돈
	if (PtInRect(&PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("하후돈Face", hdc, 960, 218);
		PLAYERMANAGER->getHahudon()->getProgressBarHp()->render(hdc);
		PLAYERMANAGER->getHahudon()->getProgressBarMp()->render(hdc);
		PLAYERMANAGER->getHahudon()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "하후돈");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "경기병");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getHahudon()->getPlayerVector()[0].level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getHahudon()->getPlayerVector()[0].atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getHahudon()->getPlayerVector()[0].def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getHahudon()->getPlayerVector()[0].ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getHahudon()->getPlayerVector()[0].will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getHahudon()->getPlayerVector()[0].agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getHahudon()->getPlayerVector()[0].movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getHahudon()->getCurrentHp(), PLAYERMANAGER->getHahudon()->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getHahudon()->getCurrentMp(), PLAYERMANAGER->getHahudon()->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getHahudon()->getCurrentExp(), PLAYERMANAGER->getHahudon()->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//하후연
	if (PtInRect(&PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("하후연Face", hdc, 960, 218);
		PLAYERMANAGER->getHahuyeon()->getProgressBarHp()->render(hdc);
		PLAYERMANAGER->getHahuyeon()->getProgressBarMp()->render(hdc);
		PLAYERMANAGER->getHahuyeon()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "하후연");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "경궁병");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getHahuyeon()->getCurrentHp(), PLAYERMANAGER->getHahuyeon()->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getHahuyeon()->getCurrentMp(), PLAYERMANAGER->getHahuyeon()->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getHahuyeon()->getCurrentExp(), PLAYERMANAGER->getHahuyeon()->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//이전
	if (PtInRect(&PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("이전Face", hdc, 960, 218);
		PLAYERMANAGER->getIjeon()->getProgressBarHp()->render(hdc);
		PLAYERMANAGER->getIjeon()->getProgressBarMp()->render(hdc);
		PLAYERMANAGER->getIjeon()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "이전");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "경보병");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getIjeon()->getPlayerVector()[0].level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getIjeon()->getPlayerVector()[0].atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getIjeon()->getPlayerVector()[0].def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getIjeon()->getPlayerVector()[0].ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getIjeon()->getPlayerVector()[0].will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getIjeon()->getPlayerVector()[0].agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getIjeon()->getPlayerVector()[0].movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getIjeon()->getCurrentHp(), PLAYERMANAGER->getIjeon()->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getIjeon()->getCurrentMp(), PLAYERMANAGER->getIjeon()->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getIjeon()->getCurrentExp(), PLAYERMANAGER->getIjeon()->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//조홍
	if (PtInRect(&PLAYERMANAGER->getJohong()->getPlayerVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("조홍Face", hdc, 960, 218);
		PLAYERMANAGER->getJohong()->getProgressBarHp()->render(hdc);
		PLAYERMANAGER->getJohong()->getProgressBarMp()->render(hdc);
		PLAYERMANAGER->getJohong()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "조홍");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "경보병");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getJohong()->getPlayerVector()[0].level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getJohong()->getPlayerVector()[0].atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getJohong()->getPlayerVector()[0].def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getJohong()->getPlayerVector()[0].ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getJohong()->getPlayerVector()[0].will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getJohong()->getPlayerVector()[0].agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getJohong()->getPlayerVector()[0].movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getJohong()->getCurrentHp(), PLAYERMANAGER->getJohong()->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getJohong()->getCurrentMp(), PLAYERMANAGER->getJohong()->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getJohong()->getCurrentExp(), PLAYERMANAGER->getJohong()->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//조인
	if (PtInRect(&PLAYERMANAGER->getJoin()->getPlayerVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("조인Face", hdc, 960, 218);
		PLAYERMANAGER->getJoin()->getProgressBarHp()->render(hdc);
		PLAYERMANAGER->getJoin()->getProgressBarMp()->render(hdc);
		PLAYERMANAGER->getJoin()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "조인");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "경기병");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getJoin()->getPlayerVector()[0].level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getJoin()->getPlayerVector()[0].atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getJoin()->getPlayerVector()[0].def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getJoin()->getPlayerVector()[0].ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getJoin()->getPlayerVector()[0].will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getJoin()->getPlayerVector()[0].agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getJoin()->getPlayerVector()[0].movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getJoin()->getCurrentHp(), PLAYERMANAGER->getJoin()->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getJoin()->getCurrentMp(), PLAYERMANAGER->getJoin()->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getJoin()->getCurrentExp(), PLAYERMANAGER->getJoin()->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//조조
	if (PtInRect(&PLAYERMANAGER->getJojo()->getPlayerVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("조조Face", hdc, 960, 218);
		PLAYERMANAGER->getJojo()->getProgressBarHp()->render(hdc);
		PLAYERMANAGER->getJojo()->getProgressBarMp()->render(hdc);
		PLAYERMANAGER->getJojo()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "조조");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "군웅");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getJojo()->getPlayerVector()[0].level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getJojo()->getPlayerVector()[0].atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getJojo()->getPlayerVector()[0].def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getJojo()->getPlayerVector()[0].ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getJojo()->getPlayerVector()[0].will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getJojo()->getPlayerVector()[0].agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", PLAYERMANAGER->getJojo()->getPlayerVector()[0].movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getJojo()->getCurrentHp(), PLAYERMANAGER->getJojo()->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getJojo()->getCurrentMp(), PLAYERMANAGER->getJojo()->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", PLAYERMANAGER->getJojo()->getCurrentExp(), PLAYERMANAGER->getJojo()->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}
}

void Interface::enemyInformation(HDC hdc)
{
	//여포
	if (ENEMYMANAGER->getYeopo()->getIsClick())
	{
		IMAGEMANAGER->frameRender("메뉴", hdc, ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc.left - 100, ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc.top - 35, ENEMYMANAGER->getYeopo()->getFrameX(), 0);
	}

	if (PtInRect(&ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("정보창", hdc, 960, 200);
		IMAGEMANAGER->render("여포Face", hdc, 960, 218);
		ENEMYMANAGER->getYeopo()->getProgressBarHp()->render(hdc);
		ENEMYMANAGER->getYeopo()->getProgressBarMp()->render(hdc);
		ENEMYMANAGER->getYeopo()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "여포");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "서량기병");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getYeopo()->getEnemyVector()[0].level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getYeopo()->getEnemyVector()[0].atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getYeopo()->getEnemyVector()[0].def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getYeopo()->getEnemyVector()[0].ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getYeopo()->getEnemyVector()[0].will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getYeopo()->getEnemyVector()[0].agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getYeopo()->getEnemyVector()[0].movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getYeopo()->getCurrentHp(), ENEMYMANAGER->getYeopo()->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getYeopo()->getCurrentMp(), ENEMYMANAGER->getYeopo()->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getYeopo()->getCurrentExp(), ENEMYMANAGER->getYeopo()->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}
}
