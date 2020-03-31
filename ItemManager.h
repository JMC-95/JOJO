#pragma once
#include "singletonBase.h"

enum ItemKind
{
	ITEM_WEAPON,
	ITEM_ARMOR,
	ITEM_ASSISTANT,
	ITEM_POTION,
	ITEM_END
};

struct ItemInfo
{
	ItemKind	itemKind;	//������ ����
	image*		itemImage;	//������ �̹���
	const char* name;		//������ �̸�
	const char* attribute;	//������ �Ӽ�
	int			level;		//������ ����
	int			exp;		//������ ����ġ
	int			power;		//������ ȿ��
	int			price;		//������ ����
	int			stock;		//������ ���
	int			count;		//������ �� ����
};

class ItemManager : public singletonBase<ItemManager>
{
private:
	vector<ItemInfo> vWeapon;
	vector<ItemInfo> vArmor;
	vector<ItemInfo> vAssistant;
	vector<ItemInfo> vPotion;
	vector<ItemInfo> vAllItem;

public:
	ItemManager();
	~ItemManager();

	HRESULT init();
	void release();
	void update();
	void render();

	vector<ItemInfo> getWeapon() { return vWeapon; }
	vector<ItemInfo> getArmor() { return vArmor; }
	vector<ItemInfo> getAssistant() { return vAssistant; }
	vector<ItemInfo> getPotion() { return vPotion; }
	vector<ItemInfo> getAllItem() { return vAllItem; }
};
