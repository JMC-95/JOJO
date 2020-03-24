#pragma once
#include "EnemyBase.h"

class Yeopo : public EnemyBase
{
private:
	EnemyInfo yeopo;

public:
	Yeopo();
	~Yeopo();

	//이동범위, 이동시 공격범위, 전투시 공격범위, 캐릭터 이미지, 공격시 이미지, 방어 or 피격 이미지
	virtual HRESULT init(const char* moveImg, const char* mAtkImg, const char* aRngImg, const char* playerImg, const char* atkImg, const char* blockImg);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

	virtual void mouseMove();
	virtual void enemyMove();
	virtual void enemyAstar();
	virtual void enemyMenu();
	virtual void enemyCollision();
	virtual void enemyAnimation();
	virtual void enemyState();

	virtual void setPosition(RECT rc);

	virtual EnemyInfo getEnemyInfo() { return yeopo; }
};
