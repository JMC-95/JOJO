#pragma once
#include "PlayerBase.h"

class Agjin : public PlayerBase
{
private:
	//vector<PlayerInfo> vAgjin;
	PlayerInfo agjin;

public:
	Agjin();
	~Agjin();

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

	//���͸� ��ȯ�ϴ� �Լ�
	//vector<PlayerInfo> getPlayerVector() { return vAgjin; }
	virtual PlayerInfo getPlayerInfo() { return agjin; }
};
