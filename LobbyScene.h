#pragma once
#include "gameNode.h"

class LobbyScene : public gameNode
{
private:
	//�κ񿡼� ���Ǵ� ��Ʈ�� ��Ƶδ� ����
	vector<RECT> vMenu;			//�κ�
	vector<RECT> vOut;			//����
	vector<RECT> vEquipment;	//���
	vector<RECT> vBuy;			//����
	vector<RECT> vSell;			//�Ű�

	//������ ����
	vector<ItemInfo> vWeapon;	//����
	vector<ItemInfo> vArmor;	//����
	vector<ItemInfo> vPotion;	//����
	vector<ItemInfo> vAllItem;	//��� ������
	vector<ItemInfo> vEquipItem;//���â ������

private:
	//�κ񿡼� ���Ǵ� ��Ʈ
	RECT menuRect[4];	//�κ�
	RECT outRect[9];	//����
	RECT equipRect[5];	//���
	RECT buyRect[5];	//����
	RECT sellRect[3];	//�Ű�

	//������ ��Ʈ
	RECT weaponRect[7];
	RECT armorRect[7];
	RECT itemRect[13];

	int money;			//��
	int playerNum;		//ĳ���� �ѹ�
	int playerCount;	//ĳ���� ī��Ʈ
	int frameX;			//�̹��� ������
	int addAtk, addDef;	//�߰� ���ݷ� �� ����

	char str[128];

	bool isBattle;		//����
	bool isInventory;	//���
	bool isBuyShop;		//����
	bool isSellShop;	//�Ű�

	bool isWeapon;		//���⸦ �����ߴ��� üũ
	bool isArmor;		//������ �����ߴ��� üũ
	bool isSell;		//�������� �Ǹ��ߴ��� üũ

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
