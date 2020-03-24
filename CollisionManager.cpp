#include "stdafx.h"
#include "CollisionManager.h"

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}

HRESULT CollisionManager::init()
{
	eHitCount = 1;
	pHitCount = 1;

	return S_OK;
}

void CollisionManager::update()
{
	playerToEnemyCollision();
	enemyToPlayerCollision();
}

void CollisionManager::playerToEnemyCollision()
{
//	//여포
//	if ((IntersectRect(&temp, &PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[0], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[1], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[2], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[3], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[4], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[5], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[6], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rcAtk[7], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc)) &&
//		PLAYERMANAGER->getAgjin()->getIsAtk())
//	{
//		ENEMYMANAGER->getYeopo()->setIsHit(true);
//		damage = (PLAYERMANAGER->getAgjin()->getPlayerVector()[0].atk - ENEMYMANAGER->getYeopo()->getEnemyVector()[0].def) / 2
//			+ PLAYERMANAGER->getAgjin()->getPlayerVector()[0].level + 25;
//		eHitCount -= 1;
//
//		if (eHitCount == 0)
//		{
//			ENEMYMANAGER->getYeopo()->hitDamage(damage);
//		}
//	}
//	else if ((IntersectRect(&temp, &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rcAtk[0], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rcAtk[1], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rcAtk[2], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rcAtk[3], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc)) &&
//		PLAYERMANAGER->getHahudon()->getIsAtk())
//	{
//		ENEMYMANAGER->getYeopo()->setIsHit(true);
//		damage = (PLAYERMANAGER->getHahudon()->getPlayerVector()[0].atk - ENEMYMANAGER->getYeopo()->getEnemyVector()[0].def) / 2
//			+ PLAYERMANAGER->getHahudon()->getPlayerVector()[0].level + 25;
//		eHitCount -= 1;
//
//		if (eHitCount == 0)
//		{
//			ENEMYMANAGER->getYeopo()->hitDamage(damage);
//		}
//	}
//	else if ((IntersectRect(&temp, &PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rcAtk[0], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rcAtk[1], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rcAtk[2], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rcAtk[3], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc)) &&
//		PLAYERMANAGER->getHahuyeon()->getIsAtk())
//	{
//		ENEMYMANAGER->getYeopo()->setIsHit(true);
//		damage = (PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].atk - ENEMYMANAGER->getYeopo()->getEnemyVector()[0].def) / 2
//			+ PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].level + 25;
//		eHitCount -= 1;
//
//		if (eHitCount == 0)
//		{
//			ENEMYMANAGER->getYeopo()->hitDamage(damage);
//		}
//	}
//	else if ((IntersectRect(&temp, &PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[0], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[1], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[2], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[3], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[4], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[5], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[6], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rcAtk[7], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc)) &&
//		PLAYERMANAGER->getIjeon()->getIsAtk())
//	{
//		ENEMYMANAGER->getYeopo()->setIsHit(true);
//		damage = (PLAYERMANAGER->getIjeon()->getPlayerVector()[0].atk - ENEMYMANAGER->getYeopo()->getEnemyVector()[0].def) / 2
//			+ PLAYERMANAGER->getIjeon()->getPlayerVector()[0].level + 25;
//		eHitCount -= 1;
//
//		if (eHitCount == 0)
//		{
//			ENEMYMANAGER->getYeopo()->hitDamage(damage);
//		}
//	}
//	else if ((IntersectRect(&temp, &PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[0], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[1], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[2], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[3], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[4], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[5], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[6], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getJohong()->getPlayerVector()[0].rcAtk[7], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc)) &&
//		PLAYERMANAGER->getJohong()->getIsAtk())
//	{
//		ENEMYMANAGER->getYeopo()->setIsHit(true);
//		damage = (PLAYERMANAGER->getJohong()->getPlayerVector()[0].atk - ENEMYMANAGER->getYeopo()->getEnemyVector()[0].def) / 2
//			+ PLAYERMANAGER->getJohong()->getPlayerVector()[0].level + 25;
//		eHitCount -= 1;
//
//		if (eHitCount == 0)
//		{
//			ENEMYMANAGER->getYeopo()->hitDamage(damage);
//		}
//	}
//	else if ((IntersectRect(&temp, &PLAYERMANAGER->getJoin()->getPlayerVector()[0].rcAtk[0], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getJoin()->getPlayerVector()[0].rcAtk[1], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getJoin()->getPlayerVector()[0].rcAtk[2], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getJoin()->getPlayerVector()[0].rcAtk[3], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc)) &&
//		PLAYERMANAGER->getJoin()->getIsAtk())
//	{
//		ENEMYMANAGER->getYeopo()->setIsHit(true);
//		damage = (PLAYERMANAGER->getJoin()->getPlayerVector()[0].atk - ENEMYMANAGER->getYeopo()->getEnemyVector()[0].def) / 2
//			+ PLAYERMANAGER->getJoin()->getPlayerVector()[0].level + 25;
//		eHitCount -= 1;
//
//		if (eHitCount == 0)
//		{
//			ENEMYMANAGER->getYeopo()->hitDamage(damage);
//		}
//	}
//	else if ((IntersectRect(&temp, &PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[0], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[1], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[2], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &PLAYERMANAGER->getJojo()->getPlayerVector()[0].rcAtk[3], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc)) &&
//		PLAYERMANAGER->getJojo()->getIsAtk())
//	{
//		ENEMYMANAGER->getYeopo()->setIsHit(true);
//		damage = (PLAYERMANAGER->getJojo()->getPlayerVector()[0].atk - ENEMYMANAGER->getYeopo()->getEnemyVector()[0].def) / 2
//			+ PLAYERMANAGER->getJojo()->getPlayerVector()[0].level + 25;
//		eHitCount -= 1;
//
//		if (eHitCount == 0)
//		{
//			ENEMYMANAGER->getYeopo()->hitDamage(damage);
//		}
//	}
//	else if ((IntersectRect(&temp, &FRIENDMANAGER->getDogyeom()->getFriendVector()[0].rcAtk[0], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &FRIENDMANAGER->getDogyeom()->getFriendVector()[0].rcAtk[1], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &FRIENDMANAGER->getDogyeom()->getFriendVector()[0].rcAtk[2], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &FRIENDMANAGER->getDogyeom()->getFriendVector()[0].rcAtk[3], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc)) &&
//		FRIENDMANAGER->getDogyeom()->getIsAtk())
//	{
//		ENEMYMANAGER->getYeopo()->setIsHit(true);
//		damage = (FRIENDMANAGER->getDogyeom()->getFriendVector()[0].atk - ENEMYMANAGER->getYeopo()->getEnemyVector()[0].def) / 2
//			+ FRIENDMANAGER->getDogyeom()->getFriendVector()[0].level + 25;
//		eHitCount -= 1;
//
//		if (eHitCount == 0)
//		{
//			ENEMYMANAGER->getYeopo()->hitDamage(damage);
//		}
//	}
//	else if ((IntersectRect(&temp, &FRIENDMANAGER->getGwanu()->getFriendVector()[0].rcAtk[0], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &FRIENDMANAGER->getGwanu()->getFriendVector()[0].rcAtk[1], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &FRIENDMANAGER->getGwanu()->getFriendVector()[0].rcAtk[2], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &FRIENDMANAGER->getGwanu()->getFriendVector()[0].rcAtk[3], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc)) &&
//		FRIENDMANAGER->getGwanu()->getIsAtk())
//	{
//		ENEMYMANAGER->getYeopo()->setIsHit(true);
//		damage = (FRIENDMANAGER->getGwanu()->getFriendVector()[0].atk - ENEMYMANAGER->getYeopo()->getEnemyVector()[0].def) / 2
//			+ FRIENDMANAGER->getGwanu()->getFriendVector()[0].level + 25;
//		eHitCount -= 1;
//
//		if (eHitCount == 0)
//		{
//			ENEMYMANAGER->getYeopo()->hitDamage(damage);
//		}
//	}
//	else if ((IntersectRect(&temp, &FRIENDMANAGER->getJangbi()->getFriendVector()[0].rcAtk[0], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &FRIENDMANAGER->getJangbi()->getFriendVector()[0].rcAtk[1], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &FRIENDMANAGER->getJangbi()->getFriendVector()[0].rcAtk[2], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &FRIENDMANAGER->getJangbi()->getFriendVector()[0].rcAtk[3], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc)) &&
//		FRIENDMANAGER->getJangbi()->getIsAtk())
//	{
//		ENEMYMANAGER->getYeopo()->setIsHit(true);
//		damage = (FRIENDMANAGER->getJangbi()->getFriendVector()[0].atk - ENEMYMANAGER->getYeopo()->getEnemyVector()[0].def) / 2
//			+ FRIENDMANAGER->getJangbi()->getFriendVector()[0].level + 25;
//		eHitCount -= 1;
//
//		if (eHitCount == 0)
//		{
//			ENEMYMANAGER->getYeopo()->hitDamage(damage);
//		}
//	}
//	else if ((IntersectRect(&temp, &FRIENDMANAGER->getWonso()->getFriendVector()[0].rcAtk[0], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &FRIENDMANAGER->getWonso()->getFriendVector()[0].rcAtk[1], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &FRIENDMANAGER->getWonso()->getFriendVector()[0].rcAtk[2], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &FRIENDMANAGER->getWonso()->getFriendVector()[0].rcAtk[3], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc)) &&
//		FRIENDMANAGER->getWonso()->getIsAtk())
//	{
//		ENEMYMANAGER->getYeopo()->setIsHit(true);
//		damage = (FRIENDMANAGER->getWonso()->getFriendVector()[0].atk - ENEMYMANAGER->getYeopo()->getEnemyVector()[0].def) / 2
//			+ FRIENDMANAGER->getWonso()->getFriendVector()[0].level + 25;
//		eHitCount -= 1;
//
//		if (eHitCount == 0)
//		{
//			ENEMYMANAGER->getYeopo()->hitDamage(damage);
//		}
//	}
//	else if ((IntersectRect(&temp, &FRIENDMANAGER->getYubi()->getFriendVector()[0].rcAtk[0], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &FRIENDMANAGER->getYubi()->getFriendVector()[0].rcAtk[1], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &FRIENDMANAGER->getYubi()->getFriendVector()[0].rcAtk[2], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc) ||
//		IntersectRect(&temp, &FRIENDMANAGER->getYubi()->getFriendVector()[0].rcAtk[3], &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rc)) &&
//		FRIENDMANAGER->getYubi()->getIsAtk())
//	{
//		ENEMYMANAGER->getYeopo()->setIsHit(true);
//		damage = (FRIENDMANAGER->getYubi()->getFriendVector()[0].atk - ENEMYMANAGER->getYeopo()->getEnemyVector()[0].def) / 2
//			+ FRIENDMANAGER->getYubi()->getFriendVector()[0].level + 25;
//		eHitCount -= 1;
//
//		if (eHitCount == 0)
//		{
//			ENEMYMANAGER->getYeopo()->hitDamage(damage);
//		}
//	}
//	else
//	{
//		ENEMYMANAGER->getYeopo()->setIsHit(false);
//		eHitCount = 1;
//	}
}

