#pragma once
#include "gameNode.h"
#include "testMap.h"

enum Select
{
	SELECT_START,
	SELECT_END
};

enum TANKDIRECTION
{
	TANKDIRECTION_LEFT,
	TANKDIRECTION_UP,
	TANKDIRECTION_RIGHT,
	TANKDIRECTION_DOWN
};

class tank : public gameNode
{
private: //A*
	vector<int> openList;
	vector<int> closeList;
	vector<int>::iterator iter;
	stack<tagTile> optimalPath;

	Select currentSelect;	//����Ŭ��

	int startTile;			//������ġ
	int endTile;			//����ġ
	int currentTile;		//������ġ

	//���� ���� ��ǥ
	int sX, sY;

	bool isFind;
	bool noPath;
	bool startAstar;

private:
	TANKDIRECTION _direction;
	testMap* _testMap;

	animation* _ani;
	image* _image;

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

	HRESULT init();
	void release();
	void update();
	void render();

	void Astar();
	void mouseClick();
	void tankMove();
	void animation();
	void setTankPosition(RECT rc);

	void setTestMapMemoryAdress(testMap* tm) { _testMap = tm; }
};
