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
	//����
	ItemInfo sword;
	sword.itemKind = ITEM_WEAPON;
	sword.itemImage = IMAGEMANAGER->addImage("sword", "images/UI/Item/�ܰ�.bmp", 32, 32, true, RGB(247, 0, 255));
	sword.name = "�ܰ�";
	sword.attribute = "��";
	sword.level = 1;
	sword.exp = 0;
	sword.power = 10;
	sword.price = 1000;
	sword.stock = 0;
	sword.count = 0;
	vWeapon.push_back(sword);

	ItemInfo spear;
	spear.itemKind = ITEM_WEAPON;
	spear.itemImage = IMAGEMANAGER->addImage("spear", "images/UI/Item/��â.bmp", 32, 32, true, RGB(247, 0, 255));
	spear.name = "��â";
	spear.attribute = "â";
	spear.level = 1;
	spear.exp = 0;
	spear.power = 12;
	spear.price = 1000;
	spear.stock = 0;
	spear.count = 0;
	vWeapon.push_back(spear);

	ItemInfo bow;
	bow.itemKind = ITEM_WEAPON;
	bow.itemImage = IMAGEMANAGER->addImage("bow", "images/UI/Item/�ݱ�.bmp", 32, 32, true, RGB(247, 0, 255));
	bow.name = "�ݱ�";
	bow.attribute = "Ȱ";
	bow.level = 1;
	bow.exp = 0;
	bow.power = 8;
	bow.price = 1000;
	bow.stock = 0;
	bow.count = 0;
	vWeapon.push_back(bow);

	ItemInfo doubleSword;
	doubleSword.itemKind = ITEM_WEAPON;
	doubleSword.itemImage = IMAGEMANAGER->addImage("doubleSword", "images/UI/Item/�ڿ��ϴ��.bmp", 32, 32, true, RGB(247, 0, 255));
	doubleSword.name = "�ڿ��ϴ��";
	doubleSword.attribute = "��";
	doubleSword.level = 1;
	doubleSword.exp = 0;
	doubleSword.power = 30;
	doubleSword.price = 3000;
	vWeapon.push_back(doubleSword);

	ItemInfo tridentHalberd;
	tridentHalberd.itemKind = ITEM_WEAPON;
	tridentHalberd.itemImage = IMAGEMANAGER->addImage("tridentHalberd", "images/UI/Item/��õȭ��.bmp", 32, 32, true, RGB(247, 0, 255));
	tridentHalberd.name = "��õȭ��";
	tridentHalberd.attribute = "â";
	tridentHalberd.level = 1;
	tridentHalberd.exp = 0;
	tridentHalberd.power = 36;
	tridentHalberd.price = 3000;
	vWeapon.push_back(tridentHalberd);

	//��
	ItemInfo leather;
	leather.itemKind = ITEM_ARMOR;
	leather.itemImage = IMAGEMANAGER->addImage("leather", "images/UI/Item/���װ���.bmp", 32, 32, true, RGB(247, 0, 255));
	leather.name = "���װ���";
	leather.attribute = "����";
	leather.level = 1;
	leather.exp = 0;
	leather.power = 10;
	leather.price = 1000;
	leather.stock = 0;
	leather.count = 0;
	vArmor.push_back(leather);

	ItemInfo silver;
	silver.itemKind = ITEM_ARMOR;
	silver.itemImage = IMAGEMANAGER->addImage("silver", "images/UI/Item/��������.bmp", 32, 32, true, RGB(247, 0, 255));
	silver.name = "��������";
	silver.attribute = "����";
	silver.level = 1;
	silver.exp = 0;
	silver.power = 30;
	silver.price = 3000;
	vArmor.push_back(silver);

	//����
	ItemInfo horse;
	horse.itemKind = ITEM_ASSISTANT;
	horse.itemImage = IMAGEMANAGER->addImage("horse", "images/UI/Item/���丶.bmp", 32, 32, true, RGB(247, 0, 255));
	horse.name = "���丶";
	horse.attribute = "����";
	horse.level = 1;
	horse.exp = 0;
	horse.power = 30;
	horse.price = 3000;
	vAssistant.push_back(horse);

	//����
	ItemInfo potion;
	potion.itemKind = ITEM_ASSISTANT;
	potion.itemImage = IMAGEMANAGER->addImage("potion", "images/UI/Item/ȸ���� ��.bmp", 32, 32, true, RGB(247, 0, 255));
	potion.name = "ȸ���� ��";
	potion.attribute = "HPȸ��";
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