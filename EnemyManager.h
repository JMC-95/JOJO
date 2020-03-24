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
	vector<EnemyBase*> vEnemy;

public:
	EnemyManager();
	~EnemyManager();

	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	//���̽� Ŭ������ ���͸� ��ȯ�ϴ� �Լ�
	vector<EnemyBase*>& getEnemy() { return vEnemy; }
};

