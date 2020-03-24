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
	//����
	if (PLAYERMANAGER->getAgjin()->getIsClick())
	{
		IMAGEMANAGER->frameRender("�޴�", hdc, PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rc.left - 100, PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rc.top - 35, PLAYERMANAGER->getAgjin()->getFrameX(), 0);
	}

	//���ĵ�
	if (PLAYERMANAGER->getHahudon()->getIsClick())
	{
		IMAGEMANAGER->frameRender("�޴�", hdc, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc.left - 100, PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc.top - 35, PLAYERMANAGER->getHahudon()->getFrameX(), 0);
	}

	//���Ŀ�
	if (PLAYERMANAGER->getHahuyeon()->getIsClick())
	{
		IMAGEMANAGER->frameRender("�޴�", hdc, PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rc.left - 100, PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rc.top - 35, PLAYERMANAGER->getHahuyeon()->getFrameX(), 0);
	}

	//����
	if (PLAYERMANAGER->getIjeon()->getIsClick())
	{
		IMAGEMANAGER->frameRender("�޴�", hdc, PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rc.left - 100, PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rc.top - 35, PLAYERMANAGER->getIjeon()->getFrameX(), 0);
	}

	//��ȫ
	if (PLAYERMANAGER->getJohong()->getIsClick())
	{
		IMAGEMANAGER->frameRender("�޴�", hdc, PLAYERMANAGER->getJohong()->getPlayerVector()[0].rc.left - 100, PLAYERMANAGER->getJohong()->getPlayerVector()[0].rc.top - 35, PLAYERMANAGER->getJohong()->getFrameX(), 0);
	}

	//����
	if (PLAYERMANAGER->getJoin()->getIsClick())
	{
		IMAGEMANAGER->frameRender("�޴�", hdc, PLAYERMANAGER->getJoin()->getPlayerVector()[0].rc.left - 100, PLAYERMANAGER->getJoin()->getPlayerVector()[0].rc.top - 35, PLAYERMANAGER->getJoin()->getFrameX(), 0);
	}

	//����
	if (PLAYERMANAGER->getJojo()->getIsClick())
	{
		IMAGEMANAGER->frameRender("�޴�", hdc, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rc.left - 100, PLAYERMANAGER->getJojo()->getPlayerVector()[0].rc.top - 35, PLAYERMANAGER->getJojo()->getFrameX(), 1);
	}
}

