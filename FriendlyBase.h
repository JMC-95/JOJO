#pragma once
#include "MainMap.h"
#include "progressBar.h"

enum FriendDirection
{
	FRIEND_LEFT,
	FRIEND_RIGHT,
	FRIEND_UP,
	FRIEND_DOWN
};

struct FriendInfo
{
	image* moveRngImg;		//캐릭터 클릭시 이동범위 이미지
	image* moveAtkRngImg;	//캐릭터 클릭시 공격범위 이미지
	image* atkRngImg;		//공격버튼 클릭시 공격범위 이미지
	image* img;				//캐릭터 이미지
	image* atkImg;			//공격 이미지
	image* blockImg;		//방어 및 피격 이미지
	RECT rc;				//캐릭터 렉트
	RECT rcAtk[8];			//공격용 렉트
	int level;				//레벨
	int hp;					//체력
	int mp;					//마력
	int atk;				//공격력
	int will;				//정신력
	int def;				//방어력
	int agi;				//순발력
	int ten;				//사기
	int movingCount;		//이동력
};

class FriendlyBase
{
protected:
	vector<RECT> atkList;		//공격용 렉트를 담는 벡터
	vector<RECT> menuList;		//메뉴 렉트를 담는 벡터
	vector<int> openList;		//오픈리스트 벡터
	vector<int> closeList;		//클로즈리스트 벡터
	vector<int>::iterator iter;
	stack<tagTile> optimalPath;	//찾은 길을 저장해두는 스택

	FriendDirection fDirection;	//캐릭터 방향
	MainMap* mainMap;			//맵을 불러옴
	animation* friendAni;		//캐릭터 애니메이션
	progressBar* _Hp;			//HP Bar
	progressBar* _Mp;			//MP Bar
	progressBar* _Exp;			//EXP Bar

	RECT rcMenu[5];				//메뉴 렉트

protected:	//일반 변수
	int number;				//번호
	int currentHp, maxHp;	//체력
	int currentMp, maxMp;	//마력
	int currentExp, maxExp;	//경험치
	int speed;				//스피드
	int frameX;				//캐릭터의 프레임을 돌리기위한 변수
	int frameY;				//캐릭터의 프레임을 돌리기위한 변수
	int friendX, friendY;	//캐릭터 좌표
	int mapX, mapY;			//클릭한 맵의 중점 좌표
	int stackX, stackY;		//스택에 쌓아둔 타일의 중점 좌표 (길을 찾을 때 필요하다.)

	char str[128];

	bool isTurn;			//현재 캐릭터의 턴인지 확인하는 변수
	bool isSelect;			//캐릭터를 선택했는지 확인하는 변수
	bool isMove;			//캐릭터의 이동을 체크해주는 변수
	bool isStop;			//캐릭터의 이동을 막아주는 변수
	bool isClick;			//캐릭터의 메뉴 클릭
	bool isTarget;			//공격 범위에 적이 있는지 체크해주는 변수
	bool isAtkRng;			//적이 있다면 공격 버튼을 활성화해주는 변수
	bool isAtk;				//적을 공격할 때 체크해주는 변수
	bool isHit;

protected:	//A*용 변수
	int startTile;			//A* 시작위치
	int endTile;			//A* 끝위치
	int currentTile;		//A* 현재위치

	bool startAstar;		//A* 에이스타를 실행하는지 확인하는 변수
	bool isFind;			//A* 길을 찾았는지 못 찾았는지 확인하는 변수
	bool noPath;			//A* 길을 찾았으면 최단 거리가 어디인지 확인하는 변수

public:
	FriendlyBase();
	~FriendlyBase();

	//이동범위, 이동시 공격범위, 전투시 공격범위, 캐릭터 이미지, 공격시 이미지, 방어 or 피격 이미지
	virtual HRESULT init(const char* moveImg, const char* mAtkImg, const char* aRngImg, const char* playerImg, const char* atkImg, const char* blockImg);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

	virtual void mouseMove();
	virtual void friendMove();
	virtual void friendAstar();
	virtual void friendMenu();
	virtual void friendCollision();
	virtual void friendAnimation();
	virtual void friendState();

	virtual void setPosition(RECT rc);

	//A* 및 FloodFill
	void aStar();
	void floodFill(int tile, int moveCount);
	void hitDamage(int _damage);

	//맵의 주소
	void setMapMemoryAdress(MainMap* map) { mainMap = map; }

	//Getter
	int getFriendX() { return friendX; }
	int getFriendY() { return friendY; }
	int getFrameX() { return frameX; }
	int getCurrentHp() { return currentHp; }
	int getMaxHp() { return maxHp; }
	int getCurrentMp() { return currentMp; }
	int getMaxMp() { return maxMp; }
	int getCurrentExp() { return currentExp; }
	int getMaxExp() { return maxExp; }

	bool getIsTurn() { return isTurn; }
	bool getIsSelect() { return isSelect; }
	bool getIsCancel() { return isStop; }
	bool getIsAtkRng() { return isAtkRng; }
	bool getIsTarget() { return isTarget; }
	bool getIsAtk() { return isAtk; }
	bool getIsHit() { return isHit; }
	bool getIsClick() { return isClick; }

	//Setter
	void setIsTurn(bool turn) { isTurn = turn; }
	void setIsSelect(bool select) { isSelect = select; }
	void setIsCancel(bool cancel) { isStop = cancel; }
	void setIsTarget(bool target) { isTarget = target; }
	void setIsAtk(bool atk) { isAtk = atk; }
	void setIsHit(bool hit) { isHit = hit; }

	//프로그레스바
	progressBar* getProgressBarHp() { return _Hp; }
	progressBar* getProgressBarMp() { return _Mp; }
	progressBar* getProgressBarExp() { return _Exp; }

	virtual FriendInfo getFriendInfo() = 0;
};
