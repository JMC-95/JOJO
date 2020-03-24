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
	Agjin* m_Agjin;
	Hahudon* m_Hahudon;
	Hahuyeon* m_Hahuyeon;
	Ijeon* m_Ijeon;
	Johong* m_Johong;
	Join* m_Join;
	Jojo* m_Jojo;
	
public:
	PlayerManager();
	~PlayerManager();

	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	//캐릭터 클래스를 반환하는 함수
	Agjin* getAgjin() { return m_Agjin; }
	Hahudon* getHahudon() { return m_Hahudon; }
	Hahuyeon* getHahuyeon() { return m_Hahuyeon; }
	Ijeon* getIjeon() { return m_Ijeon; }
	Johong* getJohong() { return m_Johong; }
	Join* getJoin() { return m_Join; }
	Jojo* getJojo() { return m_Jojo; }
};
