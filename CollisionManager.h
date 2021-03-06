#pragma once
#include "singletonBase.h"

class CollisionManager : public singletonBase<CollisionManager>
{
private:
	RECT temp;

	int damage;

public:
	CollisionManager();
	~CollisionManager();

	HRESULT init();
	void update();

	void playerToEnemyCollision();
	void enemyToPlayerCollision();

	int getDamage() { return damage; }
};
