#pragma once
#include "gameNode.h"
#include "MainMap.h"

enum Direction
{
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_LEFTUP,
	DIRECTION_RIGHTUP,
	DIRECTION_LEFTDOWN,
	DIRECTION_RIGHTDOWN
};

enum TANKDIRECTION
{
	TANKDIRECTION_LEFT,
	TANKDIRECTION_UP,
	TANKDIRECTION_RIGHT,
	TANKDIRECTION_DOWN
};

enum Select
{
	SELECT_START,
	SELECT_END
};

class tank : public gameNode
{
private:
	vector<int> openList;		//오픈리스트
	vector<int> closeList;		//클로즈리스트
	vector<int>::iterator iter;
	stack<tagTile> optimalPath;	//찾은 길을 저장해두는 스택

	TANKDIRECTION _direction;	//캐릭터의 방향을 설정할 enum문
	Select currentSelect;		//시작 지점과 목표 지점을 설정할 enum문

	MainMap* _mainMap;			//맵을 불러옴
	animation* _ani;			//애니메이션
	image* _image;				//이미지

	RECT _rc;					//캐릭터 렉트
	
private:
	int speed;				//캐릭터의 스피드
	int moveCount;			//캐릭터의 이동력
	int playerX, playerY;	//캐릭터의 x축과 y축
	int tileX, tileY;		//캐릭터가 밟고 있는 타일의 번호
	int mapX, mapY;			//맵의 중점 좌표
	int sX, sY;				//스택 중점 좌표
	int startTile;			//A* 시작위치
	int endTile;			//A* 끝위치
	int currentTile;		//A* 현재위치

	bool startAstar;		//A* 에이스타를 실행하는지 확인하는 변수
	bool isFind;			//A* 길을 찾았는지 못 찾았는지 확인하는 변수
	bool noPath;			//A* 길을 찾았으면 최단 거리가 어디인지 확인하는 변수
	bool isTurn;			//현재 캐릭터의 턴인지 확인하는 변수
	bool isRange;			//캐릭터를 클릭시 범위를 표시해주는 변수
	bool isMove;			//캐릭터의 이동을 체크해주는 변수

public:
	tank();
	~tank();

	HRESULT init(const char* imageName);
	void release();
	void update();
	void render();

	void mouseClick();
	void aStar();
	void tankMove();
	void animation();

	void setTestMapMemoryAdress(MainMap* tm) { _mainMap = tm; }
	void setTankPosition(RECT rc);
};
