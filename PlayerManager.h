#pragma once
#include "singletonBase.h"
#include "Jojo.h"
#include "Hahudon.h"
#include "Hahuyeon.h"
#include "Join.h"
#include "Johong.h"
#include "Ijeon.h"
#include "Agjin.h"

class PlayerManager : public singletonBase<PlayerManager>
{
private:
	Jojo* m_Jojo;
	Hahudon* m_Hahudon;
	Hahuyeon* m_Hahuyeon;
	Join* m_Join;
	Johong* m_Johong;
	Ijeon* m_Ijeon;
	Agjin* m_Agjin;

public:
	PlayerManager();
	~PlayerManager();

	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	//캐릭터 클래스를 반환하는 함수
	Jojo* getJojo() { return m_Jojo; }
	Hahudon* getHahudon() { return m_Hahudon; }
	Hahuyeon* getHahuyeon() { return m_Hahuyeon; }
	Join* getJoin() { return m_Join; }
	Johong* getJohong() { return m_Johong; }
	Ijeon* getIjeon() { return m_Ijeon; }
	Agjin* getAgjin() { return m_Agjin; }
};
