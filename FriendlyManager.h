#pragma once
#include "singletonBase.h"
#include "Dogyeom.h"
#include "Gwanu.h"
#include "Jangbi.h"
#include "Soldier.h"
#include "Wonso.h"
#include "Yubi.h"

class FriendlyManager : public singletonBase<FriendlyManager>
{
private:
	vector<FriendlyBase*> vFriend;

	bool fTurn;

public:
	FriendlyManager();
	~FriendlyManager();

	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	void friendTurn();

	bool getFturn() { return fTurn; }

	//���̽� Ŭ������ ���͸� ��ȯ�ϴ� �Լ�
	vector<FriendlyBase*>& getFriend() { return vFriend; }
};
