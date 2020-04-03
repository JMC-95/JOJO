#include "stdafx.h"
#include "mainGame.h"
#include "TitleScene.h"
#include "LobbyScene.h"
#include "MapTool.h"
#include "PreferencesScene.h"
#include "GameScene.h"
#include "GameOver.h"

mainGame::mainGame()
{
}

mainGame::~mainGame()
{
}

HRESULT mainGame::init()
{
	gameNode::init(true);

	//게임에서 쓰일 사운드
	sounds();

	//게임에서 쓰일 이미지
	images();

	//씬 추가
	SCENEMANAGER->addScene("TitleScene", new TitleScene);
	SCENEMANAGER->addScene("LobbyScene", new LobbyScene);
	SCENEMANAGER->addScene("MapTool", new MapTool);
	SCENEMANAGER->addScene("Preferences", new PreferencesScene);
	SCENEMANAGER->addScene("GameScene", new GameScene);
	SCENEMANAGER->addScene("GameOver", new GameOver);

	//현재 씬
	SCENEMANAGER->changeScene("GameScene");

	return S_OK;
}

void mainGame::release()
{
	gameNode::release();
}

void mainGame::update()
{
	gameNode::update();

	SCENEMANAGER->update();

	if (KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		auto& enemyVector = ENEMYMANAGER->getEnemy();

		//enemyVector.erase(enemyVector.begin() + 9, enemyVector.end() - 1);	//여포만 남기고 삭제
		enemyVector.clear();													//전체 삭제
	}
}

void mainGame::render()
{
	//흰색 비트맵
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//========================================================

	SCENEMANAGER->render();

	TIMEMANAGER->render(getMemDC());

	IMAGEMANAGER->render("cursor", getMemDC(), m_ptMouse.x, m_ptMouse.y);

	//========================================================
	//백버퍼의 내용을 HDC에 그린다.(건드리지말것.)
	this->getBackBuffer()->render(getHDC(), 0, 0);
}

void mainGame::sounds()
{
	//BGM
	SOUNDMANAGER->addSound("titleSound", "sound/bgm/TitleSound.mp3", true, true);
	SOUNDMANAGER->addSound("lobbySound", "sound/bgm/LobbySound.mp3", true, true);
	SOUNDMANAGER->addSound("mapToolSound", "sound/bgm/MapToolSound.mp3", true, true);
	SOUNDMANAGER->addSound("gameSound", "sound/bgm/GameSound.mp3", true, true);
	SOUNDMANAGER->addSound("deathSound", "sound/bgm/DeathSound.mp3", true, true);
	SOUNDMANAGER->addSound("endingSound", "sound/bgm/EndingSound.mp3", true, true);

	//Effect
	SOUNDMANAGER->addSound("click", "sound/effect/Click.wav", false, false);
	SOUNDMANAGER->addSound("clickMiss", "sound/effect/ClickMiss.wav", false, false);
	SOUNDMANAGER->addSound("move", "sound/effect/Move.wav", false, false);
	SOUNDMANAGER->addSound("cMove", "sound/effect/CavalryMove.wav", false, false);
	SOUNDMANAGER->addSound("hit", "sound/effect/Hit.wav", false, false);
	SOUNDMANAGER->addSound("aHit", "sound/effect/ArcherHit.wav", false, false);
	SOUNDMANAGER->addSound("death", "sound/effect/Death.wav", false, false);
	SOUNDMANAGER->addSound("pSelect", "sound/effect/Select.wav", false, false);
	SOUNDMANAGER->addSound("start", "sound/effect/Start.wav", false, false);
	SOUNDMANAGER->addSound("noStart", "sound/effect/NoStart.wav", false, false);
	SOUNDMANAGER->addSound("buy", "sound/effect/Buy.wav", false, false);
	SOUNDMANAGER->addSound("sell", "sound/effect/Sell.wav", false, false);
	SOUNDMANAGER->addSound("skillStart", "sound/effect/SkillStart.wav", false, false);
	SOUNDMANAGER->addSound("healStart", "sound/effect/HealStart.wav", false, false);
}

