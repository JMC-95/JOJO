#include "stdafx.h"
#include "Dogyeom.h"

Dogyeom::Dogyeom()
{
}

Dogyeom::~Dogyeom()
{
}

HRESULT Dogyeom::init(const char * moveImg, const char * mAtkImg, const char * aRngImg, const char * friendImg, const char * atkImg, const char * blockImg)
{
	//이름 및 얼굴
	dogyeom.name = "도겸";
	dogyeom.face = "도겸Face";
	dogyeom.className = "군웅";
	//이미지 및 애니메이션
	dogyeom.moveRngImg = IMAGEMANAGER->findImage(moveImg);		//캐릭터 클릭시 이동범위 이미지
	dogyeom.moveAtkRngImg = IMAGEMANAGER->findImage(mAtkImg);	//캐릭터 클릭시 공격범위 이미지
	dogyeom.atkRngImg = IMAGEMANAGER->findImage(aRngImg);		//공격버튼 클릭시 공격범위 이미지
	dogyeom.img = IMAGEMANAGER->findImage(friendImg);			//캐릭터 이미지
	dogyeom.atkImg = IMAGEMANAGER->findImage(atkImg);			//공격 이미지
	dogyeom.blockImg = IMAGEMANAGER->findImage(blockImg);		//방어 및 피격 이미지
	//스테이터스
	dogyeom.level = 6;			//레벨
	dogyeom.hp = 125;			//체력
	dogyeom.mp = 15;			//마력
	dogyeom.atk = 35;			//공격력
	dogyeom.will = 43;			//정신력
	dogyeom.def = 36;			//방어력
	dogyeom.agi = 62;			//순발력
	dogyeom.ten = 62;			//사기
	dogyeom.movingCount = 6;	//이동력

	//HP ProgressBar
	_Hp = new progressBar;
	_Hp->init("images/UI/Info/HP.bmp", "images/UI/Info/Back_P.bmp", 1056, 289, 84, 12);
	_Hp->setGauge(currentHp, maxHp);
	currentHp = maxHp = 125;

	//MP ProgressBar
	_Mp = new progressBar;
	_Mp->init("images/UI/Info/MP.bmp", "images/UI/Info/Back_P.bmp", 1056, 307, 84, 12);
	_Mp->setGauge(currentMp, maxMp);
	currentMp = maxMp = 15;

	//EXP ProgressBar
	_Exp = new progressBar;
	_Exp->init("images/UI/Info/EXP.bmp", "images/UI/Info/Back_EXP.bmp", 1095, 243, 45, 12);
	_Exp->setGauge(currentExp, maxExp);
	currentExp = 0;
	maxExp = 100;

	//캐릭터 방향 및 위치
	fDirection = FRIEND_LEFT;
	startTile = endTile = -1;
	enemyTile = 0;

	//AI 이동력 및 속도
	speed = 12;

	isTurn = true;
	isMove = true;
	isSelect = false;

	return S_OK;
}

void Dogyeom::release()
{
	SAFE_DELETE(_Hp);
	SAFE_DELETE(_Mp);
	SAFE_DELETE(_Exp);
}

void Dogyeom::update()
{
	if (isTurn)
	{
		//if (!PLAYERMANAGER->getPturn()) friendAi();
		friendAi();
	}

	friendAnimation();
	friendState();
	mouseMove();

	if (KEYMANAGER->isOnceKeyDown('3'))
	{
		isTurn = true;
		isMove = true;
	}
}

