#pragma once
#include "EnemyBase.h"

class Archer : public EnemyBase
{
private:
	EnemyInfo archer;

public:
	Archer(int num);
	~Archer();

	//�̵�����, �̵��� ���ݹ���, ������ ���ݹ���, ĳ���� �̹���, ���ݽ� �̹���, ��� or �ǰ� �̹���
	virtual HRESULT init(const char* moveImg, const char* mAtkImg, const char* aRngImg, const char* playerImg, const char* atkImg, const char* blockImg);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

	virtual void enemyAi();
	virtual void enemyMove();
	virtual void enemyAstar();
	virtual void enemyMenu();
	virtual void enemyCollision();
	virtual void enemyAnimation();
	virtual void enemyState();
	virtual void mouseMove();

	virtual void setPosition(RECT rc);

	virtual EnemyInfo getEnemyInfo() { return archer; }
};