void Interface::playerInformation(HDC hdc)
{
	//����
	if (PtInRect(&PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("����â", hdc, 960, 200);
		IMAGEMANAGER->render("����Face", hdc, 960, 218);
		PLAYERMANAGER->getAgjin()->getProgressBarHp()->render(hdc);
		PLAYERMANAGER->getAgjin()->getProgressBarMp()->render(hdc);
		PLAYERMANAGER->getAgjin()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "����");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "�溸��");
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

	//���ĵ�
	if (PtInRect(&PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("����â", hdc, 960, 200);
		IMAGEMANAGER->render("���ĵ�Face", hdc, 960, 218);
		PLAYERMANAGER->getHahudon()->getProgressBarHp()->render(hdc);
		PLAYERMANAGER->getHahudon()->getProgressBarMp()->render(hdc);
		PLAYERMANAGER->getHahudon()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "���ĵ�");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "��⺴");
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

	//���Ŀ�
	if (PtInRect(&PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("����â", hdc, 960, 200);
		IMAGEMANAGER->render("���Ŀ�Face", hdc, 960, 218);
		PLAYERMANAGER->getHahuyeon()->getProgressBarHp()->render(hdc);
		PLAYERMANAGER->getHahuyeon()->getProgressBarMp()->render(hdc);
		PLAYERMANAGER->getHahuyeon()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "���Ŀ�");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "��ú�");
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

	//����
	if (PtInRect(&PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("����â", hdc, 960, 200);
		IMAGEMANAGER->render("����Face", hdc, 960, 218);
		PLAYERMANAGER->getIjeon()->getProgressBarHp()->render(hdc);
		PLAYERMANAGER->getIjeon()->getProgressBarMp()->render(hdc);
		PLAYERMANAGER->getIjeon()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "����");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "�溸��");
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

	//��ȫ
	if (PtInRect(&PLAYERMANAGER->getJohong()->getPlayerVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("����â", hdc, 960, 200);
		IMAGEMANAGER->render("��ȫFace", hdc, 960, 218);
		PLAYERMANAGER->getJohong()->getProgressBarHp()->render(hdc);
		PLAYERMANAGER->getJohong()->getProgressBarMp()->render(hdc);
		PLAYERMANAGER->getJohong()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "��ȫ");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "�溸��");
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

	//����
	if (PtInRect(&PLAYERMANAGER->getJoin()->getPlayerVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("����â", hdc, 960, 200);
		IMAGEMANAGER->render("����Face", hdc, 960, 218);
		PLAYERMANAGER->getJoin()->getProgressBarHp()->render(hdc);
		PLAYERMANAGER->getJoin()->getProgressBarMp()->render(hdc);
		PLAYERMANAGER->getJoin()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "����");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "��⺴");
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

	//����
	if (PtInRect(&PLAYERMANAGER->getJojo()->getPlayerVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("����â", hdc, 960, 200);
		IMAGEMANAGER->render("����Face", hdc, 960, 218);
		PLAYERMANAGER->getJojo()->getProgressBarHp()->render(hdc);
		PLAYERMANAGER->getJojo()->getProgressBarMp()->render(hdc);
		PLAYERMANAGER->getJojo()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "����");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "����");
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

void Interface::friendInformation(HDC hdc)
{
	//����
	if (PtInRect(&FRIENDMANAGER->getDogyeom()->getFriendVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("����â", hdc, 960, 200);
		IMAGEMANAGER->render("����Face", hdc, 960, 218);
		FRIENDMANAGER->getDogyeom()->getProgressBarHp()->render(hdc);
		FRIENDMANAGER->getDogyeom()->getProgressBarMp()->render(hdc);
		FRIENDMANAGER->getDogyeom()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "����");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "����");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getDogyeom()->getFriendVector()[0].level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getDogyeom()->getFriendVector()[0].atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getDogyeom()->getFriendVector()[0].def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getDogyeom()->getFriendVector()[0].ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getDogyeom()->getFriendVector()[0].will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getDogyeom()->getFriendVector()[0].agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getDogyeom()->getFriendVector()[0].movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getDogyeom()->getCurrentHp(), FRIENDMANAGER->getDogyeom()->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getDogyeom()->getCurrentMp(), FRIENDMANAGER->getDogyeom()->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getDogyeom()->getCurrentExp(), FRIENDMANAGER->getDogyeom()->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//����
	if (PtInRect(&FRIENDMANAGER->getGwanu()->getFriendVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("����â", hdc, 960, 200);
		IMAGEMANAGER->render("����Face", hdc, 960, 218);
		FRIENDMANAGER->getGwanu()->getProgressBarHp()->render(hdc);
		FRIENDMANAGER->getGwanu()->getProgressBarMp()->render(hdc);
		FRIENDMANAGER->getGwanu()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "����");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "��⺴");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getGwanu()->getFriendVector()[0].level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getGwanu()->getFriendVector()[0].atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getGwanu()->getFriendVector()[0].def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getGwanu()->getFriendVector()[0].ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getGwanu()->getFriendVector()[0].will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getGwanu()->getFriendVector()[0].agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getGwanu()->getFriendVector()[0].movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getGwanu()->getCurrentHp(), FRIENDMANAGER->getGwanu()->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getGwanu()->getCurrentMp(), FRIENDMANAGER->getGwanu()->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getGwanu()->getCurrentExp(), FRIENDMANAGER->getGwanu()->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//���
	if (PtInRect(&FRIENDMANAGER->getJangbi()->getFriendVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("����â", hdc, 960, 200);
		IMAGEMANAGER->render("���Face", hdc, 960, 218);
		FRIENDMANAGER->getJangbi()->getProgressBarHp()->render(hdc);
		FRIENDMANAGER->getJangbi()->getProgressBarMp()->render(hdc);
		FRIENDMANAGER->getJangbi()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "���");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "��⺴");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getJangbi()->getFriendVector()[0].level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getJangbi()->getFriendVector()[0].atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getJangbi()->getFriendVector()[0].def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getJangbi()->getFriendVector()[0].ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getJangbi()->getFriendVector()[0].will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getJangbi()->getFriendVector()[0].agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getJangbi()->getFriendVector()[0].movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getJangbi()->getCurrentHp(), FRIENDMANAGER->getJangbi()->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getJangbi()->getCurrentMp(), FRIENDMANAGER->getJangbi()->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getJangbi()->getCurrentExp(), FRIENDMANAGER->getJangbi()->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//����
	if (PtInRect(&FRIENDMANAGER->getWonso()->getFriendVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("����â", hdc, 960, 200);
		IMAGEMANAGER->render("����Face", hdc, 960, 218);
		FRIENDMANAGER->getWonso()->getProgressBarHp()->render(hdc);
		FRIENDMANAGER->getWonso()->getProgressBarMp()->render(hdc);
		FRIENDMANAGER->getWonso()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "����");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "����");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getWonso()->getFriendVector()[0].level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getWonso()->getFriendVector()[0].atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getWonso()->getFriendVector()[0].def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getWonso()->getFriendVector()[0].ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getWonso()->getFriendVector()[0].will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getWonso()->getFriendVector()[0].agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getWonso()->getFriendVector()[0].movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getWonso()->getCurrentHp(), FRIENDMANAGER->getWonso()->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getWonso()->getCurrentMp(), FRIENDMANAGER->getWonso()->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getWonso()->getCurrentExp(), FRIENDMANAGER->getWonso()->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//����
	if (PtInRect(&FRIENDMANAGER->getYubi()->getFriendVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("����â", hdc, 960, 200);
		IMAGEMANAGER->render("����Face", hdc, 960, 218);
		FRIENDMANAGER->getYubi()->getProgressBarHp()->render(hdc);
		FRIENDMANAGER->getYubi()->getProgressBarMp()->render(hdc);
		FRIENDMANAGER->getYubi()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "����");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "����");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getYubi()->getFriendVector()[0].level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getYubi()->getFriendVector()[0].atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getYubi()->getFriendVector()[0].def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getYubi()->getFriendVector()[0].ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getYubi()->getFriendVector()[0].will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getYubi()->getFriendVector()[0].agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", FRIENDMANAGER->getYubi()->getFriendVector()[0].movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getYubi()->getCurrentHp(), FRIENDMANAGER->getYubi()->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getYubi()->getCurrentMp(), FRIENDMANAGER->getYubi()->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", FRIENDMANAGER->getYubi()->getCurrentExp(), FRIENDMANAGER->getYubi()->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}
}

