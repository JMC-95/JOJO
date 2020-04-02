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
	vPlayer.push_back(new Jojo);
	vPlayer[0]->init("heal", "pMoveRange", "attackRange", "atkRange", "Á¶Á¶", "Á¶Á¶ATK", "Á¶Á¶BH");

	vPlayer.push_back(new Hahudon);
	vPlayer[1]->init("heal", "pMoveRange", "attackRange", "atkRange", "ÇÏÈÄµ·", "ÇÏÈÄµ·ATK", "ÇÏÈÄµ·BH");

	vPlayer.push_back(new Hahuyeon);
	vPlayer[2]->init("heal", "pMoveRange", "attackRange", "atkRange", "ÇÏÈÄ¿¬", "ÇÏÈÄ¿¬ATK", "ÇÏÈÄ¿¬BH");

	vPlayer.push_back(new Join);
	vPlayer[3]->init("heal", "pMoveRange", "attackRange", "atkRange", "Á¶ÀÎ", "Á¶ÀÎATK", "Á¶ÀÎBH");

	vPlayer.push_back(new Agjin);
	vPlayer[4]->init("heal", "pMoveRange", "attackRange", "atkRange", "¾ÇÁø", "¾ÇÁøATK", "¾ÇÁøBH");

	vPlayer.push_back(new Ijeon);
	vPlayer[5]->init("heal", "pMoveRange", "attackRange", "atkRange", "ÀÌÀü", "ÀÌÀüATK", "ÀÌÀüBH");

	vPlayer.push_back(new Johong);
	vPlayer[6]->init("heal", "pMoveRange", "attackRange", "atkRange", "Á¶È«", "Á¶È«ATK", "Á¶È«BH");

	return S_OK;
}

void PlayerManager::release()
{
}

void PlayerManager::update()
{
	playerTurn();

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

void PlayerManager::playerTurn()
{
	if (!ENEMYMANAGER->getEturn())
	{
		pTurn = false;

		for (int i = 0; i < PLAYERMANAGER->getPlayer().size(); ++i)
		{
			auto& player = PLAYERMANAGER->getPlayer()[i];

			if (player->getIsTurn())
			{
				pTurn = true;
			}
		}

		for (int i = 0; i < ENEMYMANAGER->getEnemy().size(); i++)
		{
			ENEMYMANAGER->getEnemy()[i]->setIsTurn(true);
		}
	}
}
