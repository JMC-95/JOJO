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
	vEnemy[0]->init("eMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	vEnemy.push_back(new Dongtak);
	vEnemy[1]->init("eMoveRange", "attackRange", "atkRange", "��Ź", "��ŹATK", "��ŹBH");

	vEnemy.push_back(new Iyu);
	vEnemy[2]->init("eMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	vEnemy.push_back(new Jangje);
	vEnemy[3]->init("eMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	for (int i = 4; i < 14; i++)
	{
		vEnemy.push_back(new Archer(i));
		vEnemy[i]->init("eMoveRange", "attackRange", "atkRange", "���ú�", "���ú�ATK", "���ú�BH");
	}

	for (int i = 14; i < 18; i++)
	{
		vEnemy.push_back(new Infantry(i));
		vEnemy[i]->init("eMoveRange", "attackRange", "atkRange", "������", "������ATK", "������BH");
	}

	for (int i = 18; i < 22; i++)
	{
		vEnemy.push_back(new Cavalry(i));
		vEnemy[i]->init("eMoveRange", "attackRange", "atkRange", "���⺴", "���⺴ATK", "���⺴BH");
	}

	vEnemy.push_back(new Yeopo);
	vEnemy[22]->init("eMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

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

		for (int i = 9; i < ENEMYMANAGER->getEnemy().size(); i++)
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
