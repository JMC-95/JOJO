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

	RECT _rc;

public:
	tank();
	~tank();

	int _frameY;
	float _speed;
	float _x, _y;
	int tileX, tileY;//��ũ�� ��� �ִ� Ÿ�� ��ȣ

	HRESULT init(const char* imageName);
	void release();
	void update();
	void render();

	void tankMove();
	void setTankPosition(RECT rc);

	void setTestMapMemoryAdress(testMap* tm) { _testMap = tm; }
};
