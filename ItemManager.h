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
	ItemKind	itemKind;	//아이템 종류
	image*		itemImage;	//아이템 이미지
	const char* name;		//아이템 이름
	const char* attribute;	//아이템 속성
	int			level;		//아이템 레벨
	int			exp;		//아이템 경험치
	int			power;		//아이템 효과
	int			price;		//아이템 가격
	int			stock;		//아이템 재고
	int			count;		//아이템 총 수량
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
