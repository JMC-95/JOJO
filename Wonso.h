#pragma once
#include "FriendlyBase.h"

class Wonso : public FriendlyBase
{
private:
	FriendInfo wonso;

public:
	Wonso();
	~Wonso();

	//�̵�����, �̵��� ���ݹ���, ������ ���ݹ���, ĳ���� �̹���, ���ݽ� �̹���, ��� or �ǰ� �̹���
	virtual HRESULT init(const char* moveImg, const char* mAtkImg, const char* aRngImg, const char* playerImg, const char* atkImg, const char* blockImg);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

	virtual void friendAi();
	virtual void friendMove();
	virtual void friendAstar();
	virtual void friendMenu();
	virtual void friendCollision();
	virtual void friendAnimation();
	virtual void friendState();
	virtual void mouseMove();

	virtual void setPosition(RECT rc);

	virtual FriendInfo getFriendInfo() { return wonso; }
};
