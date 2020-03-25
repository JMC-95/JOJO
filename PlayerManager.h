#pragma once
#include "singletonBase.h"
#include "Agjin.h"
#include "Hahudon.h"
#include "Hahuyeon.h"
#include "Ijeon.h"
#include "Johong.h"
#include "Join.h"
#include "Jojo.h"

class PlayerManager : public singletonBase<PlayerManager>
{
private:
	vector<PlayerBase*> vPlayer;

	bool pTurn;

public:
	PlayerManager();
	~PlayerManager();

	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	void playerTurn();

	bool getPturn() { return pTurn; }

	//���̽� Ŭ������ ���͸� ��ȯ�ϴ� �Լ�
	vector<PlayerBase*>& getPlayer() { return vPlayer; }
};
