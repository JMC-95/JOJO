#pragma once
#include "PlayerBase.h"

class Ijeon : public PlayerBase
{
private:
	PlayerInfo ijeon;

public:
	Ijeon();
	~Ijeon();

	//�̵�����, �̵��� ���ݹ���, ������ ���ݹ���, ĳ���� �̹���, ���ݽ� �̹���, ��� or �ǰ� �̹���
	virtual HRESULT init(const char* moveImg, const char* mAtkImg, const char* aRngImg, const char* playerImg, const char* atkImg, const char* blockImg);
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

	virtual PlayerInfo getPlayerInfo() { return ijeon; }
};
