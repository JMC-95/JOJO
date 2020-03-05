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
	int tileX, tileY;	//ĳ���Ͱ� ��� �ִ� Ÿ���� ��ȣ

	float x, y;			//ĳ������ x��� y��
	float speed;		//ĳ������ ���ǵ�

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
