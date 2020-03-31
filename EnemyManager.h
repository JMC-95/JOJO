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

	bool eTurn;
	bool isClear;

public:
	EnemyManager();
	~EnemyManager();

	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	void enemyTurn();

	bool getEturn() { return eTurn; }
	bool getIsClear() { return isClear; }

	//���̽� Ŭ������ ���͸� ��ȯ�ϴ� �Լ�
	vector<EnemyBase*>& getEnemy() { return vEnemy; }
};

