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

	Dogyeom* m_Dogyeom;
	Gwanu* m_Gwanu;
	Jangbi* m_Jangbi;
	//Soldier* m_Soldier;
	Wonso* m_Wonso;
	Yubi* m_Yubi;

public:
	FriendlyManager();
	~FriendlyManager();

	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	//캐릭터 클래스를 반환하는 함수
	Dogyeom* getDogyeom() { return m_Dogyeom; }
	Gwanu* getGwanu() { return m_Gwanu; }
	Jangbi* getJangbi() { return m_Jangbi; }
	//Soldier* getSoldier() { return m_Soldier; }
	Wonso* getWonso() { return m_Wonso; }
	Yubi* getYubi() { return m_Yubi; }

	int nSoldier;

	//베이스 클래스의 벡터를 반환하는 함수
	vector<FriendlyBase*> getFriend() { return vFriend; }
};
