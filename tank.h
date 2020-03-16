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

class tank : public gameNode
{
private:
	vector<int> openList;		//���¸���Ʈ
	vector<int> closeList;		//Ŭ�����Ʈ
	vector<int>::iterator iter;
	stack<tagTile> optimalPath;	//ã�� ���� �����صδ� ����

	TANKDIRECTION direction;	//ĳ������ ������ ������ enum��

	MainMap* mainMap;		//���� �ҷ���
	animation* ani;			//�ִϸ��̼�
	image* rngImg;			//�̵����� �̹���
	image* atkRngImg;		//�̵����� ���ݹ��� �̹���
	image* atkImg;			//���� ���ݹ��� �̹���
	image* img;				//ĳ���� �̹���

	RECT _rc;				//ĳ���� ��Ʈ

private:
	int speed;				//ĳ������ ���ǵ�
	int movingCount;		//ĳ������ �̵���
	int playerX, playerY;	//ĳ������ ��ǥ
	int mapX, mapY;			//Ŭ���� ���� ���� ��ǥ
	int sX, sY;				//���ÿ� �׾Ƶ� Ÿ���� ���� ��ǥ (���� ã�� �� �ʿ��ϴ�.)
	int startTile;			//A* ������ġ
	int endTile;			//A* ����ġ
	int currentTile;		//A* ������ġ

	bool startAstar;		//A* ���̽�Ÿ�� �����ϴ��� Ȯ���ϴ� ����
	bool isFind;			//A* ���� ã�Ҵ��� �� ã�Ҵ��� Ȯ���ϴ� ����
	bool noPath;			//A* ���� ã������ �ִ� �Ÿ��� ������� Ȯ���ϴ� ����
	bool isTurn;			//���� ĳ������ ������ Ȯ���ϴ� ����
	bool isSelect;			//ĳ���͸� �����ߴ��� Ȯ���ϴ� ����
	bool isRange;			//ĳ������ ������ ǥ�����ִ� ����
	bool isMove;			//ĳ������ �̵��� üũ���ִ� ����
	bool isAtk;				//ĳ������ ������ üũ���ִ� ����

public:
	tank();
	~tank();

	HRESULT init(const char* rngImageName, const char* atkRngImageName, const char* atkImageName, const char* imageName);
	void release();
	void update();
	void render();

	void mouseMove();
	void aStar();
	void tankMove();
	void animation();
	void floodFill(int tile, int moveCount);

	void setTestMapMemoryAdress(MainMap* tm) { mainMap = tm; }
	void setTankPosition(RECT rc);
};
