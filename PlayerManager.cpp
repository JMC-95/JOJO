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
	vPlayer.push_back(new Agjin);
	vPlayer[0]->init("pMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");
	//vPlayer.push_back(new Hahudon);
	//vPlayer[1]->init("pMoveRange", "attackRange", "atkRange", "���ĵ�", "���ĵ�ATK", "���ĵ�BH");
	//vPlayer.push_back(new Hahudon);
	//vPlayer[2]->init("pMoveRange", "attackRange", "atkRange", "���Ŀ�", "���Ŀ�ATK", "���Ŀ�BH");
	//vPlayer.push_back(new Ijeon);
	//vPlayer[3]->init("pMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");
	//vPlayer.push_back(new Johong);
	//vPlayer[4]->init("pMoveRange", "attackRange", "atkRange", "��ȫ", "��ȫATK", "��ȫBH");
	//vPlayer.push_back(new Join);
	//vPlayer[5]->init("pMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");
	//vPlayer.push_back(new Jojo);
	//vPlayer[6]->init("pMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	/*m_Agjin = new Agjin;
	m_Agjin->init("pMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	m_Hahudon = new Hahudon;
	m_Hahudon->init("pMoveRange", "attackRange", "atkRange", "���ĵ�", "���ĵ�ATK", "���ĵ�BH");

	m_Hahuyeon = new Hahuyeon;
	m_Hahuyeon->init("pMoveRange", "attackRange", "atkRange", "���Ŀ�", "���Ŀ�ATK", "���Ŀ�BH");

	m_Ijeon = new Ijeon;
	m_Ijeon->init("pMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	m_Johong = new Johong;
	m_Johong->init("pMoveRange", "attackRange", "atkRange", "��ȫ", "��ȫATK", "��ȫBH");

	m_Join = new Join;
	m_Join->init("pMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	m_Jojo = new Jojo;
	m_Jojo->init("pMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");*/

	return S_OK;
}

void PlayerManager::release()
{
	/*SAFE_DELETE(m_Agjin);
	SAFE_DELETE(m_Hahudon);
	SAFE_DELETE(m_Hahuyeon);
	SAFE_DELETE(m_Ijeon);
	SAFE_DELETE(m_Johong);
	SAFE_DELETE(m_Join);
	SAFE_DELETE(m_Jojo);*/
}

void PlayerManager::update()
{
	/*m_Agjin->update();
	m_Hahudon->update();
	m_Hahuyeon->update();
	m_Ijeon->update();
	m_Johong->update();
	m_Join->update();
	m_Jojo->update();*/
	for (int i = 0; i < vPlayer.size(); i++)
	{
		vPlayer[i]->update();
	}
}

void PlayerManager::render(HDC hdc)
{
	/*m_Agjin->render(hdc);
	m_Hahudon->render(hdc);
	m_Hahuyeon->render(hdc);
	m_Ijeon->render(hdc);
	m_Johong->render(hdc);
	m_Join->render(hdc);
	m_Jojo->render(hdc);*/
	for (int i = 0; i < vPlayer.size(); i++)
	{
		vPlayer[i]->render(hdc);
	}
}
