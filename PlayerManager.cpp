#include "stdafx.h"
#include "PlayerManager.h"

PlayerManager::PlayerManager()
{
}

PlayerManager::~PlayerManager()
{
}

HRESULT PlayerManager::init()
{
	m_Jojo = new Jojo;
	m_Jojo->init("pMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	m_Hahudon = new Hahudon;
	m_Hahudon->init("pMoveRange", "attackRange", "atkRange", "���ĵ�", "���ĵ�ATK", "���ĵ�BH");

	m_Hahuyeon = new Hahuyeon;
	m_Hahuyeon->init("pMoveRange", "attackRange", "atkRange", "���Ŀ�", "���Ŀ�ATK", "���Ŀ�BH");

	m_Join = new Join;
	m_Join->init("pMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	m_Johong = new Johong;
	m_Johong->init("pMoveRange", "attackRange", "atkRange", "��ȫ", "��ȫATK", "��ȫBH");

	m_Ijeon = new Ijeon;
	m_Ijeon->init("pMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	m_Agjin = new Agjin;
	m_Agjin->init("pMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	return S_OK;
}

void PlayerManager::release()
{
	SAFE_DELETE(m_Jojo);
	SAFE_DELETE(m_Hahudon);
	SAFE_DELETE(m_Hahuyeon);
	SAFE_DELETE(m_Join);
	SAFE_DELETE(m_Johong);
	SAFE_DELETE(m_Ijeon);
	SAFE_DELETE(m_Agjin);
}

void PlayerManager::update()
{
	m_Jojo->update();
	m_Hahudon->update();
	m_Hahuyeon->update();
	m_Join->update();
	m_Johong->update();
	m_Ijeon->update();
	m_Agjin->update();
}

void PlayerManager::render(HDC hdc)
{
	m_Jojo->render(hdc);
	m_Hahudon->render(hdc);
	m_Hahuyeon->render(hdc);
	m_Join->render(hdc);
	m_Johong->render(hdc);
	m_Ijeon->render(hdc);
	m_Agjin->render(hdc);
}
