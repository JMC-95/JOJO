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
		auto& enemyInfo = ENEMYMANAGER->getEnemy()[i]->getEnemyInfo();
		auto& enemyRect = ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc;

		ENEMYMANAGER->getEnemy()[i]->setIsHit(false);

		//플레이어 기병 + 궁병
		for (int k = 0; k < 4; k++)
		{
			auto playerAtk = PLAYERMANAGER->getPlayer()[k]->getIsAtk();
			auto& playerInfo = PLAYERMANAGER->getPlayer()[k]->getPlayerInfo();
			auto playerRcAtk = PLAYERMANAGER->getPlayer()[k]->getPlayerInfo().rcAtk;

			bool isInterSect = false;

			for (int j = 0; j < 4; j++)
			{
				if (IntersectRect(&temp, &playerRcAtk[j], &enemyRect))
				{
					isInterSect = true;
					break;
				}
			}

			if (playerAtk && isInterSect && PtInRect(&enemyRect, m_ptMouse))
			{
				ENEMYMANAGER->getEnemy()[i]->setIsHit(true);
				damage = (playerInfo.atk - enemyInfo.def) / 2 + playerInfo.level + 25;

				if (PLAYERMANAGER->getPlayer()[k]->getIsDamage())
				{
					ENEMYMANAGER->getEnemy()[i]->hitDamage(damage);
					PLAYERMANAGER->getPlayer()[k]->setIsDamage(false);
				}
			}
		}

		//플레이어 보병
		for (int k = 4; k < PLAYERMANAGER->getPlayer().size(); k++)
		{
			auto playerAtk = PLAYERMANAGER->getPlayer()[k]->getIsAtk();
			auto& playerInfo = PLAYERMANAGER->getPlayer()[k]->getPlayerInfo();
			auto playerRcAtk = PLAYERMANAGER->getPlayer()[k]->getPlayerInfo().rcAtk;

			bool isInterSect = false;

			for (int j = 0; j < 8; j++)
			{
				if (IntersectRect(&temp, &playerRcAtk[j], &enemyRect))
				{
					isInterSect = true;
					break;
				}
			}

			if (playerAtk && isInterSect && PtInRect(&enemyRect, m_ptMouse))
			{
				damage = (playerInfo.atk - enemyInfo.def) / 2 + playerInfo.level + 25;
				ENEMYMANAGER->getEnemy()[i]->setIsHit(true);

				if (PLAYERMANAGER->getPlayer()[k]->getIsDamage())
				{
					ENEMYMANAGER->getEnemy()[i]->hitDamage(damage);
					PLAYERMANAGER->getPlayer()[k]->setIsDamage(false);
				}
			}
		}

		//우군 기병
		for (int k = 0; k < 5; k++)
		{
			auto friendAtk = FRIENDMANAGER->getFriend()[k]->getIsAtk();
			auto& friendInfo = FRIENDMANAGER->getFriend()[k]->getFriendInfo();
			auto friendRcAtk = FRIENDMANAGER->getFriend()[k]->getFriendInfo().rcAtk;

			bool isInterSect = false;

			for (int j = 0; j < 4; j++)
			{
				if (IntersectRect(&temp, &friendRcAtk[j], &enemyRect))
				{
					isInterSect = true;
					break;
				}
			}

			if (friendAtk && isInterSect && PtInRect(&enemyRect, m_ptMouse))
			{
				damage = (friendInfo.atk - enemyInfo.def) / 2 + friendInfo.level + 25;
				ENEMYMANAGER->getEnemy()[i]->setIsHit(true);

				if (FRIENDMANAGER->getFriend()[k]->getIsDamage())
				{
					ENEMYMANAGER->getEnemy()[i]->hitDamage(damage);
					FRIENDMANAGER->getFriend()[k]->setIsDamage(false);
				}
			}
		}

		//우군 보병
		for (int k = 5; k < FRIENDMANAGER->getFriend().size(); k++)
		{
			auto friendAtk = FRIENDMANAGER->getFriend()[k]->getIsAtk();
			auto& friendInfo = FRIENDMANAGER->getFriend()[k]->getFriendInfo();
			auto friendRcAtk = FRIENDMANAGER->getFriend()[k]->getFriendInfo().rcAtk;

			bool isInterSect = false;

			for (int j = 0; j < 8; j++)
			{
				if (IntersectRect(&temp, &friendRcAtk[j], &enemyRect))
				{
					isInterSect = true;
					break;
				}
			}

			if (friendAtk && isInterSect && PtInRect(&enemyRect, m_ptMouse))
			{
				damage = (friendInfo.atk - enemyInfo.def) / 2 + friendInfo.level + 25;
				ENEMYMANAGER->getEnemy()[i]->setIsHit(true);

				if (FRIENDMANAGER->getFriend()[k]->getIsDamage())
				{
					ENEMYMANAGER->getEnemy()[i]->hitDamage(damage);
					FRIENDMANAGER->getFriend()[k]->setIsDamage(false);
				}
			}
		}
	}
}

void CollisionManager::enemyToPlayerCollision()
{
}
