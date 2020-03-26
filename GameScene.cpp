#include "stdafx.h"
#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

HRESULT GameScene::init()
{
	//ÅÏ ÃÊ±âÈ­
	pTurn = 0;
	fTurn = 0;
	eTurn = 0;

	_mainMap = new MainMap;
	_mainMap->init("¸¼À½");

	_interface = new Interface;

	//¾Æ±º
	PLAYERMANAGER->init();
	for (int i = 0; i < PLAYERMANAGER->getPlayer().size(); i++)
	{
		PLAYERMANAGER->getPlayer()[i]->setMapMemoryAdress(_mainMap);
	}
	PLAYERMANAGER->getPlayer()[0]->setPosition(_mainMap->getMap()[197].rc);
	PLAYERMANAGER->getPlayer()[1]->setPosition(_mainMap->getMap()[196].rc);
	PLAYERMANAGER->getPlayer()[2]->setPosition(_mainMap->getMap()[236].rc);
	PLAYERMANAGER->getPlayer()[3]->setPosition(_mainMap->getMap()[176].rc);
	PLAYERMANAGER->getPlayer()[4]->setPosition(_mainMap->getMap()[216].rc);
	PLAYERMANAGER->getPlayer()[5]->setPosition(_mainMap->getMap()[177].rc);
	PLAYERMANAGER->getPlayer()[6]->setPosition(_mainMap->getMap()[237].rc);

	//¿ì±º
	FRIENDMANAGER->init();
	for (int i = 0; i < FRIENDMANAGER->getFriend().size(); i++)
	{
		FRIENDMANAGER->getFriend()[i]->setMapMemoryAdress(_mainMap);
	}
	FRIENDMANAGER->getFriend()[0]->setPosition(_mainMap->getMap()[111].rc);
	//FRIENDMANAGER->getFriend()[0]->setPosition(_mainMap->getMap()[117].rc);
	FRIENDMANAGER->getFriend()[1]->setPosition(_mainMap->getMap()[317].rc);
	FRIENDMANAGER->getFriend()[2]->setPosition(_mainMap->getMap()[277].rc);
	FRIENDMANAGER->getFriend()[3]->setPosition(_mainMap->getMap()[199].rc);
	FRIENDMANAGER->getFriend()[4]->setPosition(_mainMap->getMap()[297].rc);
	FRIENDMANAGER->getFriend()[5]->setPosition(_mainMap->getMap()[97].rc);
	FRIENDMANAGER->getFriend()[6]->setPosition(_mainMap->getMap()[137].rc);
	FRIENDMANAGER->getFriend()[7]->setPosition(_mainMap->getMap()[179].rc);
	FRIENDMANAGER->getFriend()[8]->setPosition(_mainMap->getMap()[219].rc);

	//Àû±º
	ENEMYMANAGER->init();
	for (int i = 0; i < ENEMYMANAGER->getEnemy().size(); i++)
	{
		ENEMYMANAGER->getEnemy()[i]->setMapMemoryAdress(_mainMap);
	}
	ENEMYMANAGER->getEnemy()[0]->setPosition(_mainMap->getMap()[204].rc);
	ENEMYMANAGER->getEnemy()[1]->setPosition(_mainMap->getMap()[182].rc);
	ENEMYMANAGER->getEnemy()[2]->setPosition(_mainMap->getMap()[202].rc);
	ENEMYMANAGER->getEnemy()[3]->setPosition(_mainMap->getMap()[184].rc);
	ENEMYMANAGER->getEnemy()[4]->setPosition(_mainMap->getMap()[188].rc);
	ENEMYMANAGER->getEnemy()[5]->setPosition(_mainMap->getMap()[169].rc);
	ENEMYMANAGER->getEnemy()[6]->setPosition(_mainMap->getMap()[189].rc);
	ENEMYMANAGER->getEnemy()[7]->setPosition(_mainMap->getMap()[209].rc);
	ENEMYMANAGER->getEnemy()[8]->setPosition(_mainMap->getMap()[229].rc);
	ENEMYMANAGER->getEnemy()[9]->setPosition(_mainMap->getMap()[167].rc);
	ENEMYMANAGER->getEnemy()[10]->setPosition(_mainMap->getMap()[187].rc);
	ENEMYMANAGER->getEnemy()[11]->setPosition(_mainMap->getMap()[207].rc);
	ENEMYMANAGER->getEnemy()[12]->setPosition(_mainMap->getMap()[227].rc);
	ENEMYMANAGER->getEnemy()[13]->setPosition(_mainMap->getMap()[148].rc);
	ENEMYMANAGER->getEnemy()[14]->setPosition(_mainMap->getMap()[168].rc);
	ENEMYMANAGER->getEnemy()[15]->setPosition(_mainMap->getMap()[208].rc);
	ENEMYMANAGER->getEnemy()[16]->setPosition(_mainMap->getMap()[228].rc);
	ENEMYMANAGER->getEnemy()[17]->setPosition(_mainMap->getMap()[248].rc);
	ENEMYMANAGER->getEnemy()[18]->setPosition(_mainMap->getMap()[125].rc);
	ENEMYMANAGER->getEnemy()[19]->setPosition(_mainMap->getMap()[165].rc);
	ENEMYMANAGER->getEnemy()[20]->setPosition(_mainMap->getMap()[205].rc);
	ENEMYMANAGER->getEnemy()[21]->setPosition(_mainMap->getMap()[225].rc);
	ENEMYMANAGER->getEnemy()[22]->setPosition(_mainMap->getMap()[265].rc);

	COLLISIONMANAGER->init();

	return S_OK;
}