void CollisionManager::enemyToPlayerCollision()
{
//	//여포
//	if ((IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[0], &PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[1], &PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[2], &PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[3], &PLAYERMANAGER->getAgjin()->getPlayerVector()[0].rc)) &&
//		ENEMYMANAGER->getYeopo()->getIsAtk())
//	{
//		PLAYERMANAGER->getAgjin()->setIsHit(true);
//		damage = (ENEMYMANAGER->getYeopo()->getEnemyVector()[0].atk - PLAYERMANAGER->getJojo()->getPlayerVector()[0].def) / 2
//			+ ENEMYMANAGER->getYeopo()->getEnemyVector()[0].level + 25;
//		pHitCount -= 1;
//
//		if (pHitCount == 0)
//		{
//			PLAYERMANAGER->getAgjin()->hitDamage(damage);
//		}
//	}
//	else if ((IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[0], &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[1], &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[2], &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[3], &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc)) &&
//		ENEMYMANAGER->getYeopo()->getIsAtk())
//	{
//		PLAYERMANAGER->getHahudon()->setIsHit(true);
//		damage = (ENEMYMANAGER->getYeopo()->getEnemyVector()[0].atk - PLAYERMANAGER->getHahudon()->getPlayerVector()[0].def) / 2
//			+ ENEMYMANAGER->getYeopo()->getEnemyVector()[0].level + 25;
//		pHitCount -= 1;
//
//		if (pHitCount == 0)
//		{
//			PLAYERMANAGER->getHahudon()->hitDamage(damage);
//		}
//	}
//	else if ((IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[0], &PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[1], &PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[2], &PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[3], &PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rc)) &&
//		ENEMYMANAGER->getYeopo()->getIsAtk())
//	{
//		PLAYERMANAGER->getHahuyeon()->setIsHit(true);
//		damage = (ENEMYMANAGER->getYeopo()->getEnemyVector()[0].atk - PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].def) / 2
//			+ ENEMYMANAGER->getYeopo()->getEnemyVector()[0].level + 25;
//		pHitCount -= 1;
//
//		if (pHitCount == 0)
//		{
//			PLAYERMANAGER->getHahuyeon()->hitDamage(damage);
//		}
//	}
//	else if ((IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[0], &PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[1], &PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[2], &PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[3], &PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rc)) &&
//		ENEMYMANAGER->getYeopo()->getIsAtk())
//	{
//		PLAYERMANAGER->getIjeon()->setIsHit(true);
//		damage = (ENEMYMANAGER->getYeopo()->getEnemyVector()[0].atk - PLAYERMANAGER->getIjeon()->getPlayerVector()[0].def) / 2
//			+ ENEMYMANAGER->getYeopo()->getEnemyVector()[0].level + 25;
//		pHitCount -= 1;
//
//		if (pHitCount == 0)
//		{
//			PLAYERMANAGER->getIjeon()->hitDamage(damage);
//		}
//	}
//	else if ((IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[0], &PLAYERMANAGER->getJohong()->getPlayerVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[1], &PLAYERMANAGER->getJohong()->getPlayerVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[2], &PLAYERMANAGER->getJohong()->getPlayerVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[3], &PLAYERMANAGER->getJohong()->getPlayerVector()[0].rc)) &&
//		ENEMYMANAGER->getYeopo()->getIsAtk())
//	{
//		PLAYERMANAGER->getJohong()->setIsHit(true);
//		damage = (ENEMYMANAGER->getYeopo()->getEnemyVector()[0].atk - PLAYERMANAGER->getJohong()->getPlayerVector()[0].def) / 2
//			+ ENEMYMANAGER->getYeopo()->getEnemyVector()[0].level + 25;
//		pHitCount -= 1;
//
//		if (pHitCount == 0)
//		{
//			PLAYERMANAGER->getJohong()->hitDamage(damage);
//		}
//	}
//	else if ((IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[0], &PLAYERMANAGER->getJoin()->getPlayerVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[1], &PLAYERMANAGER->getJoin()->getPlayerVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[2], &PLAYERMANAGER->getJoin()->getPlayerVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[3], &PLAYERMANAGER->getJoin()->getPlayerVector()[0].rc)) &&
//		ENEMYMANAGER->getYeopo()->getIsAtk())
//	{
//		PLAYERMANAGER->getJoin()->setIsHit(true);
//		damage = (ENEMYMANAGER->getYeopo()->getEnemyVector()[0].atk - PLAYERMANAGER->getJoin()->getPlayerVector()[0].def) / 2
//			+ ENEMYMANAGER->getYeopo()->getEnemyVector()[0].level + 25;
//		pHitCount -= 1;
//
//		if (pHitCount == 0)
//		{
//			PLAYERMANAGER->getJoin()->hitDamage(damage);
//		}
//	}
//	else if ((IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[0], &PLAYERMANAGER->getJojo()->getPlayerVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[1], &PLAYERMANAGER->getJojo()->getPlayerVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[2], &PLAYERMANAGER->getJojo()->getPlayerVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[3], &PLAYERMANAGER->getJojo()->getPlayerVector()[0].rc)) &&
//		ENEMYMANAGER->getYeopo()->getIsAtk())
//	{
//		PLAYERMANAGER->getJojo()->setIsHit(true);
//		damage = (ENEMYMANAGER->getYeopo()->getEnemyVector()[0].atk - PLAYERMANAGER->getJojo()->getPlayerVector()[0].def) / 2
//			+ ENEMYMANAGER->getYeopo()->getEnemyVector()[0].level + 25;
//		pHitCount -= 1;
//
//		if (pHitCount == 0)
//		{
//			PLAYERMANAGER->getJojo()->hitDamage(damage);
//		}
//	}
//	else if ((IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[0], &FRIENDMANAGER->getDogyeom()->getFriendVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[1], &FRIENDMANAGER->getDogyeom()->getFriendVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[2], &FRIENDMANAGER->getDogyeom()->getFriendVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[3], &FRIENDMANAGER->getDogyeom()->getFriendVector()[0].rc)) &&
//		ENEMYMANAGER->getYeopo()->getIsAtk())
//	{
//		FRIENDMANAGER->getDogyeom()->setIsHit(true);
//		damage = (ENEMYMANAGER->getYeopo()->getEnemyVector()[0].atk - FRIENDMANAGER->getDogyeom()->getFriendVector()[0].def) / 2
//			+ ENEMYMANAGER->getYeopo()->getEnemyVector()[0].level + 25;
//		pHitCount -= 1;
//
//		if (pHitCount == 0)
//		{
//			FRIENDMANAGER->getDogyeom()->hitDamage(damage);
//		}
//	}
//	else if ((IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[0], &FRIENDMANAGER->getGwanu()->getFriendVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[1], &FRIENDMANAGER->getGwanu()->getFriendVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[2], &FRIENDMANAGER->getGwanu()->getFriendVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[3], &FRIENDMANAGER->getGwanu()->getFriendVector()[0].rc)) &&
//		ENEMYMANAGER->getYeopo()->getIsAtk())
//	{
//		FRIENDMANAGER->getGwanu()->setIsHit(true);
//		damage = (ENEMYMANAGER->getYeopo()->getEnemyVector()[0].atk - FRIENDMANAGER->getGwanu()->getFriendVector()[0].def) / 2
//			+ ENEMYMANAGER->getYeopo()->getEnemyVector()[0].level + 25;
//		pHitCount -= 1;
//
//		if (pHitCount == 0)
//		{
//			FRIENDMANAGER->getGwanu()->hitDamage(damage);
//		}
//	}
//	else if ((IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[0], &FRIENDMANAGER->getJangbi()->getFriendVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[1], &FRIENDMANAGER->getJangbi()->getFriendVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[2], &FRIENDMANAGER->getJangbi()->getFriendVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[3], &FRIENDMANAGER->getJangbi()->getFriendVector()[0].rc)) &&
//		ENEMYMANAGER->getYeopo()->getIsAtk())
//	{
//		FRIENDMANAGER->getJangbi()->setIsHit(true);
//		damage = (ENEMYMANAGER->getYeopo()->getEnemyVector()[0].atk - FRIENDMANAGER->getJangbi()->getFriendVector()[0].def) / 2
//			+ ENEMYMANAGER->getYeopo()->getEnemyVector()[0].level + 25;
//		pHitCount -= 1;
//
//		if (pHitCount == 0)
//		{
//			FRIENDMANAGER->getJangbi()->hitDamage(damage);
//		}
//	}
//	else if ((IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[0], &FRIENDMANAGER->getWonso()->getFriendVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[1], &FRIENDMANAGER->getWonso()->getFriendVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[2], &FRIENDMANAGER->getWonso()->getFriendVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[3], &FRIENDMANAGER->getWonso()->getFriendVector()[0].rc)) &&
//		ENEMYMANAGER->getYeopo()->getIsAtk())
//	{
//		FRIENDMANAGER->getWonso()->setIsHit(true);
//		damage = (ENEMYMANAGER->getYeopo()->getEnemyVector()[0].atk - FRIENDMANAGER->getWonso()->getFriendVector()[0].def) / 2
//			+ ENEMYMANAGER->getYeopo()->getEnemyVector()[0].level + 25;
//		pHitCount -= 1;
//
//		if (pHitCount == 0)
//		{
//			FRIENDMANAGER->getWonso()->hitDamage(damage);
//		}
//	}
//	else if ((IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[0], &FRIENDMANAGER->getYubi()->getFriendVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[1], &FRIENDMANAGER->getYubi()->getFriendVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[2], &FRIENDMANAGER->getYubi()->getFriendVector()[0].rc) ||
//		IntersectRect(&temp, &ENEMYMANAGER->getYeopo()->getEnemyVector()[0].rcAtk[3], &FRIENDMANAGER->getYubi()->getFriendVector()[0].rc)) &&
//		ENEMYMANAGER->getYeopo()->getIsAtk())
//	{
//		FRIENDMANAGER->getYubi()->setIsHit(true);
//		damage = (ENEMYMANAGER->getYeopo()->getEnemyVector()[0].atk - FRIENDMANAGER->getYubi()->getFriendVector()[0].def) / 2
//			+ ENEMYMANAGER->getYeopo()->getEnemyVector()[0].level + 25;
//		pHitCount -= 1;
//
//		if (pHitCount == 0)
//		{
//			FRIENDMANAGER->getYubi()->hitDamage(damage);
//		}
//	}
//	else
//	{
//		PLAYERMANAGER->getAgjin()->setIsHit(false);
//		PLAYERMANAGER->getHahudon()->setIsHit(false);
//		PLAYERMANAGER->getHahuyeon()->setIsHit(false);
//		PLAYERMANAGER->getIjeon()->setIsHit(false);
//		PLAYERMANAGER->getJohong()->setIsHit(false);
//		PLAYERMANAGER->getJoin()->setIsHit(false);
//		PLAYERMANAGER->getJojo()->setIsHit(false);
//		FRIENDMANAGER->getDogyeom()->setIsHit(false);
//		FRIENDMANAGER->getGwanu()->setIsHit(false);
//		FRIENDMANAGER->getJangbi()->setIsHit(false);
//		FRIENDMANAGER->getWonso()->setIsHit(false);
//		FRIENDMANAGER->getYubi()->setIsHit(false);
//		pHitCount = 1;
//	}
}
