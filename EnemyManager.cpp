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

	return S_OK;
}

void EnemyManager::release()
{
}

void EnemyManager::update()
{
	for (int i = 0; i < vEnemy.size(); i++)
	{
		vEnemy[i]->update();
	}
}

void EnemyManager::render(HDC hdc)
{
	for (int i = 0; i < vEnemy.size(); i++)
	{
		vEnemy[i]->render(hdc);
	}
}
