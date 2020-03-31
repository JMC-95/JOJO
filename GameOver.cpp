#include "stdafx.h"
#include "GameOver.h"

GameOver::GameOver()
{
}

GameOver::~GameOver()
{
}

HRESULT GameOver::init()
{
	//BGM
	if (isSound)
	{
		if (ENEMYMANAGER->getIsClear())
		{
			isGameClear = true;

			SOUNDMANAGER->stop("gameSound");
			SOUNDMANAGER->play("endingSound", 1.0f);
		}
		else if (PLAYERMANAGER->getPlayer()[0]->getCurrentHp() <= 0)
		{
			isGameOver = true;

			SOUNDMANAGER->stop("gameSound");
			SOUNDMANAGER->play("deathSound", 1.0f);
		}
	}

	return S_OK;
}

void GameOver::release()
{
}

void GameOver::update()
{
}

void GameOver::render()
{
	if (isGameClear) IMAGEMANAGER->render("gameClear", getMemDC());
	else if (isGameOver) IMAGEMANAGER->render("gameOver", getMemDC());
}
