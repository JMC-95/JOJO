#pragma once
#include "FriendlyBase.h"

class Soldier : public FriendlyBase
{
private:
	vector<FriendInfo> vSoldier;

public:
	Soldier();
	~Soldier();

	//�̵�����, �̵��� ���ݹ���, ������ ���ݹ���, ĳ���� �̹���, ���ݽ� �̹���, ��� or �ǰ� �̹���
	virtual HRESULT init(const char* moveImg, const char* mAtkImg, const char* aRngImg, const char* playerImg, const char* atkImg, const char* blockImg);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

	virtual void mouseMove();
	virtual void friendMove();
	virtual void friendAstar();
	virtual void friendMenu();
	virtual void friendCollision();
	virtual void friendAnimation();
	virtual void friendState();

	virtual void setPosition(RECT rc);

	//���͸� ��ȯ�ϴ� �Լ�
	vector<FriendInfo> getFriendVector() { return vSoldier; }
};
