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
	image* weaponImg;		//���� �̹���
	image* armorImg;		//���� �̹���
	image* asstImg;			//���� �̹���

	RECT rc;				//ĳ���� ��Ʈ
	RECT rcAtk[8];			//���ݿ� ��Ʈ

	int level;				//����
	int hp;					//ü��
	int mp;					//����
	int atk;				//���ݷ�
	int addAtk;				//�߰� ���ݷ�
	int will;				//���ŷ�
	int def;				//����
	int addDef;				//�߰� ����
	int agi;				//���߷�
	int ten;				//���
	int movingCount;		//�̵���

	const char* name;		//�̸�
	const char* face;		//��
	const char* className;	//����
	const char* weaponName;	//���� �̸�
	const char* armorName;	//���� �̸�
	const char* asstName;	//���� �̸�
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
	MainMap* mainMap;			//���� �ҷ���
	animation* enemyAni;		//ĳ���� �ִϸ��̼�
	progressBar* _Hp;			//HP Bar
	progressBar* _Mp;			//MP Bar
	progressBar* _Exp;			//EXP Bar

	RECT rcMenu[5];				//�޴� ��Ʈ

protected:	//�Ϲ� ����
	int number;				//������ ��ȣ
	int speed;				//���ǵ�
	int currentHp, maxHp;	//ü��
	int currentMp, maxMp;	//����
	int currentExp, maxExp;	//����ġ
	int enemyX, enemyY;		//ĳ���� ��ǥ
	int mapX, mapY;			//Ŭ���� ���� ���� ��ǥ
	int stackX, stackY;		//���ÿ� �׾Ƶ� Ÿ���� ���� ��ǥ (���� ã�� �� �ʿ��ϴ�.)
	int frameX, frameY;		//ĳ������ �������� ���������� ����

	char str[128];

	bool isTurn;			//���� ĳ������ ������ Ȯ���ϴ� ����
	bool isSelect;			//ĳ���͸� �����ߴ��� Ȯ���ϴ� ����
	bool isMove;			//ĳ������ �̵��� üũ���ִ� ����
	bool isClick;			//ĳ������ �޴� Ŭ��
	bool isTarget;			//���� ������ ���� �ִ��� üũ���ִ� ����
	bool isAtkRng;			//���� �ִٸ� ���� ��ư�� Ȱ��ȭ���ִ� ����
	bool isAtk;				//���� ������ �� üũ���ִ� ����
	bool isHit;				//�ǰݽ� üũ���ִ� ����

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

	virtual void mouseMove();
	virtual void enemyMove();
	virtual void enemyAstar();
	virtual void enemyCollision();
	virtual void enemyMenu();
	virtual void enemyAnimation();
	virtual void enemyState();

	virtual void setPosition(RECT rc);

	//A* �� FloodFill
	void aStar();
	void floodFill(int tile, int moveCount);
	void hitDamage(int _damage);

	//���� �ּ�
	void setMapMemoryAdress(MainMap* map) { mainMap = map; }

	//Getter
	int getEnemyX() { return enemyX; }
	int getEnemyY() { return enemyY; }
	int getFrameX() { return frameX; }
	int getCurrentHp() { return currentHp; }
	int getMaxHp() { return maxHp; }
	int getCurrentMp() { return currentMp; }
	int getMaxMp() { return maxMp; }
	int getCurrentExp() { return currentExp; }
	int getMaxExp() { return maxExp; }

	bool getIsTurn() { return isTurn; }
	bool getIsSelect() { return isSelect; }
	bool getIsMove() { return isMove; }
	bool getIsAtkRng() { return isAtkRng; }
	bool getIsTarget() { return isTarget; }
	bool getIsAtk() { return isAtk; }
	bool getIsHit() { return isHit; }
	bool getIsClick() { return isClick; }

	//Setter
	void setIsTurn(bool turn) { isTurn = turn; }
	void setIsSelect(bool select) { isSelect = select; }
	void setIsMove(bool cancel) { isMove = cancel; }
	void setIsTarget(bool target) { isTarget = target; }
	void setIsAtkRng(bool atkRng) { isAtkRng = atkRng; }
	void setIsAtk(bool atk) { isAtk = atk; }
	void setIsHit(bool hit) { isHit = hit; }

	//���α׷�����
	progressBar* getProgressBarHp() { return _Hp; }
	progressBar* getProgressBarMp() { return _Mp; }
	progressBar* getProgressBarExp() { return _Exp; }

	virtual EnemyInfo getEnemyInfo() = 0;
};
