#include "stdafx.h"
#include "ItemManager.h"

ItemManager::ItemManager()
{
}

ItemManager::~ItemManager()
{
}

HRESULT ItemManager::init()
{
	//¹«±â
	ItemInfo sword;
	sword.itemKind = ITEM_WEAPON;
	sword.itemImage = IMAGEMANAGER->addImage("sword", "images/UI/Item/´Ü°Ë.bmp", 32, 32, true, RGB(247, 0, 255));
	sword.name = "´Ü°Ë";
	sword.attribute = "°Ë";
	sword.level = 1;
	sword.exp = 0;
	sword.power = 10;
	sword.price = 1000;
	sword.stock = 0;
	sword.count = 0;
	vWeapon.push_back(sword);

	ItemInfo spear;
	spear.itemKind = ITEM_WEAPON;
	spear.itemImage = IMAGEMANAGER->addImage("spear", "images/UI/Item/´ÜÃ¢.bmp", 32, 32, true, RGB(247, 0, 255));
	spear.name = "´ÜÃ¢";
	spear.attribute = "Ã¢";
	spear.level = 1;
	spear.exp = 0;
	spear.power = 12;
	spear.price = 1000;
	spear.stock = 0;
	spear.count = 0;
	vWeapon.push_back(spear);

	ItemInfo bow;
	bow.itemKind = ITEM_WEAPON;
	bow.itemImage = IMAGEMANAGER->addImage("bow", "images/UI/Item/¹Ý±Ã.bmp", 32, 32, true, RGB(247, 0, 255));
	bow.name = "¹Ý±Ã";
	bow.attribute = "È°";
	bow.level = 1;
	bow.exp = 0;
	bow.power = 8;
	bow.price = 1000;
	bow.stock = 0;
	bow.count = 0;
	vWeapon.push_back(bow);

	ItemInfo doubleSword;
	doubleSword.itemKind = ITEM_WEAPON;
	doubleSword.itemImage = IMAGEMANAGER->addImage("doubleSword", "images/UI/Item/ÀÚ¿õÀÏ´ë°Ë.bmp", 32, 32, true, RGB(247, 0, 255));
	doubleSword.name = "ÀÚ¿õÀÏ´ë°Ë";
	doubleSword.attribute = "°Ë";
	doubleSword.level = 1;
	doubleSword.exp = 0;
	doubleSword.power = 30;
	doubleSword.price = 3000;
	vWeapon.push_back(doubleSword);

	ItemInfo tridentHalberd;
	tridentHalberd.itemKind = ITEM_WEAPON;
	tridentHalberd.itemImage = IMAGEMANAGER->addImage("tridentHalberd", "images/UI/Item/¹æÃµÈ­±Ø.bmp", 32, 32, true, RGB(247, 0, 255));
	tridentHalberd.name = "¹æÃµÈ­±Ø";
	tridentHalberd.attribute = "Ã¢";
	tridentHalberd.level = 1;
	tridentHalberd.exp = 0;
	tridentHalberd.power = 36;
	tridentHalberd.price = 3000;
	vWeapon.push_back(tridentHalberd);

	//¹æ¾î±¸
	ItemInfo leather;
	leather.itemKind = ITEM_ARMOR;
	leather.itemImage = IMAGEMANAGER->addImage("leather", "images/UI/Item/°¡Á×°©¿Ê.bmp", 32, 32, true, RGB(247, 0, 255));
	leather.name = "°¡Á×°©¿Ê";
	leather.attribute = "°©¿Ê";
	leather.level = 1;
	leather.exp = 0;
	leather.power = 10;
	leather.price = 1000;
	leather.stock = 0;
	leather.count = 0;
	vArmor.push_back(leather);

	ItemInfo silver;
	silver.itemKind = ITEM_ARMOR;
	silver.itemImage = IMAGEMANAGER->addImage("silver", "images/UI/Item/¹éÀº°©¿Ê.bmp", 32, 32, true, RGB(247, 0, 255));
	silver.name = "¹éÀº°©¿Ê";
	silver.attribute = "°©¿Ê";
	silver.level = 1;
	silver.exp = 0;
	silver.power = 30;
	silver.price = 3000;
	vArmor.push_back(silver);

	//º¸Á¶
	ItemInfo horse;
	horse.itemKind = ITEM_ASSISTANT;
	horse.itemImage = IMAGEMANAGER->addImage("horse", "images/UI/Item/ÀûÅä¸¶.bmp", 32, 32, true, RGB(247, 0, 255));
	horse.name = "ÀûÅä¸¶";
	horse.attribute = "º¸Á¶";
	horse.level = 1;
	horse.exp = 0;
	horse.power = 30;
	horse.price = 3000;
	vAssistant.push_back(horse);

	//Æ÷¼Ç
	ItemInfo potion;
	potion.itemKind = ITEM_ASSISTANT;
	potion.itemImage = IMAGEMANAGER->addImage("potion", "images/UI/Item/È¸º¹ÀÇ Äá.bmp", 32, 32, true, RGB(247, 0, 255));
	potion.name = "È¸º¹ÀÇ Äá";
	potion.attribute = "HPÈ¸º¹";
	potion.power = 25;
	potion.price = 100;
	potion.stock = 0;
	vPotion.push_back(potion);

	vAllItem.push_back(sword);
	vAllItem.push_back(spear);
	vAllItem.push_back(bow);
	vAllItem.push_back(doubleSword);
	vAllItem.push_back(tridentHalberd);
	vAllItem.push_back(leather);
	vAllItem.push_back(silver);
	vAllItem.push_back(horse);
	vAllItem.push_back(potion);

	return S_OK;
}

void ItemManager::release()
{
}

void ItemManager::update()
{
}

void ItemManager::render()
{
}