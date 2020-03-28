#pragma once
#include "gameNode.h"

class PreferencesScene : public gameNode
{
private:
	RECT rc[3];

	bool bgmCheck = true;

public:
	PreferencesScene();
	~PreferencesScene();

	HRESULT init();
	void release();
	void update();
	void render();
};

