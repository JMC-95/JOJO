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
	vPlayer[0]->init("pMoveRange", "attackRange", "atkRange", "¾ÇÁø", "¾ÇÁøATK", "¾ÇÁøBH");

	vPlayer.push_back(new Hahudon);
	vPlayer[1]->init("pMoveRange", "attackRange", "atkRange", "ÇÏÈÄµ·", "ÇÏÈÄµ·ATK", "ÇÏÈÄµ·BH");

	vPlayer.push_back(new Hahuyeon);
	vPlayer[2]->init("pMoveRange", "attackRange", "atkRange", "ÇÏÈÄ¿¬", "ÇÏÈÄ¿¬ATK", "ÇÏÈÄ¿¬BH");

	vPlayer.push_back(new Ijeon);
	vPlayer[3]->init("pMoveRange", "attackRange", "atkRange", "ÀÌÀü", "ÀÌÀüATK", "ÀÌÀüBH");

	vPlayer.push_back(new Johong);
	vPlayer[4]->init("pMoveRange", "attackRange", "atkRange", "Á¶È«", "Á¶È«ATK", "Á¶È«BH");

	vPlayer.push_back(new Join);
	vPlayer[5]->init("pMoveRange", "attackRange", "atkRange", "Á¶ÀÎ", "Á¶ÀÎATK", "Á¶ÀÎBH");

	vPlayer.push_back(new Jojo);
	vPlayer[6]->init("pMoveRange", "attackRange", "atkRange", "Á¶Á¶", "Á¶Á¶ATK", "Á¶Á¶BH");

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
