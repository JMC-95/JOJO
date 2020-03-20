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
	m_Yeopo = new Yeopo;
	m_Yeopo->init("eMoveRange", "attackRange", "atkRange", "여포", "여포ATK", "여포BH");

	return S_OK;
}

void EnemyManager::release()
{
	SAFE_DELETE(m_Yeopo);
}

void EnemyManager::update()
{
	m_Yeopo->update();
}

void EnemyManager::render(HDC hdc)
{
	m_Yeopo->render(hdc);
}
