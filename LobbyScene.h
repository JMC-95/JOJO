#pragma once
#include "gameNode.h"

class LobbyScene : public gameNode
{
private:
	//로비에서 사용되는 렉트를 담아두는 벡터
	vector<RECT> vMenu;			//로비
	vector<RECT> vOut;			//출진
	vector<RECT> vEquipment;	//장비
	vector<RECT> vBuy;			//구입
	vector<RECT> vSell;			//매각

	//아이템 벡터
	vector<ItemInfo> vWeapon;	//무기
	vector<ItemInfo> vArmor;	//갑옷
	vector<ItemInfo> vPotion;	//물약
	vector<ItemInfo> vAllItem;	//모든 아이템
	vector<ItemInfo> vEquipItem;//장비창 아이템

private:
	//로비에서 사용되는 렉트
	RECT menuRect[4];	//로비
	RECT outRect[9];	//출진
	RECT equipRect[5];	//장비
	RECT buyRect[5];	//구입
	RECT sellRect[3];	//매각

	//아이템 렉트
	RECT weaponRect[7];
	RECT armorRect[7];
	RECT itemRect[13];

	int money;			//돈
	int playerNum;		//캐릭터 넘버
	int playerCount;	//캐릭터 카운트
	int frameX;			//이미지 프레임
	int addAtk, addDef;	//추가 공격력 및 방어력

	char str[128];

	bool isBattle;		//출진
	bool isInventory;	//장비
	bool isBuyShop;		//구입
	bool isSellShop;	//매각

	bool isWeapon;		//무기를 착용했는지 체크
	bool isArmor;		//갑옷을 착용했는지 체크
	bool isSell;		//아이템을 판매했는지 체크

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
