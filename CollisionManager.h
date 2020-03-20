#pragma once
#include "singletonBase.h"

class CollisionManager : public singletonBase<CollisionManager>
{
private:
	RECT temp;

public:
	CollisionManager();
	~CollisionManager();

	HRESULT init();

	void playerToEnemyCollision();
};
