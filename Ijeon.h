#pragma once
#include "PlayerBase.h"

class Ijeon : public PlayerBase
{
public:
	Ijeon();
	~Ijeon();

	//�̵�����, �̵��� ���ݹ���, ������ ���ݹ���, ĳ���� �̹���, ���ݽ� �̹���, ��� or �ǰ� �̹���
	virtual HRESULT init(const char* moveImg, const char* mAtkImg, const char* aRngImg,
		const char* playerImg, const char* atkImg, const char* blockImg);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

	void mouseMove();
	void playerMove();
	void playerAnimation();

	void setPosition(RECT rc);
};
