#pragma once
#include "gameNode.h"

class MainMenu : public gameNode
{
private:
	image* mainMenuImage;
	image* cursorImage;

	RECT temp;				// 충돌용 렉트
	RECT newRunRect;		// 새 게임 렉트
	RECT mapToolRect;		// 맵툴 렉트
	RECT optionRect;		// 옵션 렉트
	RECT cursorRect;		// 커서 렉트

	char str[128];

public:
	MainMenu();
	~MainMenu();

	HRESULT init();
	void release();
	void update();
	void render();

	void CursorMove();
};