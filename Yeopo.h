#pragma once
#include "EnemyBase.h"

class Yeopo : public EnemyBase
{
private:
	vector<EnemyInfo> vYeopo;

public:
	Yeopo();
	~Yeopo();

	//�̵�����, �̵��� ���ݹ���, ������ ���ݹ���, ĳ���� �̹���, ���ݽ� �̹���, ��� or �ǰ� �̹���
	virtual HRESULT init(const char* moveImg, const char* mAtkImg, const char* aRngImg, const char* playerImg, const char* atkImg, const char* blockImg);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

	void mouseMove();
	void enemyMove();
	void enemyAnimation();
	void enemyState();

	void setPosition(RECT rc);

	//���͸� ��ȯ�ϴ� �Լ�
	vector<EnemyInfo> getEnemyVector() { return vYeopo; }
};