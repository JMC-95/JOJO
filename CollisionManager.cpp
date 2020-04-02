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

							damage = ((playerInfo.atk + playerInfo.addAtk) - (enemyInfo.def + enemyInfo.addDef)) / 2 + playerInfo.level + 25;
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

							damage = ((playerInfo.atk + playerInfo.addAtk) - (enemyInfo.def + enemyInfo.addDef)) / 2 + playerInfo.level + 25;
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

							damage = ((friendInfo.atk + friendInfo.addAtk) - (enemyInfo.def + enemyInfo.addDef)) / 2 + friendInfo.level + 25;
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

							damage = ((friendInfo.atk + friendInfo.addAtk) - (enemyInfo.def + enemyInfo.addDef)) / 2 + friendInfo.level + 25;
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
	//적군 기병
	for (int k = 0; k < 9; k++)
	{
		auto& enemy = ENEMYMANAGER->getEnemy()[k];
		auto& enemyInfo = ENEMYMANAGER->getEnemy()[k]->getEnemyInfo();
		auto enemyAtkRc = ENEMYMANAGER->getEnemy()[k]->getEnemyInfo().rcAtk;
		auto enemyAtkRng = ENEMYMANAGER->getEnemy()[k]->getIsAtkRng();

		if (enemyAtkRng)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				//플레이어
				for (int i = 0; i < PLAYERMANAGER->getPlayer().size(); i++)
				{
					auto& player = PLAYERMANAGER->getPlayer()[i];
					auto& playerInfo = PLAYERMANAGER->getPlayer()[i]->getPlayerInfo();
					auto& playerRect = PLAYERMANAGER->getPlayer()[i]->getPlayerInfo().rc;

					enemy->setIsHit(false);

					if (IntersectRect(&temp, &enemyAtkRc[0], &playerRect) ||
						IntersectRect(&temp, &enemyAtkRc[1], &playerRect) ||
						IntersectRect(&temp, &enemyAtkRc[2], &playerRect) ||
						IntersectRect(&temp, &enemyAtkRc[3], &playerRect))
					{
						if (PtInRect(&playerRect, m_ptMouse))
						{
							SOUNDMANAGER->play("hit", 1.0f);

							enemy->setIsAtk(true);
							player->setIsHit(true);
							enemy->setIsAtkRng(false);

							damage = ((enemyInfo.atk + enemyInfo.addAtk) - (playerInfo.def + playerInfo.addDef)) / 2 + enemyInfo.level + 25;
							player->hitDamage(damage);
						}
					}
				}

				//우군
				for (int i = 0; i < FRIENDMANAGER->getFriend().size(); i++)
				{
					auto& friendly = FRIENDMANAGER->getFriend()[i];
					auto& friendInfo = FRIENDMANAGER->getFriend()[i]->getFriendInfo();
					auto& friendRect = FRIENDMANAGER->getFriend()[i]->getFriendInfo().rc;

					enemy->setIsHit(false);

					if (IntersectRect(&temp, &enemyAtkRc[0], &friendRect) ||
						IntersectRect(&temp, &enemyAtkRc[1], &friendRect) ||
						IntersectRect(&temp, &enemyAtkRc[2], &friendRect) ||
						IntersectRect(&temp, &enemyAtkRc[3], &friendRect))
					{
						if (PtInRect(&friendRect, m_ptMouse))
						{
							SOUNDMANAGER->play("hit", 1.0f);

							enemy->setIsAtk(true);
							friendly->setIsHit(true);
							enemy->setIsAtkRng(false);

							damage = ((enemyInfo.atk + enemyInfo.addAtk) - (friendInfo.def + friendInfo.addDef)) / 2 + enemyInfo.level + 25;
							friendly->hitDamage(damage);
						}
					}
				}
			}
		}
	}

	//적군 보병 + 궁병
	for (int k = 9; k < ENEMYMANAGER->getEnemy().size(); k++)
	{
		auto& enemy = ENEMYMANAGER->getEnemy()[k];
		auto& enmyInfo = ENEMYMANAGER->getEnemy()[k]->getEnemyInfo();
		auto enemyAtkRc = ENEMYMANAGER->getEnemy()[k]->getEnemyInfo().rcAtk;
		auto enemyAtkRng = ENEMYMANAGER->getEnemy()[k]->getIsAtkRng();

		if (enemyAtkRng)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				//플레이어
				for (int i = 0; i < PLAYERMANAGER->getPlayer().size(); i++)
				{
					auto& player = PLAYERMANAGER->getPlayer()[i];
					auto& playerInfo = PLAYERMANAGER->getPlayer()[i]->getPlayerInfo();
					auto& playerRect = PLAYERMANAGER->getPlayer()[i]->getPlayerInfo().rc;

					player->setIsHit(false);

					if (IntersectRect(&temp, &enemyAtkRc[0], &playerRect) ||
						IntersectRect(&temp, &enemyAtkRc[1], &playerRect) ||
						IntersectRect(&temp, &enemyAtkRc[2], &playerRect) ||
						IntersectRect(&temp, &enemyAtkRc[3], &playerRect) ||
						IntersectRect(&temp, &enemyAtkRc[4], &playerRect) ||
						IntersectRect(&temp, &enemyAtkRc[5], &playerRect) ||
						IntersectRect(&temp, &enemyAtkRc[6], &playerRect) ||
						IntersectRect(&temp, &enemyAtkRc[7], &playerRect))
					{
						if (PtInRect(&playerRect, m_ptMouse))
						{
							SOUNDMANAGER->play("hit", 1.0f);

							enemy->setIsAtk(true);
							player->setIsHit(true);
							enemy->setIsAtkRng(false);

							damage = ((enmyInfo.atk + enmyInfo.addAtk) - (playerInfo.def + playerInfo.addDef)) / 2 + enmyInfo.level + 25;
							player->hitDamage(damage);
						}
					}
				}

				//우군
				for (int i = 0; i < FRIENDMANAGER->getFriend().size(); i++)
				{
					auto& friendly = FRIENDMANAGER->getFriend()[i];
					auto& friendInfo = FRIENDMANAGER->getFriend()[i]->getFriendInfo();
					auto& friendRect = FRIENDMANAGER->getFriend()[i]->getFriendInfo().rc;

					friendly->setIsHit(false);

					if (IntersectRect(&temp, &enemyAtkRc[0], &friendRect) ||
						IntersectRect(&temp, &enemyAtkRc[1], &friendRect) ||
						IntersectRect(&temp, &enemyAtkRc[2], &friendRect) ||
						IntersectRect(&temp, &enemyAtkRc[3], &friendRect) ||
						IntersectRect(&temp, &enemyAtkRc[4], &friendRect) ||
						IntersectRect(&temp, &enemyAtkRc[5], &friendRect) ||
						IntersectRect(&temp, &enemyAtkRc[6], &friendRect) ||
						IntersectRect(&temp, &enemyAtkRc[7], &friendRect))
					{
						if (PtInRect(&friendRect, m_ptMouse))
						{
							SOUNDMANAGER->play("hit", 1.0f);

							enemy->setIsAtk(true);
							friendly->setIsHit(true);
							enemy->setIsAtkRng(false);

							damage = ((enmyInfo.atk + enmyInfo.addAtk) - (friendInfo.def + friendInfo.addDef)) / 2 + enmyInfo.level + 25;
							friendly->hitDamage(damage);
						}
					}
				}
			}
		}
	}
}
