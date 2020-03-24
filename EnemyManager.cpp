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

	vEnemy.push_back(new Yeopo);
	vEnemy[4]->init("eMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

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