void mainGame::images()
{
	//씬 이미지
	IMAGEMANAGER->addImage("titleScene", "images/UI/Scene/TitleScene.bmp", 642, 462, true, RGB(255, 0, 255));			//타이틀 씬
	IMAGEMANAGER->addImage("lobbyScene", "images/UI/Scene/LobbyScene.bmp", 642, 462, true, RGB(255, 0, 255));			//로비 씬
	IMAGEMANAGER->addImage("preferences", "images/UI/Scene/PreferencesScene.bmp", 642, 462, true, RGB(255, 0, 255));	//환경설정 씬
	IMAGEMANAGER->addImage("check", "images/UI/Scene/check.bmp", 9, 9, true, RGB(255, 0, 255));

	//로비에서 사용할 이미지
	//선택
	IMAGEMANAGER->addImage("equipSelect", "images/UI/Lobby/EquipSelect.bmp", 325, 20, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("buySelect", "images/UI/Lobby/BuySelect.bmp", 348, 20, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("sellSelect", "images/UI/Lobby/SellSelect.bmp", 568, 20, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("sellSelect2", "images/UI/Lobby/SellSelect2.bmp", 545, 20, true, RGB(247, 0, 255));
	//출진
	IMAGEMANAGER->addImage("outgoing", "images/UI/Lobby/출진.bmp", 642, 462, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("o조조", "images/UI/Lobby/조조.bmp", 193, 427, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("n조조", "images/UI/Lobby/조조name.bmp", 45, 13, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("o하후돈", "images/UI/Lobby/하후돈.bmp", 193, 427, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("n하후돈", "images/UI/Lobby/하후돈name.bmp", 45, 13, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("o하후연", "images/UI/Lobby/하후연.bmp", 193, 427, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("n하후연", "images/UI/Lobby/하후연name.bmp", 45, 13, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("o조인", "images/UI/Lobby/조인.bmp", 193, 427, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("n조인", "images/UI/Lobby/조인name.bmp", 45, 13, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("o악진", "images/UI/Lobby/악진.bmp", 193, 427, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("n악진", "images/UI/Lobby/악진name.bmp", 45, 13, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("o이전", "images/UI/Lobby/이전.bmp", 193, 427, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("n이전", "images/UI/Lobby/이전name.bmp", 45, 13, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("o조홍", "images/UI/Lobby/조홍.bmp", 193, 427, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("n조홍", "images/UI/Lobby/조홍name.bmp", 45, 13, true, RGB(255, 0, 255));
	//장비
	IMAGEMANAGER->addImage("equipment", "images/UI/Lobby/장비.bmp", 642, 462, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("empty", "images/UI/Lobby/없음.bmp", 30, 18, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("erase", "images/UI/Lobby/삭제.bmp", 22, 13, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("검info", "images/UI/Item/검Info.bmp", 390, 280, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("창info", "images/UI/Item/창Info.bmp", 390, 280, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("활info", "images/UI/Item/활Info.bmp", 390, 280, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("갑옷info", "images/UI/Item/갑옷Info.bmp", 390, 280, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("콩info", "images/UI/Item/콩Info.bmp", 390, 280, true, RGB(255, 0, 255));
	//구입
	IMAGEMANAGER->addFrameImage("buy", "images/UI/Lobby/구입.bmp", 1284, 462, 2, 1, true, RGB(255, 0, 255));
	//매각
	IMAGEMANAGER->addFrameImage("sell", "images/UI/Lobby/매각.bmp", 1284, 462, 2, 1, true, RGB(255, 0, 255));

	//맵툴용 이미지
	IMAGEMANAGER->addImage("map", "images/MapTool/Map.bmp", 960, 960, true, RGB(255, 0, 255));					//실제 맵의 이미지
	IMAGEMANAGER->addImage("miniMap", "images/MapTool/Map.bmp", 960 / 6, 960 / 6, true, RGB(255, 0, 255));		//미니 맵의 이미지
	IMAGEMANAGER->addImage("subMap", "images/MapTool/SubMap.bmp", 240, 960, true, RGB(255, 0, 255));			//맵툴 이미지
	IMAGEMANAGER->addImage("select", "images/MapTool/Select.bmp", 30, 30, true, RGB(255, 0, 255));				//맵툴 선택 이미지

	//맵툴 타일 이미지
	IMAGEMANAGER->addFrameImage("tileMap", "images/MapTool/TileMap.bmp", 144, 480, SAMPLE_TILE_X, SAMPLE_TILE_Y, true, RGB(255, 0, 255));

	//맵 속성 이미지
	IMAGEMANAGER->addImage("배경", "images/UI/Attribute/배경.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("바위산", "images/UI/Attribute/바위산.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("병영", "images/UI/Attribute/병영.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("산지", "images/UI/Attribute/산지.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("성내", "images/UI/Attribute/성내.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("성문", "images/UI/Attribute/성문.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("성벽", "images/UI/Attribute/성벽.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("평지", "images/UI/Attribute/평지.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("황무지", "images/UI/Attribute/황무지.bmp", 144, 96, true, RGB(247, 0, 255));

	//인게임 이미지
	IMAGEMANAGER->addImage("subWin", "images/UI/InGame/SubWindow.bmp", 240, 960, true, RGB(247, 0, 255));		//서브 윈도우 이미지
	IMAGEMANAGER->addImage("tileCheck", "images/UI/InGame/TileCheck.bmp", 48, 48, true, RGB(247, 0, 255));		//타일 체크 이미지
	IMAGEMANAGER->addImage("pMoveRange", "images/UI/InGame/pMovingRange.bmp", 48, 48, true, RGB(247, 0, 255));	//플레이어 이동범위
	IMAGEMANAGER->addImage("eMoveRange", "images/UI/InGame/eMovingRange.bmp", 48, 48, true, RGB(247, 0, 255));	//플레이어 외 이동범위 (아군, 적)
	IMAGEMANAGER->addImage("attackRange", "images/UI/InGame/AttackRange.bmp", 48, 48, true, RGB(247, 0, 255));	//이동시의 공격범위
	IMAGEMANAGER->addImage("atkRange", "images/UI/InGame/AtkRange.bmp", 48, 48, true, RGB(247, 0, 255));		//실제 공격범위
	IMAGEMANAGER->addImage("cursor", "images/UI/InGame/Cursor.bmp", 26, 26, true, RGB(255, 0, 255));			//마우스 커서
	IMAGEMANAGER->addImage("playerTurn", "images/UI/InGame/PlayerTurn.bmp", 960, 960, true, RGB(247, 0, 255));	//플레이어 턴
	IMAGEMANAGER->addImage("friendTurn", "images/UI/InGame/FriendTurn.bmp", 960, 960, true, RGB(247, 0, 255));	//우군 턴
	IMAGEMANAGER->addImage("enemyTurn", "images/UI/InGame/EnemyTurn.bmp", 960, 960, true, RGB(247, 0, 255));	//적군 턴
	IMAGEMANAGER->addImage("gameClear", "images/UI/InGame/GameClear.bmp", 1200, 960, true, RGB(247, 0, 255));	//승리
	IMAGEMANAGER->addImage("gameOver", "images/UI/InGame/GameOver.bmp", 1200, 960, true, RGB(247, 0, 255));		//패배

	//날씨 이미지
	IMAGEMANAGER->addFrameImage("맑음", "images/UI/Weather/Sunny.bmp", 216, 200, 1, 4, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("흐림", "images/UI/Weather/Blur.bmp", 216, 200, 1, 4, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("비", "images/UI/Weather/Rain.bmp", 216, 200, 1, 4, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("폭우", "images/UI/Weather/HeavyRain.bmp", 216, 200, 1, 4, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("눈", "images/UI/Weather/Snow.bmp", 216, 200, 1, 4, true, RGB(247, 0, 255));

	//정보창 이미지
	IMAGEMANAGER->addImage("정보창", "images/UI/Info/Status.bmp", 240, 305, true, RGB(247, 0, 255));

	//스킬
	IMAGEMANAGER->addImage("skillMenu", "images/UI/Skill/Skill.bmp", 203, 158, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("skillSelect", "images/UI/Skill/SkillSelect.bmp", 189, 18, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("소보급", "images/UI/Skill/소보급.bmp", 99, 50, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("up", "images/UI/Skill/레벨업.bmp", 99, 50, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("heal", "images/UI/Skill/Heal.bmp", 64, 1280, 1, 20, true, RGB(247, 0, 255));

	//플레이어 이미지
	IMAGEMANAGER->addFrameImage("메뉴", "images/UI/InGame/Menu.bmp", 176, 242, 2, 2, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("조조", "images/Player/조조.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("조조ATK", "images/Player/조조ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("조조BH", "images/Player/조조BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("하후돈", "images/Player/하후돈.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("하후돈ATK", "images/Player/하후돈ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("하후돈BH", "images/Player/하후돈BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("하후연", "images/Player/하후연.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("하후연ATK", "images/Player/하후연ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("하후연BH", "images/Player/하후연BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("조인", "images/Player/조인.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("조인ATK", "images/Player/조인ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("조인BH", "images/Player/조인BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("조홍", "images/Player/조홍.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("조홍ATK", "images/Player/조홍ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("조홍BH", "images/Player/조홍BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("악진", "images/Player/악진.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("악진ATK", "images/Player/악진ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("악진BH", "images/Player/악진BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("이전", "images/Player/이전.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("이전ATK", "images/Player/이전ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("이전BH", "images/Player/이전BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));

	//우군 이미지
	IMAGEMANAGER->addFrameImage("유비", "images/Friendly/유비.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("유비ATK", "images/Friendly/유비ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("유비BH", "images/Friendly/유비BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("관우", "images/Friendly/관우.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("관우ATK", "images/Friendly/관우ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("관우BH", "images/Friendly/관우BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("장비", "images/Friendly/장비.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("장비ATK", "images/Friendly/장비ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("장비BH", "images/Friendly/장비BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("원소", "images/Friendly/원소.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("원소ATK", "images/Friendly/원소ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("원소BH", "images/Friendly/원소BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("도겸", "images/Friendly/도겸.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("도겸ATK", "images/Friendly/도겸ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("도겸BH", "images/Friendly/도겸BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("우군보병", "images/Friendly/보병.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("우군보병ATK", "images/Friendly/보병ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("우군보병BH", "images/Friendly/보병BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));

	//적 이미지
	IMAGEMANAGER->addFrameImage("동탁", "images/Enemy/동탁.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("이유", "images/Enemy/이유.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("번주", "images/Enemy/번주.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("장제", "images/Enemy/장제.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("여포", "images/Enemy/여포.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("여포ATK", "images/Enemy/여포ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("여포BH", "images/Enemy/여포BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("적보병", "images/Enemy/보병.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("적보병ATK", "images/Enemy/보병ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("적보병BH", "images/Enemy/보병BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("적궁병", "images/Enemy/궁병.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("적궁병ATK", "images/Enemy/궁병ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("적궁병BH", "images/Enemy/궁병BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("적기병", "images/Enemy/기병.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("적기병ATK", "images/Enemy/기병ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("적기병BH", "images/Enemy/기병BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));

	//얼굴 이미지
	IMAGEMANAGER->addImage("조조Face", "images/UI/Face/조조face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("조인Face", "images/UI/Face/조인face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("조홍Face", "images/UI/Face/조홍face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("악진Face", "images/UI/Face/악진face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("이전Face", "images/UI/Face/이전face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("하후돈Face", "images/UI/Face/하후돈face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("하후연Face", "images/UI/Face/하후연face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("유비Face", "images/UI/Face/유비face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("관우Face", "images/UI/Face/관우face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("장비Face", "images/UI/Face/장비face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("원소Face", "images/UI/Face/원소face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("도겸Face", "images/UI/Face/도겸face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("우군보병Face", "images/UI/Face/우군보병face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("동탁Face", "images/UI/Face/동탁face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("이유Face", "images/UI/Face/이유face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("번주Face", "images/UI/Face/번주face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("장제Face", "images/UI/Face/장제face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("여포Face", "images/UI/Face/여포face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("적보병Face", "images/UI/Face/적보병face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("적궁병Face", "images/UI/Face/적궁병face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("적기병Face", "images/UI/Face/적기병face.bmp", 64, 80, true, RGB(247, 0, 255));
}
