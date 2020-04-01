#pragma once
#include "gameNode.h"

class LobbyScene : public gameNode
{
private:
	//�κ񿡼� ���Ǵ� ��Ʈ�� ��Ƶδ� ����
	vector<RECT> vMenu;				//�κ�
	vector<RECT> vOut;				//����
	vector<RECT> vEquipment;		//���
	vector<RECT> vBuy;				//����
	vector<RECT> vSell;				//�Ű�

	//������ ����
	vector<ItemInfo> vWeapon;		//����
	vector<ItemInfo> vArmor;		//����
	vector<ItemInfo> vPotion;		//����
	vector<ItemInfo> vAllItem;		//��� ������
	vector<ItemInfo> vSellPotion;	//�Ǹſ빰��

	//ĳ���� ���â ����
	vector<ItemInfo> vEquipJj;	//����
	vector<ItemInfo> vEquipHd;	//���ĵ�
	vector<ItemInfo> vEquipHy;	//���Ŀ�
	vector<ItemInfo> vEquipJi;	//����
	vector<ItemInfo> vEquipAj;	//����
	vector<ItemInfo> vEquipIj;	//����
	vector<ItemInfo> vEquipJh;	//��ȫ

private:
	//�κ񿡼� ���Ǵ� ��Ʈ
	RECT menuRect[4];	//�κ�
	RECT outRect[9];	//����
	RECT equipRect[5];	//���
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

	bool isEquip;		//��� �����ߴ��� üũ
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
	void equip();
	void equipRelease();
	void sellItem();
};
