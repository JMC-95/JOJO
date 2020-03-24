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

public:
	PlayerManager();
	~PlayerManager();

	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	//베이스 클래스의 벡터를 반환하는 함수
	vector<PlayerBase*>& getPlayer() { return vPlayer; }
};
