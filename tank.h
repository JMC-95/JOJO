#pragma once
#include "gameNode.h"
#include "testMap.h"

enum TANKDIRECTION
{
	TANKDIRECTION_LEFT,
	TANKDIRECTION_UP,
	TANKDIRECTION_RIGHT,
	TANKDIRECTION_DOWN
};

class tank : public gameNode
{
private:
	TANKDIRECTION _direction;
	testMap* _testMap;
	animation* _ani;
	image* _image;
	image* img;

	RECT _rc;
	
private:
	int tileX, tileY;	//캐릭터가 밟고 있는 타일의 번호

	float x, y;			//캐릭터의 x축과 y축
	float speed;		//캐릭터의 스피드

	bool isTurn;
	bool isMove;

public:
	tank();
	~tank();

	HRESULT init(const char* imageName);
	void release();
	void update();
	void render();

	void mouseClick();
	void tankMove();
	void setTankPosition(RECT rc);

	void setTestMapMemoryAdress(testMap* tm) { _testMap = tm; }
};