void Dogyeom::render(HDC hdc)
{
	if (isTurn)
	{
		if (isAtk)
		{
			dogyeom.atkImg->aniRender(hdc, dogyeom.rc.left - 8, dogyeom.rc.top - 8, friendAni);
		}
		else if (isHit)
		{
			dogyeom.blockImg->frameRender(hdc, dogyeom.rc.left, dogyeom.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, dogyeom.rc.left, dogyeom.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			dogyeom.img->aniRender(hdc, dogyeom.rc.left, dogyeom.rc.top, friendAni);
		}
	}
	else
	{
		if (isHit)
		{
			dogyeom.blockImg->frameRender(hdc, dogyeom.rc.left, dogyeom.rc.top, 0, 4);

			HFONT myFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "나눔고딕체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			sprintf_s(str, "%d", COLLISIONMANAGER->getDamage());
			TextOut(hdc, dogyeom.rc.left, dogyeom.rc.top, str, strlen(str));
			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			dogyeom.img->frameAlphaRender(hdc, dogyeom.rc.left, dogyeom.rc.top, 0, frameY, 100);
		}
	}
}

void Dogyeom::friendAi()
{
	RECT temp;

	if (KEYMANAGER->isOnceKeyDown('1'))
	{
		//턴이 되면 벡터를 초기화 하자.
		vAstar.clear();

		//가장 가까운 적의 위치를 찾는 함수
		aiAstar();

		positionX = dogyeom.rc.left / TILE_WIDTH;
		positionY = dogyeom.rc.top / TILE_HEIGHT;
		friendTile = positionX + (positionY * TILE_Y);

		//선택한 타일 (캐릭터)
		startTile = friendTile;

		isSelect = true;
		isFind = false;
		noPath = false;
		startAstar = false;

		//공격범위
		for (int j = 0; j < 4; j++)
		{
			dogyeom.rcAtk[0] = RectMake(dogyeom.rc.left - 48, dogyeom.rc.top, TILE_WIDTH, TILE_HEIGHT);
			dogyeom.rcAtk[1] = RectMake(dogyeom.rc.left + 48, dogyeom.rc.top, TILE_WIDTH, TILE_HEIGHT);
			dogyeom.rcAtk[2] = RectMake(dogyeom.rc.left, dogyeom.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
			dogyeom.rcAtk[3] = RectMake(dogyeom.rc.left, dogyeom.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
			atkList.push_back(dogyeom.rcAtk[j]);
		}

		//이동범위
		floodFill(startTile, dogyeom.movingCount);

		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			if (mainMap->getMap()[i].flood)
			{
				for (int k = 0; k < PLAYERMANAGER->getPlayer().size(); ++k)
				{
					auto& rc = mainMap->getMap()[i].rc;
					auto& playerRect = PLAYERMANAGER->getPlayer()[k]->getPlayerInfo().rc;

					if (IntersectRect(&temp, &rc, &playerRect))
					{
						mainMap->getMap()[i].flood = false;
					}
				}

				for (int k = 0; k < FRIENDMANAGER->getFriend().size(); ++k)
				{
					auto& rc = mainMap->getMap()[i].rc;
					auto& friendRect = FRIENDMANAGER->getFriend()[k]->getFriendInfo().rc;

					if (IntersectRect(&temp, &rc, &friendRect))
					{
						mainMap->getMap()[i].flood = false;
					}
				}

				for (int k = 4; k < 18; ++k)
				{
					auto& rc = mainMap->getMap()[i].rc;
					auto& enemyRect = ENEMYMANAGER->getEnemy()[k]->getEnemyInfo().rc;

					if (IntersectRect(&temp, &rc, &enemyRect))
					{
						mainMap->getMap()[i].flood = false;
					}
				}
			}
		}
	}

	if (KEYMANAGER->isOnceKeyDown('2') && isSelect)
	{
		SOUNDMANAGER->play("cMove", 1.0f);

		//찾은 적의 타일 번호
		positionX = ENEMYMANAGER->getEnemy()[enemyNum]->getEnemyInfo().rc.left / TILE_WIDTH;
		positionY = ENEMYMANAGER->getEnemy()[enemyNum]->getEnemyInfo().rc.top / TILE_WIDTH;
		enemyTile = positionX + (positionY * TILE_Y);

		//선택한 타일(목표)
		endTile = enemyTile;

		//선택한 맵의 x좌표와 y좌표
		mapX = mainMap->getMap()[endTile].rc.left + TILE_WIDTH * 0.5;
		mapY = mainMap->getMap()[endTile].rc.top + TILE_HEIGHT * 0.5;

		//이순간 Astar가 시작된다.
		//Astar에 필요한 모든 것을 초기화 시켜주자.
		openList.clear();
		closeList.clear();

		if (startTile != -1 && endTile != -1)
		{
			startAstar = true;
			currentTile = startTile;

			//시작 지점을 openList에 넣자
			openList.push_back(currentTile);
		}

		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			if (mainMap->getMap()[i].flood)
			{
				mainMap->getMap()[i].flood = false;
			}
		}
	}

	friendAstar();
	friendCollision();
	friendMenu();
}

void Dogyeom::friendMove()
{
	stackX = optimalPath.top().rc.left + (optimalPath.top().rc.right - optimalPath.top().rc.left) / 2;
	stackY = optimalPath.top().rc.top + (optimalPath.top().rc.bottom - optimalPath.top().rc.top) / 2;

	if (friendX > stackX)
	{
		fDirection = FRIEND_LEFT;
	}
	else if (friendX < stackX)
	{
		fDirection = FRIEND_RIGHT;
	}
	else if (friendY > stackY)
	{
		fDirection = FRIEND_UP;
	}
	else if (friendY < stackY)
	{
		fDirection = FRIEND_DOWN;
	}

	if (dogyeom.rc.left > 0 || dogyeom.rc.right < WINSIZEY ||
		dogyeom.rc.top > 0 || dogyeom.rc.bottom < WINSIZEY)
	{
		switch (fDirection)
		{
		case FRIEND_LEFT:
			friendX -= speed;
			dogyeom.rc = RectMakeCenter(friendX, friendY, dogyeom.img->getFrameWidth(), dogyeom.img->getFrameHeight());
			break;
		case FRIEND_RIGHT:
			friendX += speed;
			dogyeom.rc = RectMakeCenter(friendX, friendY, dogyeom.img->getFrameWidth(), dogyeom.img->getFrameHeight());
			break;
		case FRIEND_UP:
			friendY -= speed;
			dogyeom.rc = RectMakeCenter(friendX, friendY, dogyeom.img->getFrameWidth(), dogyeom.img->getFrameHeight());
			break;
		case FRIEND_DOWN:
			friendY += speed;
			dogyeom.rc = RectMakeCenter(friendX, friendY, dogyeom.img->getFrameWidth(), dogyeom.img->getFrameHeight());
			break;
		}

		if (friendX == stackX && friendY == stackY)
		{
			optimalPath.pop();
		}
	}
}

void Dogyeom::friendAstar()
{
	//목표 타일을 클릭하면 A* 시작
	if (startAstar && !isFind && !noPath)
	{
		while (!isFind)
		{
			aStar();
		}
	}

	//목표 타일을 클릭하면 캐릭터 이동
	if (!optimalPath.empty())
	{
		if (isMove) friendMove();

		if (friendX == mapX && friendY == mapY)
		{
			//isTurn = false;
			isMove = false;
			isClick = true;

			//공격범위
			for (int j = 0; j < 4; j++)
			{
				dogyeom.rcAtk[0] = RectMake(dogyeom.rc.left - 48, dogyeom.rc.top, TILE_WIDTH, TILE_HEIGHT);
				dogyeom.rcAtk[1] = RectMake(dogyeom.rc.left + 48, dogyeom.rc.top, TILE_WIDTH, TILE_HEIGHT);
				dogyeom.rcAtk[2] = RectMake(dogyeom.rc.left, dogyeom.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
				dogyeom.rcAtk[3] = RectMake(dogyeom.rc.left, dogyeom.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
				atkList.push_back(dogyeom.rcAtk[j]);
			}
		}
	}

	if (isClick)
	{
		//메뉴선택 렉트
		for (int j = 0; j < 5; j++)
		{
			rcMenu[0] = RectMake(dogyeom.rc.left - 97, dogyeom.rc.top - 30, 82, 20);
			rcMenu[1] = RectMake(dogyeom.rc.left - 97, dogyeom.rc.top - 9, 82, 20);
			rcMenu[2] = RectMake(dogyeom.rc.left - 97, dogyeom.rc.top + 12, 82, 20);
			rcMenu[3] = RectMake(dogyeom.rc.left - 97, dogyeom.rc.top + 38, 82, 20);
			rcMenu[4] = RectMake(dogyeom.rc.left - 97, dogyeom.rc.top + 63, 82, 20);
			menuList.push_back(rcMenu[j]);
		}
	}
}

void Dogyeom::friendMenu()
{
	//메뉴
	if (isClick)
	{
		if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
		{
			if (PtInRect(&rcMenu[0], m_ptMouse) && isTarget)	//공격
			{
				atkList.clear();
				menuList.clear();

				isAtkRng = true;
				isClick = false;
			}
			if (PtInRect(&rcMenu[1], m_ptMouse))	//책략
			{
				atkList.clear();
				menuList.clear();

				//isClick = false;
			}
			if (PtInRect(&rcMenu[2], m_ptMouse))	//도구
			{
				atkList.clear();
				menuList.clear();

				//isClick = false;
			}
			if (PtInRect(&rcMenu[3], m_ptMouse))	//대기
			{
				atkList.clear();
				menuList.clear();

				isMove = true;
				isTurn = false;
				isSelect = false;
				isClick = false;
			}
			if (PtInRect(&rcMenu[4], m_ptMouse))	//취소
			{
				atkList.clear();
				menuList.clear();

				isMove = true;
				isSelect = false;
				isClick = false;
			}
		}
	}
}

void Dogyeom::friendCollision()
{
	RECT temp;
	frameX = 0;

	for (int j = 0; j < ENEMYMANAGER->getEnemy().size(); j++)
	{
		auto enemyX = ENEMYMANAGER->getEnemy()[j]->getEnemyX();
		auto enemyY = ENEMYMANAGER->getEnemy()[j]->getEnemyY();
		auto enemyHit = ENEMYMANAGER->getEnemy()[j]->getIsHit();
		auto& enemyRect = ENEMYMANAGER->getEnemy()[j]->getEnemyInfo().rc;

		for (int k = 0; k < 4; k++)
		{
			if (IntersectRect(&temp, &dogyeom.rcAtk[k], &enemyRect))
			{
				isTarget = true;
				frameX = 1;
			}
		}

		if (enemyHit)
		{
			if (friendX > enemyX) fDirection = FRIEND_LEFT;
			else if (friendX < enemyX) fDirection = FRIEND_RIGHT;
			else if (friendY > enemyY) fDirection = FRIEND_UP;
			else if (friendY < enemyY) fDirection = FRIEND_DOWN;
		}
	}
}

void Dogyeom::friendAnimation()
{
	if (isTurn)
	{
		if (isAtk)
		{
			switch (fDirection)
			{
			case FRIEND_LEFT:
				ANIMATIONMANAGER->addAnimation("leftAtk", "도겸ATK", 12, 15, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("leftAtk");
				ANIMATIONMANAGER->resume("leftAtk");
				break;
			case FRIEND_RIGHT:
				ANIMATIONMANAGER->addAnimation("rightAtk", "도겸ATK", 8, 11, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("rightAtk");
				ANIMATIONMANAGER->resume("rightAtk");
				break;
			case FRIEND_UP:
				ANIMATIONMANAGER->addAnimation("upAtk", "도겸ATK", 4, 7, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("upAtk");
				ANIMATIONMANAGER->resume("upAtk");
				break;
			case FRIEND_DOWN:
				ANIMATIONMANAGER->addAnimation("downAtk", "도겸ATK", 0, 3, 5, false, false);
				friendAni = ANIMATIONMANAGER->findAnimation("downAtk");
				ANIMATIONMANAGER->resume("downAtk");
				break;
			}
		}
		else
		{
			switch (fDirection)
			{
			case FRIEND_LEFT:
				ANIMATIONMANAGER->addAnimation("friendLeft", "도겸", 4, 5, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendLeft");
				ANIMATIONMANAGER->resume("friendLeft");
				break;
			case FRIEND_RIGHT:
				ANIMATIONMANAGER->addAnimation("friendRight", "도겸", 6, 7, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendRight");
				ANIMATIONMANAGER->resume("friendRight");
				break;
			case FRIEND_UP:
				ANIMATIONMANAGER->addAnimation("friendUp", "도겸", 2, 3, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendUp");
				ANIMATIONMANAGER->resume("friendUp");
				break;
			case FRIEND_DOWN:
				ANIMATIONMANAGER->addAnimation("friendDown", "도겸", 0, 1, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendDown");
				ANIMATIONMANAGER->resume("friendDown");
				break;
			}

			if (currentHp < 40 && !isSelect && isMove)
			{
				ANIMATIONMANAGER->addAnimation("friendHp", "도겸", 12, 13, 2, false, true);
				friendAni = ANIMATIONMANAGER->findAnimation("friendHp");
				ANIMATIONMANAGER->resume("friendHp");
			}
		}
	}
	else
	{
		switch (fDirection)
		{
		case FRIEND_LEFT:
			frameY = 10;
			break;
		case FRIEND_RIGHT:
			frameY = 11;
			break;
		case FRIEND_UP:
			frameY = 9;
			break;
		case FRIEND_DOWN:
			frameY = 8;
			break;
		}
	}
}

void Dogyeom::friendState()
{
	_Hp->update();
	_Hp->setGauge(currentHp, maxHp);
	_Mp->update();
	_Mp->setGauge(currentMp, maxMp);
	_Exp->update();
	_Exp->setGauge(currentExp, maxExp);

	if (currentHp < 0) currentHp = 0;
}

void Dogyeom::mouseMove()
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && PtInRect(&dogyeom.rc, m_ptMouse))
		{
			if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
			{
				//선택한 타일 (캐릭터)
				startTile = i;

				//캐릭터 클릭
				isSelect = true;

				//공격범위
				for (int j = 0; j < 4; j++)
				{
					dogyeom.rcAtk[0] = RectMake(dogyeom.rc.left - 48, dogyeom.rc.top, TILE_WIDTH, TILE_HEIGHT);
					dogyeom.rcAtk[1] = RectMake(dogyeom.rc.left + 48, dogyeom.rc.top, TILE_WIDTH, TILE_HEIGHT);
					dogyeom.rcAtk[2] = RectMake(dogyeom.rc.left, dogyeom.rc.top - 48, TILE_WIDTH, TILE_HEIGHT);
					dogyeom.rcAtk[3] = RectMake(dogyeom.rc.left, dogyeom.rc.top + 48, TILE_WIDTH, TILE_HEIGHT);
					atkList.push_back(dogyeom.rcAtk[j]);
				}

				//이동범위
				if (isMove) floodFill(startTile, dogyeom.movingCount);
			}
		}

		if (PtInRect(&mainMap->getMap()[i].rc, m_ptMouse) && isSelect)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				//캐릭터 클릭
				isSelect = false;

				//공격범위
				atkList.clear();

				//이동범위
				for (int i = 0; i < TILE_X * TILE_Y; i++)
				{
					if (mainMap->getMap()[i].flood) mainMap->getMap()[i].flood = false;
				}
			}
		}
	}
}

void Dogyeom::setPosition(RECT rc)
{
	dogyeom.rc = rc;
	friendX = dogyeom.rc.left + (dogyeom.rc.right - dogyeom.rc.left) / 2;
	friendY = dogyeom.rc.top + (dogyeom.rc.bottom - dogyeom.rc.top) / 2;
}

void Dogyeom::aiAstar()
{
	//캐릭터의 위치에서 가장 가까운 적의 위치를 찾아내는 Astar
	for (int i = 0; i < 14; i++)
	{
		int enemyNum = i + 4;
		auto& enemyRect = ENEMYMANAGER->getEnemy()[enemyNum]->getEnemyInfo().rc;

		positionX = dogyeom.rc.left / TILE_WIDTH;
		positionY = dogyeom.rc.top / TILE_HEIGHT;
		friendTile = positionX + (positionY * TILE_Y);

		int ePositionX = enemyRect.left / TILE_WIDTH;
		int ePositionY = enemyRect.top / TILE_HEIGHT;
		enemyTile = ePositionX + (ePositionY * TILE_Y);

		isFind = false;
		noPath = false;
		startAstar = false;

		//선택한 타일 (캐릭터)
		startTile = friendTile;

		//선택한 타일(목표)
		endTile = enemyTile;

		//이순간 Astar가 시작된다.
		//Astar에 필요한 모든 것을 초기화 시켜주자.
		openList.clear();
		closeList.clear();

		if (startTile != -1 && endTile != -1)
		{
			startAstar = true;
			currentTile = startTile;

			//시작 지점을 openList에 넣자
			openList.push_back(currentTile);
		}

		//목표 타일을 클릭하면 A* 시작
		if (startAstar && !isFind && !noPath)
		{
			while (!isFind)
			{
				aStar();
			}
		}

		//벡터에 optimalPath 값을 각각 넣어줌.
		vAstar.push_back(optimalPath.size());

		//for문을 실행하지 않으면 optimalPath의 값이 계속 중첩됨.
		for (int j = optimalPath.size(); j > 0; j--)
		{
			optimalPath.pop();
		}
	}

	//최소값을 찾을땐 999, 최대값을 찾을땐 0을 입력한다.
	int minNum = vAstar[0];	//최소값

	//for문을 실행하면서 optimalPath의 값이 가장 작은 위치의 적을 찾아줌. 
	for (int i = 0; i < vAstar.size(); i++)
	{
		int distance = vAstar[i];

		if (minNum > vAstar[i]) minNum = vAstar[i];

		if (minNum == distance) enemyNum = i + 4;
	}
}