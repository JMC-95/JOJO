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

public:
	FriendlyManager();
	~FriendlyManager();

	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	//베이스 클래스의 벡터를 반환하는 함수
	vector<FriendlyBase*>& getFriend() { return vFriend; }
};
