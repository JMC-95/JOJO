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
	for (int i = 0; i < ENEMYMANAGER->getEnemy().size(); i++)
	{
		//플레이어
		if ((IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[0], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[1], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[2], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[3], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[4], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[5], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[6], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().rcAtk[7], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc)) &&
			PLAYERMANAGER->getPlayer()[0]->getIsAtk())
		{
			ENEMYMANAGER->getEnemy()[i]->setIsHit(true);
			damage = (PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().atk - ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().def) / 2
				+ PLAYERMANAGER->getPlayer()[0]->getPlayerInfo().level + 25;
			eHitCount -= 1;

			if (eHitCount == 0)
			{
				ENEMYMANAGER->getEnemy()[i]->hitDamage(damage);
			}
		}
		else if ((IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[0], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[1], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[2], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().rcAtk[3], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc)) &&
			PLAYERMANAGER->getPlayer()[1]->getIsAtk())
		{
			ENEMYMANAGER->getEnemy()[i]->setIsHit(true);
			damage = (PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().atk - ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().def) / 2
				+ PLAYERMANAGER->getPlayer()[1]->getPlayerInfo().level + 25;
			eHitCount -= 1;

			if (eHitCount == 0)
			{
				ENEMYMANAGER->getEnemy()[i]->hitDamage(damage);
			}
		}
		else if ((IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[0], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[1], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[2], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().rcAtk[3], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc)) &&
			PLAYERMANAGER->getPlayer()[2]->getIsAtk())
		{
			ENEMYMANAGER->getEnemy()[i]->setIsHit(true);
			damage = (PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().atk - ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().def) / 2
				+ PLAYERMANAGER->getPlayer()[2]->getPlayerInfo().level + 25;
			eHitCount -= 1;

			if (eHitCount == 0)
			{
				ENEMYMANAGER->getEnemy()[i]->hitDamage(damage);
			}
		}
		else if ((IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[0], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[1], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[2], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[3], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[4], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[5], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[6], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().rcAtk[7], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc)) &&
			PLAYERMANAGER->getPlayer()[3]->getIsAtk())
		{
			ENEMYMANAGER->getEnemy()[i]->setIsHit(true);
			damage = (PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().atk - ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().def) / 2
				+ PLAYERMANAGER->getPlayer()[3]->getPlayerInfo().level + 25;
			eHitCount -= 1;

			if (eHitCount == 0)
			{
				ENEMYMANAGER->getEnemy()[i]->hitDamage(damage);
			}
		}
		else if ((IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[0], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[1], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[2], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[3], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[4], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[5], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[6], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().rcAtk[7], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc)) &&
			PLAYERMANAGER->getPlayer()[4]->getIsAtk())
		{
			ENEMYMANAGER->getEnemy()[i]->setIsHit(true);
			damage = (PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().atk - ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().def) / 2
				+ PLAYERMANAGER->getPlayer()[4]->getPlayerInfo().level + 25;
			eHitCount -= 1;

			if (eHitCount == 0)
			{
				ENEMYMANAGER->getEnemy()[i]->hitDamage(damage);
			}
		}
		else if ((IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[0], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[1], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[2], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().rcAtk[3], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc)) &&
			PLAYERMANAGER->getPlayer()[5]->getIsAtk())
		{
			ENEMYMANAGER->getEnemy()[i]->setIsHit(true);
			damage = (PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().atk - ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().def) / 2
				+ PLAYERMANAGER->getPlayer()[5]->getPlayerInfo().level + 25;
			eHitCount -= 1;

			if (eHitCount == 0)
			{
				ENEMYMANAGER->getEnemy()[i]->hitDamage(damage);
			}
		}
		else if ((IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rcAtk[0], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rcAtk[1], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rcAtk[2], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().rcAtk[3], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc)) &&
			PLAYERMANAGER->getPlayer()[6]->getIsAtk())
		{
			ENEMYMANAGER->getEnemy()[i]->setIsHit(true);
			damage = (PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().atk - ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().def) / 2
				+ PLAYERMANAGER->getPlayer()[6]->getPlayerInfo().level + 25;
			eHitCount -= 1;

			if (eHitCount == 0)
			{
				ENEMYMANAGER->getEnemy()[i]->hitDamage(damage);
			}
		}
		//우군
		else if ((IntersectRect(&temp, &FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[0], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[1], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[2], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[0]->getFriendInfo().rcAtk[3], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc)) &&
			FRIENDMANAGER->getFriend()[0]->getIsAtk())
		{
			ENEMYMANAGER->getEnemy()[i]->setIsHit(true);
			damage = (FRIENDMANAGER->getFriend()[0]->getFriendInfo().atk - ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().def) / 2
				+ FRIENDMANAGER->getFriend()[0]->getFriendInfo().level + 25;
			eHitCount -= 1;

			if (eHitCount == 0)
			{
				ENEMYMANAGER->getEnemy()[i]->hitDamage(damage);
			}
		}
		else if ((IntersectRect(&temp, &FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[0], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[1], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[2], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[1]->getFriendInfo().rcAtk[3], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc)) &&
			FRIENDMANAGER->getFriend()[1]->getIsAtk())
		{
			ENEMYMANAGER->getEnemy()[i]->setIsHit(true);
			damage = (FRIENDMANAGER->getFriend()[1]->getFriendInfo().atk - ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().def) / 2
				+ FRIENDMANAGER->getFriend()[1]->getFriendInfo().level + 25;
			eHitCount -= 1;

			if (eHitCount == 0)
			{
				ENEMYMANAGER->getEnemy()[i]->hitDamage(damage);
			}
		}
		else if ((IntersectRect(&temp, &FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[0], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[1], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[2], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[2]->getFriendInfo().rcAtk[3], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc)) &&
			FRIENDMANAGER->getFriend()[2]->getIsAtk())
		{
			ENEMYMANAGER->getEnemy()[i]->setIsHit(true);
			damage = (FRIENDMANAGER->getFriend()[2]->getFriendInfo().atk - ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().def) / 2
				+ FRIENDMANAGER->getFriend()[2]->getFriendInfo().level + 25;
			eHitCount -= 1;

			if (eHitCount == 0)
			{
				ENEMYMANAGER->getEnemy()[i]->hitDamage(damage);
			}
		}
		else if ((IntersectRect(&temp, &FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[0], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[1], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[2], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[3]->getFriendInfo().rcAtk[3], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc)) &&
			FRIENDMANAGER->getFriend()[3]->getIsAtk())
		{
			ENEMYMANAGER->getEnemy()[i]->setIsHit(true);
			damage = (FRIENDMANAGER->getFriend()[3]->getFriendInfo().atk - ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().def) / 2
				+ FRIENDMANAGER->getFriend()[3]->getFriendInfo().level + 25;
			eHitCount -= 1;

			if (eHitCount == 0)
			{
				ENEMYMANAGER->getEnemy()[i]->hitDamage(damage);
			}
		}
		else if ((IntersectRect(&temp, &FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[0], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[1], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[2], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[4]->getFriendInfo().rcAtk[3], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc)) &&
			FRIENDMANAGER->getFriend()[4]->getIsAtk())
		{
			ENEMYMANAGER->getEnemy()[i]->setIsHit(true);
			damage = (FRIENDMANAGER->getFriend()[4]->getFriendInfo().atk - ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().def) / 2
				+ FRIENDMANAGER->getFriend()[4]->getFriendInfo().level + 25;
			eHitCount -= 1;

			if (eHitCount == 0)
			{
				ENEMYMANAGER->getEnemy()[i]->hitDamage(damage);
			}
		}
		else if ((IntersectRect(&temp, &FRIENDMANAGER->getFriend()[5]->getFriendInfo().rcAtk[0], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[5]->getFriendInfo().rcAtk[1], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[5]->getFriendInfo().rcAtk[2], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[5]->getFriendInfo().rcAtk[3], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[5]->getFriendInfo().rcAtk[4], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[5]->getFriendInfo().rcAtk[5], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[5]->getFriendInfo().rcAtk[6], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[5]->getFriendInfo().rcAtk[7], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc)) &&
			FRIENDMANAGER->getFriend()[5]->getIsAtk())
		{
			ENEMYMANAGER->getEnemy()[i]->setIsHit(true);
			damage = (FRIENDMANAGER->getFriend()[5]->getFriendInfo().atk - ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().def) / 2
				+ FRIENDMANAGER->getFriend()[5]->getFriendInfo().level + 25;
			eHitCount -= 1;

			if (eHitCount == 0)
			{
				ENEMYMANAGER->getEnemy()[i]->hitDamage(damage);
			}
		}
		else if ((IntersectRect(&temp, &FRIENDMANAGER->getFriend()[6]->getFriendInfo().rcAtk[0], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[6]->getFriendInfo().rcAtk[1], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[6]->getFriendInfo().rcAtk[2], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[6]->getFriendInfo().rcAtk[3], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[6]->getFriendInfo().rcAtk[4], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[6]->getFriendInfo().rcAtk[5], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[6]->getFriendInfo().rcAtk[6], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[6]->getFriendInfo().rcAtk[7], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc)) &&
			FRIENDMANAGER->getFriend()[6]->getIsAtk())
		{
			ENEMYMANAGER->getEnemy()[i]->setIsHit(true);
			damage = (FRIENDMANAGER->getFriend()[6]->getFriendInfo().atk - ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().def) / 2
				+ FRIENDMANAGER->getFriend()[6]->getFriendInfo().level + 25;
			eHitCount -= 1;

			if (eHitCount == 0)
			{
				ENEMYMANAGER->getEnemy()[i]->hitDamage(damage);
			}
		}
		else if ((IntersectRect(&temp, &FRIENDMANAGER->getFriend()[7]->getFriendInfo().rcAtk[0], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[7]->getFriendInfo().rcAtk[1], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[7]->getFriendInfo().rcAtk[2], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[7]->getFriendInfo().rcAtk[3], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[7]->getFriendInfo().rcAtk[4], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[7]->getFriendInfo().rcAtk[5], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[7]->getFriendInfo().rcAtk[6], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[7]->getFriendInfo().rcAtk[7], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc)) &&
			FRIENDMANAGER->getFriend()[7]->getIsAtk())
		{
			ENEMYMANAGER->getEnemy()[i]->setIsHit(true);
			damage = (FRIENDMANAGER->getFriend()[7]->getFriendInfo().atk - ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().def) / 2
				+ FRIENDMANAGER->getFriend()[7]->getFriendInfo().level + 25;
			eHitCount -= 1;

			if (eHitCount == 0)
			{
				ENEMYMANAGER->getEnemy()[i]->hitDamage(damage);
			}
		}
		else if ((IntersectRect(&temp, &FRIENDMANAGER->getFriend()[8]->getFriendInfo().rcAtk[0], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[8]->getFriendInfo().rcAtk[1], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[8]->getFriendInfo().rcAtk[2], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[8]->getFriendInfo().rcAtk[3], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[8]->getFriendInfo().rcAtk[4], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[8]->getFriendInfo().rcAtk[5], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[8]->getFriendInfo().rcAtk[6], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc) ||
			IntersectRect(&temp, &FRIENDMANAGER->getFriend()[8]->getFriendInfo().rcAtk[7], &ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc)) &&
			FRIENDMANAGER->getFriend()[4]->getIsAtk())
		{
			ENEMYMANAGER->getEnemy()[i]->setIsHit(true);
			damage = (FRIENDMANAGER->getFriend()[4]->getFriendInfo().atk - ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().def) / 2
				+ FRIENDMANAGER->getFriend()[4]->getFriendInfo().level + 25;
			eHitCount -= 1;

			if (eHitCount == 0)
			{
				ENEMYMANAGER->getEnemy()[i]->hitDamage(damage);
			}
		}
		else
		{
			ENEMYMANAGER->getEnemy()[i]->setIsHit(false);
			eHitCount = 1;
		}
	}
}

