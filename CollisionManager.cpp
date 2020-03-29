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
	death();
}

void CollisionManager::playerToEnemyCollision()
{
	RECT temp;

	//플레이어 기병 + 궁병
	for (int k = 0; k < 4; k++)
	{
		auto& player = PLAYERMANAGER->getPlayer()[k];
		auto& playerInfo = PLAYERMANAGER->getPlayer()[k]->getPlayerInfo();
		auto playerAtkRc = PLAYERMANAGER->getPlayer()[k]->getPlayerInfo().rcAtk;
		auto playerAtkRng = PLAYERMANAGER->getPlayer()[k]->getIsAtkRng();

		if (playerAtkRng)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				for (int i = 0; i < ENEMYMANAGER->getEnemy().size(); i++)
				{
					auto& enemy = ENEMYMANAGER->getEnemy()[i];
					auto& enemyInfo = ENEMYMANAGER->getEnemy()[i]->getEnemyInfo();
					auto& enemyRect = ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc;

					enemy->setIsHit(false);

					if (IntersectRect(&temp, &playerAtkRc[0], &enemyRect) ||
						IntersectRect(&temp, &playerAtkRc[1], &enemyRect) ||
						IntersectRect(&temp, &playerAtkRc[2], &enemyRect) ||
						IntersectRect(&temp, &playerAtkRc[3], &enemyRect))
					{
						if (PtInRect(&enemyRect, m_ptMouse))
						{
							if (k == 1) SOUNDMANAGER->play("aHit", 1.0f);
							else SOUNDMANAGER->play("hit", 1.0f);

							player->setIsAtk(true);
							enemy->setIsHit(true);
							player->setIsAtkRng(false);

							damage = (playerInfo.atk - enemyInfo.def) / 2 + playerInfo.level + 25;
							enemy->hitDamage(damage);
						}
					}
				}
			}
		}
	}

	//플레이어 보병
	for (int k = 4; k < PLAYERMANAGER->getPlayer().size(); k++)
	{
		auto& player = PLAYERMANAGER->getPlayer()[k];
		auto& playerInfo = PLAYERMANAGER->getPlayer()[k]->getPlayerInfo();
		auto playerAtkRc = PLAYERMANAGER->getPlayer()[k]->getPlayerInfo().rcAtk;
		auto playerAtkRng = PLAYERMANAGER->getPlayer()[k]->getIsAtkRng();

		if (playerAtkRng)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				for (int i = 0; i < ENEMYMANAGER->getEnemy().size(); i++)
				{
					auto& enemy = ENEMYMANAGER->getEnemy()[i];
					auto& enemyInfo = ENEMYMANAGER->getEnemy()[i]->getEnemyInfo();
					auto& enemyRect = ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc;

					enemy->setIsHit(false);

					if (IntersectRect(&temp, &playerAtkRc[0], &enemyRect) ||
						IntersectRect(&temp, &playerAtkRc[1], &enemyRect) ||
						IntersectRect(&temp, &playerAtkRc[2], &enemyRect) ||
						IntersectRect(&temp, &playerAtkRc[3], &enemyRect) ||
						IntersectRect(&temp, &playerAtkRc[4], &enemyRect) ||
						IntersectRect(&temp, &playerAtkRc[5], &enemyRect) ||
						IntersectRect(&temp, &playerAtkRc[6], &enemyRect) ||
						IntersectRect(&temp, &playerAtkRc[7], &enemyRect))
					{
						if (PtInRect(&enemyRect, m_ptMouse))
						{
							SOUNDMANAGER->play("hit", 1.0f);

							player->setIsAtk(true);
							enemy->setIsHit(true);
							player->setIsAtkRng(false);

							damage = (playerInfo.atk - enemyInfo.def) / 2 + playerInfo.level + 25;
							enemy->hitDamage(damage);
						}
					}
				}
			}
		}
	}

	//우군 기병
	for (int k = 0; k < 5; k++)
	{
		auto& friendly = FRIENDMANAGER->getFriend()[k];
		auto& friendInfo = FRIENDMANAGER->getFriend()[k]->getFriendInfo();
		auto friendAtkRc = FRIENDMANAGER->getFriend()[k]->getFriendInfo().rcAtk;
		auto friendAtkRng = FRIENDMANAGER->getFriend()[k]->getIsAtkRng();

		if (friendAtkRng)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				for (int i = 0; i < ENEMYMANAGER->getEnemy().size(); i++)
				{
					auto& enemy = ENEMYMANAGER->getEnemy()[i];
					auto& enemyInfo = ENEMYMANAGER->getEnemy()[i]->getEnemyInfo();
					auto& enemyRect = ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc;

					enemy->setIsHit(false);

					if (IntersectRect(&temp, &friendAtkRc[0], &enemyRect) ||
						IntersectRect(&temp, &friendAtkRc[1], &enemyRect) ||
						IntersectRect(&temp, &friendAtkRc[2], &enemyRect) ||
						IntersectRect(&temp, &friendAtkRc[3], &enemyRect))
					{
						if (PtInRect(&enemyRect, m_ptMouse))
						{
							SOUNDMANAGER->play("hit", 1.0f);

							friendly->setIsAtk(true);
							enemy->setIsHit(true);
							friendly->setIsAtkRng(false);

							damage = (friendInfo.atk - enemyInfo.def) / 2 + friendInfo.level + 25;
							enemy->hitDamage(damage);
						}
					}
				}
			}
		}
	}

	//우군 보병
	for (int k = 5; k < FRIENDMANAGER->getFriend().size(); k++)
	{
		auto& friendly = FRIENDMANAGER->getFriend()[k];
		auto& friendInfo = FRIENDMANAGER->getFriend()[k]->getFriendInfo();
		auto friendAtkRc = FRIENDMANAGER->getFriend()[k]->getFriendInfo().rcAtk;
		auto friendAtkRng = FRIENDMANAGER->getFriend()[k]->getIsAtkRng();

		if (friendAtkRng)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				for (int i = 0; i < ENEMYMANAGER->getEnemy().size(); i++)
				{
					auto& enemy = ENEMYMANAGER->getEnemy()[i];
					auto& enemyInfo = ENEMYMANAGER->getEnemy()[i]->getEnemyInfo();
					auto& enemyRect = ENEMYMANAGER->getEnemy()[i]->getEnemyInfo().rc;

					enemy->setIsHit(false);

					if (IntersectRect(&temp, &friendAtkRc[0], &enemyRect) ||
						IntersectRect(&temp, &friendAtkRc[1], &enemyRect) ||
						IntersectRect(&temp, &friendAtkRc[2], &enemyRect) ||
						IntersectRect(&temp, &friendAtkRc[3], &enemyRect) ||
						IntersectRect(&temp, &friendAtkRc[4], &enemyRect) ||
						IntersectRect(&temp, &friendAtkRc[5], &enemyRect) ||
						IntersectRect(&temp, &friendAtkRc[6], &enemyRect) ||
						IntersectRect(&temp, &friendAtkRc[7], &enemyRect))
					{
						if (PtInRect(&enemyRect, m_ptMouse))
						{
							SOUNDMANAGER->play("hit", 1.0f);

							friendly->setIsAtk(true);
							enemy->setIsHit(true);
							friendly->setIsAtkRng(false);

							damage = (friendInfo.atk - enemyInfo.def) / 2 + friendInfo.level + 25;
							enemy->hitDamage(damage);
						}
					}
				}
			}
		}
	}
}

