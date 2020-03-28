#pragma once
#include "gameNode.h"

class TitleScene : public gameNode
{
private:
	RECT listRect[4];

public:
	TitleScene();
	~TitleScene();

	HRESULT init();
	void release();
	void update();
	void render();
};

