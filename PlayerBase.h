#pragma once
#include "MainMap.h"

enum Direction
{
	PLAYER_IDLE,
	PLAYER_LEFT,
	PLAYER_RIGHT,
	PLAYER_UP,
	PLAYER_DOWN
};

struct PlayerInfo
{
	image* moveRngImg;		//캐릭터 클릭시 이동범위 이미지
	image* mAtkRngImg;		//캐릭터 클릭시 공격범위 이미지
	image* atkRngImg;		//공격버튼 클릭시 공격범위 이미지
	image* img;				//캐릭터 이미지
	image* atkImg;			//공격 이미지
	image* blockImg;		//방어 및 피격 이미지
	RECT rc;				//캐릭터 렉트
	int hp;					//체력
	int mp;					//마력
	int exp;				//경험치
	int atk;				//공격력
	int will;				//정신력
	int def;				//방어력
	int agi;				//순발력
	int ten;				//사기
	int movingCount;		//이동력
	int speed;				//스피드
};

class PlayerBase
{
protected:
	vector<int> openList;		//오픈리스트 벡터
	vector<int> closeList;		//클로즈리스트 벡터
	vector<int>::iterator iter;
	stack<tagTile> optimalPath;	//찾은 길을 저장해두는 스택

	MainMap* mainMap;		//맵을 불러옴
	animation* playerAni;	//캐릭터 애니메이션
	Direction pDirection;	//캐릭터 방향

protected:	//일반 변수
	int playerX, playerY;	//캐릭터 좌표
	int mapX, mapY;			//클릭한 맵의 중점 좌표
	int stackX, stackY;		//스택에 쌓아둔 타일의 중점 좌표 (길을 찾을 때 필요하다.)

	bool isTurn;			//현재 캐릭터의 턴인지 확인하는 변수
	bool isSelect;			//캐릭터를 선택했는지 확인하는 변수
	bool isMove;			//캐릭터의 이동을 체크해주는 변수
	bool isAtk;				//캐릭터의 공격을 체크해주는 변수
	
protected:	//A*용 변수
	int startTile;			//A* 시작위치
	int endTile;			//A* 끝위치
	int currentTile;		//A* 현재위치

	bool startAstar;		//A* 에이스타를 실행하는지 확인하는 변수
	bool isFind;			//A* 길을 찾았는지 못 찾았는지 확인하는 변수
	bool noPath;			//A* 길을 찾았으면 최단 거리가 어디인지 확인하는 변수

public:
	PlayerBase();
	~PlayerBase();

	//이동범위, 이동시 공격범위, 전투시 공격범위, 캐릭터 이미지, 공격시 이미지, 방어 or 피격 이미지
	virtual HRESULT init(const char* moveImg, const char* mAtkImg, const char* aRngImg,
		const char* playerImg, const char* atkImg, const char* blockImg);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

	void aStar();
	void floodFill(int tile, int moveCount);

	void setMapMemoryAdress(MainMap* map) { mainMap = map; }

	bool getIsSelect() { return isSelect; }
};
