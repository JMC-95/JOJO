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
	image* moveRngImg;		//ĳ���� Ŭ���� �̵����� �̹���
	image* moveAtkRngImg;	//ĳ���� Ŭ���� ���ݹ��� �̹���
	image* atkRngImg;		//���ݹ�ư Ŭ���� ���ݹ��� �̹���
	image* img;				//ĳ���� �̹���
	image* atkImg;			//���� �̹���
	image* blockImg;		//��� �� �ǰ� �̹���
	RECT rc;				//ĳ���� ��Ʈ
	RECT rcAtk[8];			//���ݿ� ��Ʈ
	int level;				//����
	int hp;					//ü��
	int mp;					//����
	int atk;				//���ݷ�
	int will;				//���ŷ�
	int def;				//����
	int agi;				//���߷�
	int ten;				//���
	int movingCount;		//�̵���
};

class FriendlyBase
{
protected:
	vector<RECT> atkList;		//���ݿ� ��Ʈ�� ��� ����
	vector<RECT> menuList;		//�޴� ��Ʈ�� ��� ����
	vector<int> openList;		//���¸���Ʈ ����
	vector<int> closeList;		//Ŭ�����Ʈ ����
	vector<int>::iterator iter;
	stack<tagTile> optimalPath;	//ã�� ���� �����صδ� ����

	FriendDirection fDirection;	//ĳ���� ����
	MainMap* mainMap;			//���� �ҷ���
	animation* friendAni;		//ĳ���� �ִϸ��̼�
	progressBar* _Hp;			//HP Bar
	progressBar* _Mp;			//MP Bar
	progressBar* _Exp;			//EXP Bar

	RECT rcMenu[5];				//�޴� ��Ʈ

protected:	//�Ϲ� ����
	int number;				//��ȣ
	int currentHp, maxHp;	//ü��
	int currentMp, maxMp;	//����
	int currentExp, maxExp;	//����ġ
	int speed;				//���ǵ�
	int frameX;				//ĳ������ �������� ���������� ����
	int frameY;				//ĳ������ �������� ���������� ����
	int friendX, friendY;	//ĳ���� ��ǥ
	int mapX, mapY;			//Ŭ���� ���� ���� ��ǥ
	int stackX, stackY;		//���ÿ� �׾Ƶ� Ÿ���� ���� ��ǥ (���� ã�� �� �ʿ��ϴ�.)

	char str[128];

	bool isTurn;			//���� ĳ������ ������ Ȯ���ϴ� ����
	bool isSelect;			//ĳ���͸� �����ߴ��� Ȯ���ϴ� ����
	bool isMove;			//ĳ������ �̵��� üũ���ִ� ����
	bool isStop;			//ĳ������ �̵��� �����ִ� ����
	bool isClick;			//ĳ������ �޴� Ŭ��
	bool isTarget;			//���� ������ ���� �ִ��� üũ���ִ� ����
	bool isAtkRng;			//���� �ִٸ� ���� ��ư�� Ȱ��ȭ���ִ� ����
	bool isAtk;				//���� ������ �� üũ���ִ� ����
	bool isHit;

protected:	//A*�� ����
	int startTile;			//A* ������ġ
	int endTile;			//A* ����ġ
	int currentTile;		//A* ������ġ

	bool startAstar;		//A* ���̽�Ÿ�� �����ϴ��� Ȯ���ϴ� ����
	bool isFind;			//A* ���� ã�Ҵ��� �� ã�Ҵ��� Ȯ���ϴ� ����
	bool noPath;			//A* ���� ã������ �ִ� �Ÿ��� ������� Ȯ���ϴ� ����

public:
	FriendlyBase();
	~FriendlyBase();

	//�̵�����, �̵��� ���ݹ���, ������ ���ݹ���, ĳ���� �̹���, ���ݽ� �̹���, ��� or �ǰ� �̹���
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

	//A* �� FloodFill
	void aStar();
	void floodFill(int tile, int moveCount);
	void hitDamage(int _damage);

	//���� �ּ�
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

	//���α׷�����
	progressBar* getProgressBarHp() { return _Hp; }
	progressBar* getProgressBarMp() { return _Mp; }
	progressBar* getProgressBarExp() { return _Exp; }

	virtual FriendInfo getFriendInfo() = 0;
};
