#include "stdafx.h"
#include "FriendlyManager.h"

FriendlyManager::FriendlyManager()
{
}

FriendlyManager::~FriendlyManager()
{
}

HRESULT FriendlyManager::init()
{
	vFriend.push_back(new Dogyeom);
	vFriend[0]->init("eMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	vFriend.push_back(new Gwanu);
	vFriend[1]->init("eMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	vFriend.push_back(new Jangbi);
	vFriend[2]->init("eMoveRange", "attackRange", "atkRange", "���", "���ATK", "���BH");

	vFriend.push_back(new Wonso);
	vFriend[3]->init("eMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	vFriend.push_back(new Yubi);
	vFriend[4]->init("eMoveRange", "attackRange", "atkRange", "����", "����ATK", "����BH");

	for (int i = 5; i < 9; i++)
	{
		vFriend.push_back(new Soldier(i));
		vFriend[i]->init("eMoveRange", "attackRange", "atkRange", "�챺����", "�챺����ATK", "�챺����BH");
	}

	return S_OK;
}

void FriendlyManager::release()
{
}

void FriendlyManager::update()
{
	friendTurn();

	for (int i = 0; i < vFriend.size(); i++)
	{
		vFriend[i]->update();
	}
}

void FriendlyManager::render(HDC hdc)
{
	for (int i = 0; i < vFriend.size(); i++)
	{
		vFriend[i]->render(hdc);
	}
}

void FriendlyManager::friendTurn()
{
	if (!PLAYERMANAGER->getPturn() && !ENEMYMANAGER->getEturn())
	{
		fTurn = false;

		for (int i = 0; i < FRIENDMANAGER->getFriend().size(); ++i)
		{
			auto& friendly = FRIENDMANAGER->getFriend()[i];

			if (friendly->getIsTurn())
			{
				fTurn = true;
			}
		}
	}
}
