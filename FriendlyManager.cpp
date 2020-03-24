#include "stdafx.h"
#include "FriendlyManager.h"

FriendlyManager::FriendlyManager()
{
}

FriendlyManager::~FriendlyManager()
{
}

HRESULT FriendlyManager::init()
{
	m_Dogyeom = new Dogyeom;
	m_Dogyeom->init("eMoveRange", "attackRange", "atkRange", "도겸", "도겸ATK", "도겸BH");

	m_Gwanu = new Gwanu;
	m_Gwanu->init("eMoveRange", "attackRange", "atkRange", "관우", "관우ATK", "관우BH");

	m_Jangbi = new Jangbi;
	m_Jangbi->init("eMoveRange", "attackRange", "atkRange", "장비", "장비ATK", "장비BH");

	//m_Soldier = new Soldier;
	//m_Soldier->init("eMoveRange", "attackRange", "atkRange", "우군보병", "우군보병ATK", "우군보병BH");	
	//vFriend.push_back(m_Soldier);
	for (int i = 0; i < 4; i++)
	{
		vFriend.push_back(new Soldier(i));
		vFriend[i]->init("eMoveRange", "attackRange", "atkRange", "우군보병", "우군보병ATK", "우군보병BH");
	}

	m_Wonso = new Wonso;
	m_Wonso->init("eMoveRange", "attackRange", "atkRange", "원소", "원소ATK", "원소BH");

	m_Yubi = new Yubi;
	m_Yubi->init("eMoveRange", "attackRange", "atkRange", "유비", "유비ATK", "유비BH");

	return S_OK;
}

void FriendlyManager::release()
{
	SAFE_DELETE(m_Dogyeom);
	SAFE_DELETE(m_Gwanu);
	SAFE_DELETE(m_Jangbi);
	for (int i = 0; i < 4; i++)
	{
		SAFE_DELETE(vFriend[i]);
	}
	SAFE_DELETE(m_Wonso);
	SAFE_DELETE(m_Yubi);
}

void FriendlyManager::update()
{
	m_Dogyeom->update();
	m_Gwanu->update();
	m_Jangbi->update();
	for (int i = 0; i < vFriend.size(); i++)
	{
		vFriend[i]->update();
	}
	//m_Soldier->update();
	m_Wonso->update();
	m_Yubi->update();
}

void FriendlyManager::render(HDC hdc)
{
	m_Dogyeom->render(hdc);
	m_Gwanu->render(hdc);
	m_Jangbi->render(hdc);
	for (int i = 0; i < vFriend.size(); i++)
	{
		vFriend[i]->render(hdc);
	}
	//m_Soldier->render(hdc);
	m_Wonso->render(hdc);
	m_Yubi->render(hdc);
}
