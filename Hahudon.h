#pragma once
#include "PlayerBase.h"

class Hahudon : public PlayerBase
{
private:
	vector<PlayerInfo> vHahudon;

public:
	Hahudon();
	~Hahudon();

	//이동범위, 이동시 공격범위, 전투시 공격범위, 캐릭터 이미지, 공격시 이미지, 방어 or 피격 이미지
	virtual HRESULT init(const char* moveImg, const char* mAtkImg, const char* aRngImg, const char* playerImg, const char* atkImg, const char* blockImg);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

	void mouseMove();
	void playerMove();
	void playerAnimation();
	void playerState();

	void setPosition(RECT rc);

	//벡터를 반환하는 함수
	vector<PlayerInfo> getPlayerVector() { return vHahudon; }
};