void CollisionManager::enemyToPlayerCollision()
{
	//여포
	for (int i = 0; i < PLAYERMANAGER->getPlayer().size(); i++)
	{
		if ((IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[0], &PLAYERMANAGER->getPlayer()[i]->getPlayerInfo().rc) ||
			IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[1], &PLAYERMANAGER->getPlayer()[i]->getPlayerInfo().rc) ||
			IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[2], &PLAYERMANAGER->getPlayer()[i]->getPlayerInfo().rc) ||
			IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[3], &PLAYERMANAGER->getPlayer()[i]->getPlayerInfo().rc)) &&
			ENEMYMANAGER->getEnemy()[4]->getIsAtk())
		{
			PLAYERMANAGER->getPlayer()[i]->setIsHit(true);
			damage = (ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().atk - PLAYERMANAGER->getPlayer()[i]->getPlayerInfo().def) / 2
				+ ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().level + 25;
			pHitCount -= 1;

			if (pHitCount == 0)
			{
				PLAYERMANAGER->getPlayer()[i]->hitDamage(damage);
			}
		}
		else
		{
			PLAYERMANAGER->getPlayer()[i]->setIsHit(false);
			pHitCount = 1;
		}
	}
	/*else if ((IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[0], &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[1], &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[2], &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[3], &PLAYERMANAGER->getHahudon()->getPlayerVector()[0].rc)) &&
		ENEMYMANAGER->getEnemy()[4]->getIsAtk())
	{
		PLAYERMANAGER->getHahudon()->setIsHit(true);
		damage = (ENEMYMANAGER->getEnemy()[4]->getEnemyInfo()atk - PLAYERMANAGER->getHahudon()->getPlayerVector()[0].def) / 2
			+ ENEMYMANAGER->getEnemy()[4]->getEnemyInfo()level + 25;
		pHitCount -= 1;

		if (pHitCount == 0)
		{
			PLAYERMANAGER->getHahudon()->hitDamage(damage);
		}
	}
	else if ((IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[0], &PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[1], &PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[2], &PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[3], &PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].rc)) &&
		ENEMYMANAGER->getEnemy()[4]->getIsAtk())
	{
		PLAYERMANAGER->getHahuyeon()->setIsHit(true);
		damage = (ENEMYMANAGER->getEnemy()[4]->getEnemyInfo()atk - PLAYERMANAGER->getHahuyeon()->getPlayerVector()[0].def) / 2
			+ ENEMYMANAGER->getEnemy()[4]->getEnemyInfo()level + 25;
		pHitCount -= 1;

		if (pHitCount == 0)
		{
			PLAYERMANAGER->getHahuyeon()->hitDamage(damage);
		}
	}
	else if ((IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[0], &PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[1], &PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[2], &PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[3], &PLAYERMANAGER->getIjeon()->getPlayerVector()[0].rc)) &&
		ENEMYMANAGER->getEnemy()[4]->getIsAtk())
	{
		PLAYERMANAGER->getIjeon()->setIsHit(true);
		damage = (ENEMYMANAGER->getEnemy()[4]->getEnemyInfo()atk - PLAYERMANAGER->getIjeon()->getPlayerVector()[0].def) / 2
			+ ENEMYMANAGER->getEnemy()[4]->getEnemyInfo()level + 25;
		pHitCount -= 1;

		if (pHitCount == 0)
		{
			PLAYERMANAGER->getIjeon()->hitDamage(damage);
		}
	}
	else if ((IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[0], &PLAYERMANAGER->getJohong()->getPlayerVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[1], &PLAYERMANAGER->getJohong()->getPlayerVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[2], &PLAYERMANAGER->getJohong()->getPlayerVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[3], &PLAYERMANAGER->getJohong()->getPlayerVector()[0].rc)) &&
		ENEMYMANAGER->getEnemy()[4]->getIsAtk())
	{
		PLAYERMANAGER->getJohong()->setIsHit(true);
		damage = (ENEMYMANAGER->getEnemy()[4]->getEnemyInfo()atk - PLAYERMANAGER->getJohong()->getPlayerVector()[0].def) / 2
			+ ENEMYMANAGER->getEnemy()[4]->getEnemyInfo()level + 25;
		pHitCount -= 1;

		if (pHitCount == 0)
		{
			PLAYERMANAGER->getJohong()->hitDamage(damage);
		}
	}
	else if ((IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[0], &PLAYERMANAGER->getJoin()->getPlayerVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[1], &PLAYERMANAGER->getJoin()->getPlayerVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[2], &PLAYERMANAGER->getJoin()->getPlayerVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[3], &PLAYERMANAGER->getJoin()->getPlayerVector()[0].rc)) &&
		ENEMYMANAGER->getEnemy()[4]->getIsAtk())
	{
		PLAYERMANAGER->getJoin()->setIsHit(true);
		damage = (ENEMYMANAGER->getEnemy()[4]->getEnemyInfo()atk - PLAYERMANAGER->getJoin()->getPlayerVector()[0].def) / 2
			+ ENEMYMANAGER->getEnemy()[4]->getEnemyInfo()level + 25;
		pHitCount -= 1;

		if (pHitCount == 0)
		{
			PLAYERMANAGER->getJoin()->hitDamage(damage);
		}
	}
	else if ((IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[0], &PLAYERMANAGER->getJojo()->getPlayerVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[1], &PLAYERMANAGER->getJojo()->getPlayerVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[2], &PLAYERMANAGER->getJojo()->getPlayerVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[3], &PLAYERMANAGER->getJojo()->getPlayerVector()[0].rc)) &&
		ENEMYMANAGER->getEnemy()[4]->getIsAtk())
	{
		PLAYERMANAGER->getJojo()->setIsHit(true);
		damage = (ENEMYMANAGER->getEnemy()[4]->getEnemyInfo()atk - PLAYERMANAGER->getJojo()->getPlayerVector()[0].def) / 2
			+ ENEMYMANAGER->getEnemy()[4]->getEnemyInfo()level + 25;
		pHitCount -= 1;

		if (pHitCount == 0)
		{
			PLAYERMANAGER->getJojo()->hitDamage(damage);
		}
	}
	else if ((IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[0], &FRIENDMANAGER->getDogyeom()->getFriendVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[1], &FRIENDMANAGER->getDogyeom()->getFriendVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[2], &FRIENDMANAGER->getDogyeom()->getFriendVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[3], &FRIENDMANAGER->getDogyeom()->getFriendVector()[0].rc)) &&
		ENEMYMANAGER->getEnemy()[4]->getIsAtk())
	{
		FRIENDMANAGER->getDogyeom()->setIsHit(true);
		damage = (ENEMYMANAGER->getEnemy()[4]->getEnemyInfo()atk - FRIENDMANAGER->getDogyeom()->getFriendVector()[0].def) / 2
			+ ENEMYMANAGER->getEnemy()[4]->getEnemyInfo()level + 25;
		pHitCount -= 1;

		if (pHitCount == 0)
		{
			FRIENDMANAGER->getDogyeom()->hitDamage(damage);
		}
	}
	else if ((IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[0], &FRIENDMANAGER->getGwanu()->getFriendVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[1], &FRIENDMANAGER->getGwanu()->getFriendVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[2], &FRIENDMANAGER->getGwanu()->getFriendVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[3], &FRIENDMANAGER->getGwanu()->getFriendVector()[0].rc)) &&
		ENEMYMANAGER->getEnemy()[4]->getIsAtk())
	{
		FRIENDMANAGER->getGwanu()->setIsHit(true);
		damage = (ENEMYMANAGER->getEnemy()[4]->getEnemyInfo()atk - FRIENDMANAGER->getGwanu()->getFriendVector()[0].def) / 2
			+ ENEMYMANAGER->getEnemy()[4]->getEnemyInfo()level + 25;
		pHitCount -= 1;

		if (pHitCount == 0)
		{
			FRIENDMANAGER->getGwanu()->hitDamage(damage);
		}
	}
	else if ((IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[0], &FRIENDMANAGER->getJangbi()->getFriendVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[1], &FRIENDMANAGER->getJangbi()->getFriendVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[2], &FRIENDMANAGER->getJangbi()->getFriendVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[3], &FRIENDMANAGER->getJangbi()->getFriendVector()[0].rc)) &&
		ENEMYMANAGER->getEnemy()[4]->getIsAtk())
	{
		FRIENDMANAGER->getJangbi()->setIsHit(true);
		damage = (ENEMYMANAGER->getEnemy()[4]->getEnemyInfo()atk - FRIENDMANAGER->getJangbi()->getFriendVector()[0].def) / 2
			+ ENEMYMANAGER->getEnemy()[4]->getEnemyInfo()level + 25;
		pHitCount -= 1;

		if (pHitCount == 0)
		{
			FRIENDMANAGER->getJangbi()->hitDamage(damage);
		}
	}
	else if ((IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[0], &FRIENDMANAGER->getWonso()->getFriendVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[1], &FRIENDMANAGER->getWonso()->getFriendVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[2], &FRIENDMANAGER->getWonso()->getFriendVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[3], &FRIENDMANAGER->getWonso()->getFriendVector()[0].rc)) &&
		ENEMYMANAGER->getEnemy()[4]->getIsAtk())
	{
		FRIENDMANAGER->getWonso()->setIsHit(true);
		damage = (ENEMYMANAGER->getEnemy()[4]->getEnemyInfo()atk - FRIENDMANAGER->getWonso()->getFriendVector()[0].def) / 2
			+ ENEMYMANAGER->getEnemy()[4]->getEnemyInfo()level + 25;
		pHitCount -= 1;

		if (pHitCount == 0)
		{
			FRIENDMANAGER->getWonso()->hitDamage(damage);
		}
	}
	else if ((IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[0], &FRIENDMANAGER->getYubi()->getFriendVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[1], &FRIENDMANAGER->getYubi()->getFriendVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[2], &FRIENDMANAGER->getYubi()->getFriendVector()[0].rc) ||
		IntersectRect(&temp, &ENEMYMANAGER->getEnemy()[4]->getEnemyInfo().rcAtk[3], &FRIENDMANAGER->getYubi()->getFriendVector()[0].rc)) &&
		ENEMYMANAGER->getEnemy()[4]->getIsAtk())
	{
		FRIENDMANAGER->getYubi()->setIsHit(true);
		damage = (ENEMYMANAGER->getEnemy()[4]->getEnemyInfo()atk - FRIENDMANAGER->getYubi()->getFriendVector()[0].def) / 2
			+ ENEMYMANAGER->getEnemy()[4]->getEnemyInfo()level + 25;
		pHitCount -= 1;

		if (pHitCount == 0)
		{
			FRIENDMANAGER->getYubi()->hitDamage(damage);
		}
	}
	else
	{
		PLAYERMANAGER->getAgjin()->setIsHit(false);
		PLAYERMANAGER->getHahudon()->setIsHit(false);
		PLAYERMANAGER->getHahuyeon()->setIsHit(false);
		PLAYERMANAGER->getIjeon()->setIsHit(false);
		PLAYERMANAGER->getJohong()->setIsHit(false);
		PLAYERMANAGER->getJoin()->setIsHit(false);
		PLAYERMANAGER->getJojo()->setIsHit(false);
		FRIENDMANAGER->getDogyeom()->setIsHit(false);
		FRIENDMANAGER->getGwanu()->setIsHit(false);
		FRIENDMANAGER->getJangbi()->setIsHit(false);
		FRIENDMANAGER->getWonso()->setIsHit(false);
		FRIENDMANAGER->getYubi()->setIsHit(false);
		pHitCount = 1;
	}*/
}
