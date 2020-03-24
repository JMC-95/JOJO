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

	vPlayer.push_back(new Hahudon);
	vPlayer[1]->init("pMoveRange", "attackRange", "atkRange", "���ĵ�", "���ĵ�ATK", "���ĵ�BH");

	vPlayer.push_back(new Hahuyeon);
	vPlayer[2]->init("pMoveRange", "attackRange", "atkRange", "���Ŀ�", "���Ŀ�ATK", "���Ŀ�BH");

	vPlayer.push_back(new Ijeon);
	vPlayer[3]->init("pMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	vPlayer.push_back(new Johong);
	vPlayer[4]->init("pMoveRange", "attackRange", "atkRange", "��ȫ", "��ȫATK", "��ȫBH");

	vPlayer.push_back(new Join);
	vPlayer[5]->init("pMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	vPlayer.push_back(new Jojo);
	vPlayer[6]->init("pMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	return S_OK;
}

void PlayerManager::release()
{
}

void PlayerManager::update()
{
	for (int i = 0; i < vPlayer.size(); i++)
	{
		vPlayer[i]->update();
	}
}

void PlayerManager::render(HDC hdc)
{
	for (int i = 0; i < vPlayer.size(); i++)
	{
		vPlayer[i]->render(hdc);
	}
}
