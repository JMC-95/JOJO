#pragma once
#include "FriendlyBase.h"

class Wonso : public FriendlyBase
{
private:
	FriendInfo wonso;

public:
	Wonso();
	~Wonso();

	//이동범위, 이동시 공격범위, 전투시 공격범위, 캐릭터 이미지, 공격시 이미지, 방어 or 피격 이미지
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
