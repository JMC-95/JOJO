#pragma once
#include "MainMap.h"
#include "progressBar.h"

enum PlayerDirection
{
	PLAYER_LEFT,
	PLAYER_RIGHT,
	PLAYER_UP,
	PLAYER_DOWN
};

struct PlayerInfo
{
	image* skillImg;		//스킬 이미지
	image* moveRngImg;		//캐릭터 클릭시 이동범위 이미지
	image* moveAtkRngImg;	//캐릭터 클릭시 공격범위 이미지
	image* atkRngImg;		//공격버튼 클릭시 공격범위 이미지
	image* img;				//캐릭터 이미지
	image* atkImg;			//공격 이미지
	image* blockImg;		//방어 및 피격 이미지
	image* weaponImg;		//무기 이미지
	image* armorImg;		//갑옷 이미지

	RECT rc;				//캐릭터 렉트
	RECT rcAtk[8];			//공격용 렉트

	int level;				//레벨
	int hp;					//체력
	int mp;					//마력
	int atk;				//공격력
	int addAtk;				//추가 공격력
	int will;				//정신력
	int def;				//방어력
	int addDef;				//추가 방어력
	int agi;				//순발력
	int ten;				//사기
	int movingCount;		//이동력

	const char* name;		//이름
	const char* face;		//얼굴
	const char* className;	//병과
	const char* weaponName;	//무기 이름
	const char* armorName;	//갑옷 이름
};

class PlayerBase
{
protected:
	vector<RECT> atkList;		//공격용 렉트를 담는 벡터
	vector<RECT> menuList;		//메뉴 렉트를 담는 벡터
	vector<int> openList;		//오픈리스트 벡터
	vector<int> closeList;		//클로즈리스트 벡터
	vector<int>::iterator iter;
	stack<tagTile> optimalPath;	//찾은 길을 저장해두는 스택

	PlayerDirection pDirection;	//캐릭터 방향
	PlayerDirection sDirection;	//캐릭터 방향 저장용
	MainMap* mainMap;			//맵을 불러옴
	animation* playerAni;		//캐릭터 애니메이션
	animation* skillAni;		//스킬 애니메이션
	progressBar* _Hp;			//HP Bar
	progressBar* _Mp;			//MP Bar
	progressBar* _Exp;			//EXP Bar

	RECT rcMenu[5];				//메뉴 렉트
	RECT temp;

protected:	//일반 변수
	int levelCount;			//레벨 카운트
	int speed;				//스피드
	int currentHp, maxHp;	//체력
	int currentMp, maxMp;	//마력
	int currentExp, maxExp;	//경험치
	int playerX, playerY;	//캐릭터 좌표
	int mapX, mapY;			//클릭한 맵의 중점 좌표
	int stackX, stackY;		//스택에 쌓아둔 타일의 중점 좌표 (길을 찾을 때 필요하다.)
	int frameX, frameY;		//캐릭터의 프레임을 돌리기위한 변수

	char str[128];

	bool isTurn;			//현재 캐릭터의 턴인지 확인하는 변수
	bool isSelect;			//캐릭터를 선택했는지 확인하는 변수
	bool isMove;			//캐릭터의 이동을 체크해주는 변수
	bool isClick;			//캐릭터의 메뉴 클릭
	bool isTarget;			//공격 범위에 적이 있는지 체크해주는 변수
	bool isAtkRng;			//적이 있다면 공격 버튼을 활성화해주는 변수
	bool isAtk;				//적을 공격할 때 체크해주는 변수
	bool isHit;				//피격시 체크해주는 변수
	bool isSkill;			//책략 메뉴
	bool isSkillCheck;		//책략 체크
	bool isHeal;			//소보급
	bool isHealCheck;		//소보급 체크
	bool isLevelUp;			//레벨업을 했냐?

protected:	//A*용 변수
	int saveTile;			//시작 타일을 저장하는 변수
	int startTile;			//A* 시작위치
	int endTile;			//A* 끝위치
	int currentTile;		//A* 현재위치

	bool startAstar;		//A* 에이스타를 실행하는지 확인하는 변수
	bool isFind;			//A* 길을 찾았는지 못 찾았는지 확인하는 변수
	bool noPath;			//A* 길을 찾았으면 최단 거리가 어디인지 확인하는 변수

protected:	//미니맵용 변수
	int positionX, positionY;
	int playerTile;

public:
	PlayerBase();
	~PlayerBase();

	//스킬 이미지, 이동범위, 이동시 공격범위, 전투시 공격범위, 캐릭터 이미지, 공격시 이미지, 방어 or 피격 이미지
	virtual HRESULT init(const char* skillImg, const char* moveImg, const char* mAtkImg, const char* aRngImg, const char* playerImg, const char* atkImg, const char* blockImg);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

	virtual void mouseMove();
	virtual void playerMove();
	virtual void playerAstar();
	virtual void playerCollision();
	virtual void playerMenu();
	virtual void playerAnimation();
	virtual void playerState();

	virtual void setPosition(RECT rc);

	//A* 및 FloodFill
	void aStar();
	void floodFill(int tile, int moveCount);
	void skillFill(int tile, int moveCount);
	void hitDamage(int _damage);
	void healDamage(int _damage);
	void expDamaga(int exp);

	//맵의 주소
	void setMapMemoryAdress(MainMap* map) { mainMap = map; }

	//Getter
	int getPlayerX() { return playerX; }
	int getPlayerY() { return playerY; }
	int getFrameX() { return frameX; }
	int getCurrentHp() { return currentHp; }
	int getMaxHp() { return maxHp; }
	int getCurrentMp() { return currentMp; }
	int getMaxMp() { return maxMp; }
	int getCurrentExp() { return currentExp; }
	int getMaxExp() { return maxExp; }
	int getPlayerTile() { return playerTile; }

	bool getIsTurn() { return isTurn; }
	bool getIsSelect() { return isSelect; }
	bool getIsMove() { return isMove; }
	bool getIsAtkRng() { return isAtkRng; }
	bool getIsTarget() { return isTarget; }
	bool getIsAtk() { return isAtk; }
	bool getIsHit() { return isHit; }
	bool getIsClick() { return isClick; }
	bool getIsSkill() { return isSkill; }
	bool getIsSkillCheck() { return isSkillCheck; }
	bool getIsHeal() { return isHeal; }
	bool getIsLevelUp() { return isLevelUp; }

	//Setter
	void setIsTurn(bool turn) { isTurn = turn; }
	void setIsSelect(bool select) { isSelect = select; }
	void setIsMove(bool move) { isMove = move; }
	void setIsTarget(bool target) { isTarget = target; }
	void setIsAtkRng(bool atkRng) { isAtkRng = atkRng; }
	void setIsAtk(bool atk) { isAtk = atk; }
	void setIsHit(bool hit) { isHit = hit; }
	void setIsSkill(bool skill) { isSkill = skill; }
	void setIsSkillCheck(bool skillCheck) { isSkillCheck = skillCheck; }
	void setIsHeal(bool heal) { isHeal = heal; }

	//프로그레스바
	progressBar* getProgressBarHp() { return _Hp; }
	progressBar* getProgressBarMp() { return _Mp; }
	progressBar* getProgressBarExp() { return _Exp; }

	virtual PlayerInfo getPlayerInfo() = 0;
};
