#pragma once
#include "MainMap.h"
#include "progressBar.h"

enum EnemyDirection
{
	ENEMY_LEFT,
	ENEMY_RIGHT,
	ENEMY_UP,
	ENEMY_DOWN
};

struct EnemyInfo
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

class EnemyBase
{

protected:
	vector<RECT> atkList;		//���ݿ� ��Ʈ�� ��� ����
	vector<RECT> menuList;		//�޴� ��Ʈ�� ��� ����
	vector<int> openList;		//���¸���Ʈ ����
	vector<int> closeList;		//Ŭ�����Ʈ ����
	vector<int>::iterator iter;
	stack<tagTile> optimalPath;	//ã�� ���� �����صδ� ����

	EnemyDirection eDirection;	//ĳ���� ����
	MainMap* mainMap;		//���� �ҷ���
	animation* enemyAni;	//ĳ���� �ִϸ��̼�
	progressBar* _Hp;		//HP Bar
	progressBar* _Mp;		//MP Bar
	progressBar* _Exp;		//EXP Bar

	RECT rcMenu[5];			//�޴� ��Ʈ

protected:	//�Ϲ� ����
	int currentHp, maxHp;	//ü��
	int currentMp, maxMp;	//����
	int currentExp, maxExp;	//����ġ
	int damage;				//������
	int speed;				//���ǵ�
	int frameX;				//ĳ������ �������� ���������� ����
	int frameY;				//ĳ������ �������� ���������� ����
	int enemyX, enemyY;		//ĳ���� ��ǥ
	int mapX, mapY;			//Ŭ���� ���� ���� ��ǥ
	int stackX, stackY;		//���ÿ� �׾Ƶ� Ÿ���� ���� ��ǥ (���� ã�� �� �ʿ��ϴ�.)

	bool isTurn;			//���� ĳ������ ������ Ȯ���ϴ� ����
	bool isSelect;			//ĳ���͸� �����ߴ��� Ȯ���ϴ� ����
	bool isMove;			//ĳ������ �̵��� üũ���ִ� ����
	bool isStop;			//ĳ������ �̵��� ������ִ� ����
	bool isTarget;			//���� ������ ���� �ִ��� üũ���ִ� ����
	bool isAtkRng;			//���� �ִٸ� ���� ��ư�� Ȱ��ȭ���ִ� ����
	bool isClick;			//ĳ������ �޴� Ŭ��

protected:	//A*�� ����
	int startTile;			//A* ������ġ
	int endTile;			//A* ����ġ
	int currentTile;		//A* ������ġ

	bool startAstar;		//A* ���̽�Ÿ�� �����ϴ��� Ȯ���ϴ� ����
	bool isFind;			//A* ���� ã�Ҵ��� �� ã�Ҵ��� Ȯ���ϴ� ����
	bool noPath;			//A* ���� ã������ �ִ� �Ÿ��� ������� Ȯ���ϴ� ����

public:
	EnemyBase();
	~EnemyBase();

	//�̵�����, �̵��� ���ݹ���, ������ ���ݹ���, ĳ���� �̹���, ���ݽ� �̹���, ��� or �ǰ� �̹���
	virtual HRESULT init(const char* moveImg, const char* mAtkImg, const char* aRngImg, const char* playerImg, const char* atkImg, const char* blockImg);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

	//A* �� FloodFill
	void aStar();
	void floodFill(int tile, int moveCount);
	void hitDamage(int _damage);

	//���� �ּ�
	void setMapMemoryAdress(MainMap* map) { mainMap = map; }

	//GetSet
	int getEnemyX() { return enemyX; }
	int getEnemyY() { return enemyY; }
	int getFrameX() { return frameX; }
	int getCurrentHp() { return currentHp; }
	int getMaxHp() { return maxHp; }
	int getCurrentMp() { return currentMp; }
	int getMaxMp() { return maxMp; }
	int getCurrentExp() { return currentExp; }
	int getMaxExp() { return maxExp; }

	bool getIsSelect() { return isSelect; }
	bool getIsAtkRng() { return isAtkRng; }
	bool getIsAtk() { return isTarget; }
	bool getIsClick() { return isClick; }

	//���α׷�����
	progressBar* getProgressBarHp() { return _Hp; }
	progressBar* getProgressBarMp() { return _Mp; }
	progressBar* getProgressBarExp() { return _Exp; }
};