void GameScene::release()
{
	SAFE_DELETE(_mainMap);
	SAFE_DELETE(_interface);
}

void GameScene::update()
{
	gameTurn();
	_mainMap->update();
	PLAYERMANAGER->update();
	FRIENDMANAGER->update();
	ENEMYMANAGER->update();

	ANIMATIONMANAGER->update();
	COLLISIONMANAGER->update();
}

void GameScene::render()
{
	_mainMap->render(getMemDC());
	PLAYERMANAGER->render(getMemDC());
	FRIENDMANAGER->render(getMemDC());
	ENEMYMANAGER->render(getMemDC());
	_interface->render(getMemDC());

	if (PLAYERMANAGER->getPturn())
	{
		if (pTurnCount < 60)
		{
			IMAGEMANAGER->render("playerTurn", getMemDC());

			HFONT myFont = CreateFont(100, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "³ª´®°íµñ ExtraBold");
			HFONT oldFont = (HFONT)SelectObject(getMemDC(), myFont);
			SetTextColor(getMemDC(), RGB(255, 255, 255));
			sprintf_s(str, "¾Æ±º  Â÷·Ê");
			TextOut(getMemDC(), 350, 400, str, strlen(str));
			SelectObject(getMemDC(), oldFont);
			DeleteObject(myFont);

			HFONT myFont2 = CreateFont(72, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "³ª´®°íµñ");
			HFONT oldFont2 = (HFONT)SelectObject(getMemDC(), myFont2);
			SetTextColor(getMemDC(), RGB(255, 255, 255));
			sprintf_s(str, "Á¦ %dÅÏ", pTurn);
			TextOut(getMemDC(), 460, 530, str, strlen(str));
			SelectObject(getMemDC(), oldFont2);
			DeleteObject(myFont2);
		}
	}
	else if (FRIENDMANAGER->getFturn())
	{
		if (fTurnCount < 60)
		{
			IMAGEMANAGER->render("friendTurn", getMemDC());

			HFONT myFont = CreateFont(100, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "³ª´®°íµñ ExtraBold");
			HFONT oldFont = (HFONT)SelectObject(getMemDC(), myFont);
			SetTextColor(getMemDC(), RGB(255, 255, 255));
			sprintf_s(str, "¿ì±º  Â÷·Ê");
			TextOut(getMemDC(), 350, 400, str, strlen(str));
			SelectObject(getMemDC(), oldFont);
			DeleteObject(myFont);

			HFONT myFont2 = CreateFont(72, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "³ª´®°íµñ");
			HFONT oldFont2 = (HFONT)SelectObject(getMemDC(), myFont2);
			SetTextColor(getMemDC(), RGB(255, 255, 255));
			sprintf_s(str, "Á¦ %dÅÏ", pTurn);
			TextOut(getMemDC(), 460, 530, str, strlen(str));
			SelectObject(getMemDC(), oldFont2);
			DeleteObject(myFont2);
		}
	}
	else if (ENEMYMANAGER->getEturn())
	{
		if (eTurnCount < 60)
		{
			IMAGEMANAGER->render("enemyTurn", getMemDC());

			HFONT myFont = CreateFont(100, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "³ª´®°íµñ ExtraBold");
			HFONT oldFont = (HFONT)SelectObject(getMemDC(), myFont);
			SetTextColor(getMemDC(), RGB(255, 255, 255));
			sprintf_s(str, "Àû±º  Â÷·Ê");
			TextOut(getMemDC(), 350, 400, str, strlen(str));
			SelectObject(getMemDC(), oldFont);
			DeleteObject(myFont);

			HFONT myFont2 = CreateFont(72, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "³ª´®°íµñ");
			HFONT oldFont2 = (HFONT)SelectObject(getMemDC(), myFont2);
			SetTextColor(getMemDC(), RGB(255, 255, 255));
			sprintf_s(str, "Á¦ %dÅÏ", pTurn);
			TextOut(getMemDC(), 460, 530, str, strlen(str));
			SelectObject(getMemDC(), oldFont2);
			DeleteObject(myFont2);
		}
	}
}

void GameScene::gameTurn()
{
	if (PLAYERMANAGER->getPturn())
	{
		pTurnCount++;

		if (pTurnCount == 1)
		{
			pTurn++;
		}
	}
	else if (!PLAYERMANAGER->getPturn()) pTurnCount = 0;

	if (FRIENDMANAGER->getFturn())
	{
		fTurnCount++;

		if (fTurnCount == 1)
		{
			fTurn++;
		}
	}
	else if (!FRIENDMANAGER->getFturn()) fTurnCount = 0;

	if (ENEMYMANAGER->getEturn())
	{
		eTurnCount++;

		if (eTurnCount == 1)
		{
			eTurn++;
		}
	}
	else if (!ENEMYMANAGER->getEturn()) eTurnCount = 0;
}