void CollisionManager::enemyToPlayerCollision()
{
}

void CollisionManager::death()
{
	//플레이어 죽음
	auto& playerVector = PLAYERMANAGER->getPlayer();

	for (auto player = playerVector.begin(); player != playerVector.end(); )
	{
		int hp = (*player)->getCurrentHp();

		if (hp <= 0)
		{
			SOUNDMANAGER->play("death", 1.0f);
			player = playerVector.erase(player);
		}
		else
		{
			player++;
		}
	}

	//우군 죽음
	auto& friendVector = PLAYERMANAGER->getPlayer();

	for (auto friendly = friendVector.begin(); friendly != friendVector.end(); )
	{
		int hp = (*friendly)->getCurrentHp();

		if (hp <= 0)
		{
			SOUNDMANAGER->play("death", 1.0f);
			friendly = friendVector.erase(friendly);
		}
		else
		{
			friendly++;
		}
	}

	//적군 죽음
	auto& enemyVector = ENEMYMANAGER->getEnemy();

	for (auto enemy = enemyVector.begin(); enemy != enemyVector.end(); )
	{
		int hp = (*enemy)->getCurrentHp();

		if (hp <= 0)
		{
			SOUNDMANAGER->play("death", 1.0f);
			enemy = enemyVector.erase(enemy);
		}
		else
		{
			enemy++;
		}
	}
}
