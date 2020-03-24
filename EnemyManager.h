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

	//베이스 클래스의 벡터를 반환하는 함수
	vector<EnemyBase*>& getEnemy() { return vEnemy; }
};

