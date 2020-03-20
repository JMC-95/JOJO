#pragma once
#include "singletonBase.h"
#include "Yeopo.h"

class EnemyManager : public singletonBase<EnemyManager>
{
private:
	Yeopo* m_Yeopo;

public:
	EnemyManager();
	~EnemyManager();

	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	//캐릭터 클래스를 반환하는 함수
	Yeopo* getYeopo() { return m_Yeopo; }
};

