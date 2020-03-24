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
	//m_Archer = new Archer;
	//m_Archer->init("eMoveRange", "attackRange", "atkRange", "���ú�", "���ú�ATK", "���ú�BH");

	m_Beonju = new Beonju;
	m_Beonju->init("eMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	//m_Cavalry = new Cavalry;
	//m_Cavalry->init("eMoveRange", "attackRange", "atkRange", "���⺴", "���⺴ATK", "���⺴BH");

	m_Dongtak = new Dongtak;
	m_Dongtak->init("eMoveRange", "attackRange", "atkRange", "��Ź", "��ŹATK", "��ŹBH");

	//m_Infantry = new Infantry;
	//m_Infantry->init("eMoveRange", "attackRange", "atkRange", "������", "������ATK", "������BH");

	m_Iyu = new Iyu;
	m_Iyu->init("eMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	m_Jangje = new Jangje;
	m_Jangje->init("eMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	m_Yeopo = new Yeopo;
	m_Yeopo->init("eMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	return S_OK;
}

void EnemyManager::release()
{
	//SAFE_DELETE(m_Archer);
	SAFE_DELETE(m_Beonju);
	//SAFE_DELETE(m_Cavalry);
	SAFE_DELETE(m_Dongtak);
	//SAFE_DELETE(m_Infantry);
	SAFE_DELETE(m_Iyu);
	SAFE_DELETE(m_Jangje);
	SAFE_DELETE(m_Yeopo);
}

void EnemyManager::update()
{
	//m_Archer->update();
	m_Beonju->update();
	//m_Cavalry->update();
	m_Dongtak->update();
	//m_Infantry->update();
	m_Iyu->update();
	m_Jangje->update();
	m_Yeopo->update();
}

void EnemyManager::render(HDC hdc)
{
	//m_Archer->render(hdc);
	m_Beonju->render(hdc);
	//m_Cavalry->render(hdc);
	m_Dongtak->render(hdc);
	//m_Infantry->render(hdc);
	m_Iyu->render(hdc);
	m_Jangje->render(hdc);
	m_Yeopo->render(hdc);
}
