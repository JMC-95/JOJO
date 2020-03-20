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

	//ĳ���� Ŭ������ ��ȯ�ϴ� �Լ�
	Yeopo* getYeopo() { return m_Yeopo; }
};

