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
	image* moveRngImg;		//ĳ���� Ŭ���� �̵����� �̹���
	image* mAtkRngImg;		//ĳ���� Ŭ���� ���ݹ��� �̹���
	image* atkRngImg;		//���ݹ�ư Ŭ���� ���ݹ��� �̹���
	image* img;				//ĳ���� �̹���
	image* atkImg;			//���� �̹���
	image* blockImg;		//��� �� �ǰ� �̹���
	RECT rc;				//ĳ���� ��Ʈ
	int hp;					//ü��
	int mp;					//����
	int exp;				//����ġ
	int atk;				//���ݷ�
	int will;				//���ŷ�
	int def;				//����
	int agi;				//���߷�
	int ten;				//���
	int movingCount;		//�̵���
	int speed;				//���ǵ�
};

class PlayerBase
{
protected:
	vector<int> openList;		//���¸���Ʈ ����
	vector<int> closeList;		//Ŭ�����Ʈ ����
	vector<int>::iterator iter;
	stack<tagTile> optimalPath;	//ã�� ���� �����صδ� ����

	MainMap* mainMap;		//���� �ҷ���
	animation* playerAni;	//ĳ���� �ִϸ��̼�
	Direction pDirection;	//ĳ���� ����

protected:	//�Ϲ� ����
	int playerX, playerY;	//ĳ���� ��ǥ
	int mapX, mapY;			//Ŭ���� ���� ���� ��ǥ
	int stackX, stackY;		//���ÿ� �׾Ƶ� Ÿ���� ���� ��ǥ (���� ã�� �� �ʿ��ϴ�.)

	bool isTurn;			//���� ĳ������ ������ Ȯ���ϴ� ����
	bool isSelect;			//ĳ���͸� �����ߴ��� Ȯ���ϴ� ����
	bool isMove;			//ĳ������ �̵��� üũ���ִ� ����
	bool isAtk;				//ĳ������ ������ üũ���ִ� ����
	
protected:	//A*�� ����
	int startTile;			//A* ������ġ
	int endTile;			//A* ����ġ
	int currentTile;		//A* ������ġ

	bool startAstar;		//A* ���̽�Ÿ�� �����ϴ��� Ȯ���ϴ� ����
	bool isFind;			//A* ���� ã�Ҵ��� �� ã�Ҵ��� Ȯ���ϴ� ����
	bool noPath;			//A* ���� ã������ �ִ� �Ÿ��� ������� Ȯ���ϴ� ����

public:
	PlayerBase();
	~PlayerBase();

	//�̵�����, �̵��� ���ݹ���, ������ ���ݹ���, ĳ���� �̹���, ���ݽ� �̹���, ��� or �ǰ� �̹���
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
