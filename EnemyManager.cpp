#include "stdafx.h"
#include "EnemyManager.h"

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

HRESULT EnemyManager::init()
{
	vEnemy.push_back(new Beonju);
	vEnemy[0]->init("eMoveRange", "attackRange", "atkRange", "번주", "번주ATK", "번주BH");

	vEnemy.push_back(new Dongtak);
	vEnemy[1]->init("eMoveRange", "attackRange", "atkRange", "동탁", "동탁ATK", "동탁BH");

	vEnemy.push_back(new Iyu);
	vEnemy[2]->init("eMoveRange", "attackRange", "atkRange", "이유", "이유ATK", "이유BH");

	vEnemy.push_back(new Jangje);
	vEnemy[3]->init("eMoveRange", "attackRange", "atkRange", "장제", "장제ATK", "장제BH");

	vEnemy.push_back(new Yeopo);
	vEnemy[4]->init("eMoveRange", "attackRange", "atkRange", "여포", "여포ATK", "여포BH");

	for (int i = 5; i < 9; i++)
	{
		vEnemy.push_back(new Cavalry(i));
		vEnemy[i]->init("eMoveRange", "attackRange", "atkRange", "적기병", "적기병ATK", "적기병BH");
	}

	for (int i = 9; i < 13; i++)
	{
		vEnemy.push_back(new Infantry(i));
		vEnemy[i]->init("eMoveRange", "attackRange", "atkRange", "적보병", "적보병ATK", "적보병BH");
	}

	for (int i = 13; i < 23; i++)
	{
		vEnemy.push_back(new Archer(i));
		vEnemy[i]->init("eMoveRange", "attackRange", "atkRange", "적궁병", "적궁병ATK", "적궁병BH");
	}

	isClear = false;

	return S_OK;
}

void EnemyManager::release()
{
}

void EnemyManager::update()
{
	enemyTurn();

	for (int i = 0; i < vEnemy.size(); i++)
	{
		vEnemy[i]->update();
	}

	if (vEnemy.size() < 10) isClear = true;
}

void EnemyManager::render(HDC hdc)
{
	for (int i = 0; i < vEnemy.size(); i++)
	{
		vEnemy[i]->render(hdc);
	}
}

void EnemyManager::enemyTurn()
{
	if (!PLAYERMANAGER->getPturn() && !FRIENDMANAGER->getFturn())
	{
		eTurn = false;

		for (int i = 4; i < 17; i++)
		{
			auto& enemy = ENEMYMANAGER->getEnemy()[i];

			if (enemy->getIsTurn())
			{
				eTurn = true;
			}
		}

		for (int i = 0; i < PLAYERMANAGER->getPlayer().size(); i++)
		{
			PLAYERMANAGER->getPlayer()[i]->setIsTurn(true);
		}

		for (int i = 0; i < FRIENDMANAGER->getFriend().size(); i++)
		{
			FRIENDMANAGER->getFriend()[i]->setIsTurn(true);
		}
	}
}
