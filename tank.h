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

	Select currentSelect;	//현재클릭

	int startTile;			//시작위치
	int endTile;			//끝위치
	int currentTile;		//현재위치

	//스택 중점 좌표
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
	int tileX, tileY;	//캐릭터가 밟고 있는 타일의 번호

	float x, y;			//캐릭터의 x축과 y축
	float speed;		//캐릭터의 스피드

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
