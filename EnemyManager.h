#pragma once
#include "singletonBase.h"
#include "Dongtak.h"
#include "Iyu.h"
#include "Jangje.h"
#include "Beonju.h"
#include "Yeopo.h"
#include "Infantry.h"
#include "Archer.h"
#include "Cavalry.h"

class EnemyManager : public singletonBase<EnemyManager>
{
private:
	Dongtak* m_Dongtak;
	Iyu* m_Iyu;
	Jangje* m_Jangje;
	Beonju* m_Beonju;
	Yeopo* m_Yeopo;
	Infantry* m_Infantry;
	Archer* m_Archer;
	Cavalry* m_Cavalry;

public:
	EnemyManager();
	~EnemyManager();

	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	//캐릭터 클래스를 반환하는 함수
	Dongtak* getDongtak() { return m_Dongtak; }
	Iyu* getIyu() { return m_Iyu; }
	Jangje* getJangje() { return m_Jangje; }
	Beonju* getBeonju() { return m_Beonju; }
	Yeopo* getYeopo() { return m_Yeopo; }
	Infantry* getInfantry() { return m_Infantry; }
	Archer* getArcher() { return m_Archer; }
	Cavalry* getCavalry() { return m_Cavalry; }
};

