#pragma once
#include "PlayerBase.h"

class Hahuyeon : public PlayerBase
{
private:
	vector<PlayerInfo> vHahuyeon;
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
	void playerAnimation();

	void setPosition(RECT rc);

	//���͸� ��ȯ�ϴ� �Լ�
	vector<PlayerInfo> getPlayerVector() { return vHahuyeon; }
};
