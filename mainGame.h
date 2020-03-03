#pragma once
#include "gameNode.h"

class mainGame :  public gameNode
{
private:

public:
	mainGame();
	~mainGame();

	virtual HRESULT init();				//초기화
	virtual void release();				//해제
	virtual void update();				//연산하는곳
	virtual void render(/*HDC hdc*/);	//그리는곳
};