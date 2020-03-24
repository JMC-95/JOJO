#pragma once
#include "PlayerBase.h"

class Hahuyeon : public PlayerBase
{
private:
	PlayerInfo hahuyeon;

public:
	Hahuyeon();
	~Hahuyeon();

	//�̵�����, �̵��� ���ݹ���, ������ ���ݹ���, ĳ���� �̹���, ���ݽ� �̹���, ��� or �ǰ� �̹���
	virtual HRESULT init(const char* moveImg, const char* mAtkImg, const char* aRngImg, const char* playerImg, const char* atkImg, const char* blockImg);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

	void mouseMove();
	void playerMove();
	void playerAstar();
	void playerMenu();
	void playerCollision();
	void playerAnimation();
	void playerState();

	void setPosition(RECT rc);

	virtual PlayerInfo getPlayerInfo() { return hahuyeon; }
};
