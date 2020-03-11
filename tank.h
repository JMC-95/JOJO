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
	vector<int> openList;		//���¸���Ʈ
	vector<int> closeList;		//Ŭ�����Ʈ
	vector<int>::iterator iter;
	stack<tagTile> optimalPath;	//ã�� ���� �����صδ� ����

	TANKDIRECTION _direction;	//ĳ������ ������ ������ enum��
	Select currentSelect;		//���� ������ ��ǥ ������ ������ enum��

	MainMap* _mainMap;			//���� �ҷ���
	animation* _ani;			//�ִϸ��̼�
	image* _image;				//�̹���

	RECT _rc;					//ĳ���� ��Ʈ
	
private:
	int speed;				//ĳ������ ���ǵ�
	int moveCount;			//ĳ������ �̵���
	int playerX, playerY;	//ĳ������ x��� y��
	int tileX, tileY;		//ĳ���Ͱ� ��� �ִ� Ÿ���� ��ȣ
	int mapX, mapY;			//���� ���� ��ǥ
	int sX, sY;				//���� ���� ��ǥ
	int startTile;			//A* ������ġ
	int endTile;			//A* ����ġ
	int currentTile;		//A* ������ġ

	bool startAstar;		//A* ���̽�Ÿ�� �����ϴ��� Ȯ���ϴ� ����
	bool isFind;			//A* ���� ã�Ҵ��� �� ã�Ҵ��� Ȯ���ϴ� ����
	bool noPath;			//A* ���� ã������ �ִ� �Ÿ��� ������� Ȯ���ϴ� ����
	bool isTurn;			//���� ĳ������ ������ Ȯ���ϴ� ����
	bool isRange;			//ĳ���͸� Ŭ���� ������ ǥ�����ִ� ����
	bool isMove;			//ĳ������ �̵��� üũ���ִ� ����

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
