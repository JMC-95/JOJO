#pragma once
#include "PlayerBase.h"

class Jojo : public PlayerBase
{
private:
	PlayerInfo jojo;

private: //스킬
	vector<RECT> vSkill;

	RECT skillRect[2];

	int playerNumber;	//캐릭터 넘버
	int skillCount;		//스킬 이미지를 돌리기 위한 카운트
	int frameCount;		//프레임 이미지를 돌리기 위한 카운트

public:
	Jojo();
	~Jojo();

	//이동범위, 이동시 공격범위, 전투시 공격범위, 캐릭터 이미지, 공격시 이미지, 방어 or 피격 이미지
	virtual HRESULT init(const char* skillImg, const char* moveImg, const char* mAtkImg, const char* aRngImg, const char* playerImg, const char* atkImg, const char* blockImg);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

	virtual void mouseMove();
	virtual void playerMove();
	virtual void playerAstar();
	virtual void playerMenu();
	virtual void playerCollision();
	virtual void playerAnimation();
	virtual void playerState();

	virtual void setPosition(RECT rc);

	virtual PlayerInfo getPlayerInfo() { return jojo; }
};
