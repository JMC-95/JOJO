#pragma once
#include "gameNode.h"

class LobbyScene : public gameNode
{
private:
	//�� ����
	vector<RECT> vMenu;				//�κ�
	vector<RECT> vOut;				//����
	vector<RECT> vEquipment;		//���
	vector<RECT> vBuy;				//����
	vector<RECT> vSell;				//�Ű�

	//������ ����
	vector<ItemInfo> vWeapon;		//����
	vector<ItemInfo> vArmor;		//����
	vector<ItemInfo> vAssistant;	//����
	vector<ItemInfo> vPotion;		//����
	vector<ItemInfo> vAllItem;		//��� ������

private:
	//�� ��Ʈ
	RECT menuRect[4];	//�κ�
	RECT outRect[9];	//����
	RECT equipRect[3];	//���
	RECT buyRect[5];	//����
	RECT sellRect[3];	//�Ű�

	//������ ��Ʈ
	RECT itemRect[13];

	int money;			//��
	int playerNum;		//ĳ���� �ѹ�
	int playerCount;	//ĳ���� ī��Ʈ
	int frameX;			//�̹��� ������

	char str[128];

	bool isBattle;		//����
	bool isInventory;	//���
	bool isBuyShop;		//����
	bool isSellShop;	//�Ű�

public:
	LobbyScene();
	~LobbyScene();

	HRESULT init();
	void release();
	void update();
	void render();

	void battle();
	void equipment();
	void buyShop();
	void sellShop();
};