void Interface::enemyInformation(HDC hdc)
{
	//����
	if (ENEMYMANAGER->getYeopo()->getIsClick())
	{
		IMAGEMANAGER->frameRender("�޴�", hdc, ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc.left - 100, ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc.top - 35, ENEMYMANAGER->getYeopo()->getFrameX(), 0);
	}

	//����
	if (PtInRect(&ENEMYMANAGER->getBeonju()->getEnemyVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("����â", hdc, 960, 200);
		IMAGEMANAGER->render("����Face", hdc, 960, 218);
		ENEMYMANAGER->getBeonju()->getProgressBarHp()->render(hdc);
		ENEMYMANAGER->getBeonju()->getProgressBarMp()->render(hdc);
		ENEMYMANAGER->getBeonju()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "����");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "��⺴");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getBeonju()->getEnemyVector()[0].level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getBeonju()->getEnemyVector()[0].atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getBeonju()->getEnemyVector()[0].def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getBeonju()->getEnemyVector()[0].ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getBeonju()->getEnemyVector()[0].will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getBeonju()->getEnemyVector()[0].agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getBeonju()->getEnemyVector()[0].movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getBeonju()->getCurrentHp(), ENEMYMANAGER->getBeonju()->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getBeonju()->getCurrentMp(), ENEMYMANAGER->getBeonju()->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getBeonju()->getCurrentExp(), ENEMYMANAGER->getBeonju()->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//��Ź
	if (PtInRect(&ENEMYMANAGER->getDongtak()->getEnemyVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("����â", hdc, 960, 200);
		IMAGEMANAGER->render("��ŹFace", hdc, 960, 218);
		ENEMYMANAGER->getDongtak()->getProgressBarHp()->render(hdc);
		ENEMYMANAGER->getDongtak()->getProgressBarMp()->render(hdc);
		ENEMYMANAGER->getDongtak()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "��Ź");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "����");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getDongtak()->getEnemyVector()[0].level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getDongtak()->getEnemyVector()[0].atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getDongtak()->getEnemyVector()[0].def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getDongtak()->getEnemyVector()[0].ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getDongtak()->getEnemyVector()[0].will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getDongtak()->getEnemyVector()[0].agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getDongtak()->getEnemyVector()[0].movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getDongtak()->getCurrentHp(), ENEMYMANAGER->getDongtak()->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getDongtak()->getCurrentMp(), ENEMYMANAGER->getDongtak()->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getDongtak()->getCurrentExp(), ENEMYMANAGER->getDongtak()->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//����
	if (PtInRect(&ENEMYMANAGER->getIyu()->getEnemyVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("����â", hdc, 960, 200);
		IMAGEMANAGER->render("����Face", hdc, 960, 218);
		ENEMYMANAGER->getIyu()->getProgressBarHp()->render(hdc);
		ENEMYMANAGER->getIyu()->getProgressBarMp()->render(hdc);
		ENEMYMANAGER->getIyu()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "����");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "å��");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getIyu()->getEnemyVector()[0].level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getIyu()->getEnemyVector()[0].atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getIyu()->getEnemyVector()[0].def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getIyu()->getEnemyVector()[0].ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getIyu()->getEnemyVector()[0].will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getIyu()->getEnemyVector()[0].agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getIyu()->getEnemyVector()[0].movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getIyu()->getCurrentHp(), ENEMYMANAGER->getIyu()->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getIyu()->getCurrentMp(), ENEMYMANAGER->getIyu()->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getIyu()->getCurrentExp(), ENEMYMANAGER->getIyu()->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//����
	if (PtInRect(&ENEMYMANAGER->getJangje()->getEnemyVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("����â", hdc, 960, 200);
		IMAGEMANAGER->render("����Face", hdc, 960, 218);
		ENEMYMANAGER->getJangje()->getProgressBarHp()->render(hdc);
		ENEMYMANAGER->getJangje()->getProgressBarMp()->render(hdc);
		ENEMYMANAGER->getJangje()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "����");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "��⺴");
		TextOut(hdc, 1040, 220, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getJangje()->getEnemyVector()[0].level);
		TextOut(hdc, 1057, 243, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getJangje()->getEnemyVector()[0].atk);
		TextOut(hdc, 1005, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getJangje()->getEnemyVector()[0].def);
		TextOut(hdc, 1097, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getJangje()->getEnemyVector()[0].ten);
		TextOut(hdc, 1170, 343, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getJangje()->getEnemyVector()[0].will);
		TextOut(hdc, 1005, 378, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getJangje()->getEnemyVector()[0].agi);
		TextOut(hdc, 1097, 378, str, strlen(str));
		sprintf_s(str, "%d", ENEMYMANAGER->getJangje()->getEnemyVector()[0].movingCount);
		TextOut(hdc, 1177, 378, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getJangje()->getCurrentHp(), ENEMYMANAGER->getJangje()->getMaxHp());
		TextOut(hdc, 1088, 289, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getJangje()->getCurrentMp(), ENEMYMANAGER->getJangje()->getMaxMp());
		TextOut(hdc, 1095, 307, str, strlen(str));
		sprintf_s(str, "%d/%d", ENEMYMANAGER->getJangje()->getCurrentExp(), ENEMYMANAGER->getJangje()->getMaxExp());
		TextOut(hdc, 1095, 243, str, strlen(str));
		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
	}

	//����
	if (PtInRect(&ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc, m_ptMouse))
	{
		IMAGEMANAGER->render("����â", hdc, 960, 200);
		IMAGEMANAGER->render("����Face", hdc, 960, 218);
		ENEMYMANAGER->getYeopo()->getProgressBarHp()->render(hdc);
		ENEMYMANAGER->getYeopo()->getProgressBarMp()->render(hdc);
		ENEMYMANAGER->getYeopo()->getProgressBarExp()->render(hdc);

		HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�������ü");
		HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		sprintf_s(str, "����");
		TextOut(hdc, 970, 305, str, strlen(str));
		sprintf_s(str, "�����⺴");
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
