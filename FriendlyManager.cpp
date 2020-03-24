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
	m_Dogyeom->init("eMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	m_Gwanu = new Gwanu;
	m_Gwanu->init("eMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	m_Jangbi = new Jangbi;
	m_Jangbi->init("eMoveRange", "attackRange", "atkRange", "���", "���ATK", "���BH");

	//m_Soldier = new Soldier;
	//m_Soldier->init("eMoveRange", "attackRange", "atkRange", "�챺����", "�챺����ATK", "�챺����BH");	
	//vFriend.push_back(m_Soldier);
	for (int i = 0; i < 4; i++)
	{
		vFriend.push_back(new Soldier(i));
		vFriend[i]->init("eMoveRange", "attackRange", "atkRange", "�챺����", "�챺����ATK", "�챺����BH");
	}

	m_Wonso = new Wonso;
	m_Wonso->init("eMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	m_Yubi = new Yubi;
	m_Yubi->init("eMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

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
