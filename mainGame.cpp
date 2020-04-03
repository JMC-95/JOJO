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

	//���ӿ��� ���� ����
	sounds();

	//���ӿ��� ���� �̹���
	images();

	//�� �߰�
	SCENEMANAGER->addScene("TitleScene", new TitleScene);
	SCENEMANAGER->addScene("LobbyScene", new LobbyScene);
	SCENEMANAGER->addScene("MapTool", new MapTool);
	SCENEMANAGER->addScene("Preferences", new PreferencesScene);
	SCENEMANAGER->addScene("GameScene", new GameScene);
	SCENEMANAGER->addScene("GameOver", new GameOver);

	//���� ��
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

		//enemyVector.erase(enemyVector.begin() + 9, enemyVector.end() - 1);	//������ ����� ����
		enemyVector.clear();													//��ü ����
	}
}

void mainGame::render()
{
	//��� ��Ʈ��
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//========================================================

	SCENEMANAGER->render();

	TIMEMANAGER->render(getMemDC());

	IMAGEMANAGER->render("cursor", getMemDC(), m_ptMouse.x, m_ptMouse.y);

	//========================================================
	//������� ������ HDC�� �׸���.(�ǵ帮������.)
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
	//�� �̹���
	IMAGEMANAGER->addImage("titleScene", "images/UI/Scene/TitleScene.bmp", 642, 462, true, RGB(255, 0, 255));			//Ÿ��Ʋ ��
	IMAGEMANAGER->addImage("lobbyScene", "images/UI/Scene/LobbyScene.bmp", 642, 462, true, RGB(255, 0, 255));			//�κ� ��
	IMAGEMANAGER->addImage("preferences", "images/UI/Scene/PreferencesScene.bmp", 642, 462, true, RGB(255, 0, 255));	//ȯ�漳�� ��
	IMAGEMANAGER->addImage("check", "images/UI/Scene/check.bmp", 9, 9, true, RGB(255, 0, 255));

	//�κ񿡼� ����� �̹���
	//����
	IMAGEMANAGER->addImage("equipSelect", "images/UI/Lobby/EquipSelect.bmp", 325, 20, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("buySelect", "images/UI/Lobby/BuySelect.bmp", 348, 20, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("sellSelect", "images/UI/Lobby/SellSelect.bmp", 568, 20, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("sellSelect2", "images/UI/Lobby/SellSelect2.bmp", 545, 20, true, RGB(247, 0, 255));
	//����
	IMAGEMANAGER->addImage("outgoing", "images/UI/Lobby/����.bmp", 642, 462, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("o����", "images/UI/Lobby/����.bmp", 193, 427, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("n����", "images/UI/Lobby/����name.bmp", 45, 13, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("o���ĵ�", "images/UI/Lobby/���ĵ�.bmp", 193, 427, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("n���ĵ�", "images/UI/Lobby/���ĵ�name.bmp", 45, 13, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("o���Ŀ�", "images/UI/Lobby/���Ŀ�.bmp", 193, 427, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("n���Ŀ�", "images/UI/Lobby/���Ŀ�name.bmp", 45, 13, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("o����", "images/UI/Lobby/����.bmp", 193, 427, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("n����", "images/UI/Lobby/����name.bmp", 45, 13, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("o����", "images/UI/Lobby/����.bmp", 193, 427, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("n����", "images/UI/Lobby/����name.bmp", 45, 13, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("o����", "images/UI/Lobby/����.bmp", 193, 427, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("n����", "images/UI/Lobby/����name.bmp", 45, 13, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("o��ȫ", "images/UI/Lobby/��ȫ.bmp", 193, 427, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("n��ȫ", "images/UI/Lobby/��ȫname.bmp", 45, 13, true, RGB(255, 0, 255));
	//���
	IMAGEMANAGER->addImage("equipment", "images/UI/Lobby/���.bmp", 642, 462, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("empty", "images/UI/Lobby/����.bmp", 30, 18, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("erase", "images/UI/Lobby/����.bmp", 22, 13, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("��info", "images/UI/Item/��Info.bmp", 390, 280, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("âinfo", "images/UI/Item/âInfo.bmp", 390, 280, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Ȱinfo", "images/UI/Item/ȰInfo.bmp", 390, 280, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("����info", "images/UI/Item/����Info.bmp", 390, 280, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("��info", "images/UI/Item/��Info.bmp", 390, 280, true, RGB(255, 0, 255));
	//����
	IMAGEMANAGER->addFrameImage("buy", "images/UI/Lobby/����.bmp", 1284, 462, 2, 1, true, RGB(255, 0, 255));
	//�Ű�
	IMAGEMANAGER->addFrameImage("sell", "images/UI/Lobby/�Ű�.bmp", 1284, 462, 2, 1, true, RGB(255, 0, 255));

	//������ �̹���
	IMAGEMANAGER->addImage("map", "images/MapTool/Map.bmp", 960, 960, true, RGB(255, 0, 255));					//���� ���� �̹���
	IMAGEMANAGER->addImage("miniMap", "images/MapTool/Map.bmp", 960 / 6, 960 / 6, true, RGB(255, 0, 255));		//�̴� ���� �̹���
	IMAGEMANAGER->addImage("subMap", "images/MapTool/SubMap.bmp", 240, 960, true, RGB(255, 0, 255));			//���� �̹���
	IMAGEMANAGER->addImage("select", "images/MapTool/Select.bmp", 30, 30, true, RGB(255, 0, 255));				//���� ���� �̹���

	//���� Ÿ�� �̹���
	IMAGEMANAGER->addFrameImage("tileMap", "images/MapTool/TileMap.bmp", 144, 480, SAMPLE_TILE_X, SAMPLE_TILE_Y, true, RGB(255, 0, 255));

	//�� �Ӽ� �̹���
	IMAGEMANAGER->addImage("���", "images/UI/Attribute/���.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("������", "images/UI/Attribute/������.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����", "images/UI/Attribute/����.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����", "images/UI/Attribute/����.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����", "images/UI/Attribute/����.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����", "images/UI/Attribute/����.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����", "images/UI/Attribute/����.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����", "images/UI/Attribute/����.bmp", 144, 96, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("Ȳ����", "images/UI/Attribute/Ȳ����.bmp", 144, 96, true, RGB(247, 0, 255));

	//�ΰ��� �̹���
	IMAGEMANAGER->addImage("subWin", "images/UI/InGame/SubWindow.bmp", 240, 960, true, RGB(247, 0, 255));		//���� ������ �̹���
	IMAGEMANAGER->addImage("tileCheck", "images/UI/InGame/TileCheck.bmp", 48, 48, true, RGB(247, 0, 255));		//Ÿ�� üũ �̹���
	IMAGEMANAGER->addImage("pMoveRange", "images/UI/InGame/pMovingRange.bmp", 48, 48, true, RGB(247, 0, 255));	//�÷��̾� �̵�����
	IMAGEMANAGER->addImage("eMoveRange", "images/UI/InGame/eMovingRange.bmp", 48, 48, true, RGB(247, 0, 255));	//�÷��̾� �� �̵����� (�Ʊ�, ��)
	IMAGEMANAGER->addImage("attackRange", "images/UI/InGame/AttackRange.bmp", 48, 48, true, RGB(247, 0, 255));	//�̵����� ���ݹ���
	IMAGEMANAGER->addImage("atkRange", "images/UI/InGame/AtkRange.bmp", 48, 48, true, RGB(247, 0, 255));		//���� ���ݹ���
	IMAGEMANAGER->addImage("cursor", "images/UI/InGame/Cursor.bmp", 26, 26, true, RGB(255, 0, 255));			//���콺 Ŀ��
	IMAGEMANAGER->addImage("playerTurn", "images/UI/InGame/PlayerTurn.bmp", 960, 960, true, RGB(247, 0, 255));	//�÷��̾� ��
	IMAGEMANAGER->addImage("friendTurn", "images/UI/InGame/FriendTurn.bmp", 960, 960, true, RGB(247, 0, 255));	//�챺 ��
	IMAGEMANAGER->addImage("enemyTurn", "images/UI/InGame/EnemyTurn.bmp", 960, 960, true, RGB(247, 0, 255));	//���� ��
	IMAGEMANAGER->addImage("gameClear", "images/UI/InGame/GameClear.bmp", 1200, 960, true, RGB(247, 0, 255));	//�¸�
	IMAGEMANAGER->addImage("gameOver", "images/UI/InGame/GameOver.bmp", 1200, 960, true, RGB(247, 0, 255));		//�й�

	//���� �̹���
	IMAGEMANAGER->addFrameImage("����", "images/UI/Weather/Sunny.bmp", 216, 200, 1, 4, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("�帲", "images/UI/Weather/Blur.bmp", 216, 200, 1, 4, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("��", "images/UI/Weather/Rain.bmp", 216, 200, 1, 4, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����", "images/UI/Weather/HeavyRain.bmp", 216, 200, 1, 4, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("��", "images/UI/Weather/Snow.bmp", 216, 200, 1, 4, true, RGB(247, 0, 255));

	//����â �̹���
	IMAGEMANAGER->addImage("����â", "images/UI/Info/Status.bmp", 240, 305, true, RGB(247, 0, 255));

	//��ų
	IMAGEMANAGER->addImage("skillMenu", "images/UI/Skill/Skill.bmp", 203, 158, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("skillSelect", "images/UI/Skill/SkillSelect.bmp", 189, 18, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("�Һ���", "images/UI/Skill/�Һ���.bmp", 99, 50, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("up", "images/UI/Skill/������.bmp", 99, 50, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("heal", "images/UI/Skill/Heal.bmp", 64, 1280, 1, 20, true, RGB(247, 0, 255));

	//�÷��̾� �̹���
	IMAGEMANAGER->addFrameImage("�޴�", "images/UI/InGame/Menu.bmp", 176, 242, 2, 2, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����", "images/Player/����.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����ATK", "images/Player/����ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����BH", "images/Player/����BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("���ĵ�", "images/Player/���ĵ�.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("���ĵ�ATK", "images/Player/���ĵ�ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("���ĵ�BH", "images/Player/���ĵ�BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("���Ŀ�", "images/Player/���Ŀ�.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("���Ŀ�ATK", "images/Player/���Ŀ�ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("���Ŀ�BH", "images/Player/���Ŀ�BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����", "images/Player/����.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����ATK", "images/Player/����ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����BH", "images/Player/����BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("��ȫ", "images/Player/��ȫ.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("��ȫATK", "images/Player/��ȫATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("��ȫBH", "images/Player/��ȫBH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����", "images/Player/����.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����ATK", "images/Player/����ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����BH", "images/Player/����BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����", "images/Player/����.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����ATK", "images/Player/����ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����BH", "images/Player/����BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));

	//�챺 �̹���
	IMAGEMANAGER->addFrameImage("����", "images/Friendly/����.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����ATK", "images/Friendly/����ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����BH", "images/Friendly/����BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����", "images/Friendly/����.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����ATK", "images/Friendly/����ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����BH", "images/Friendly/����BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("���", "images/Friendly/���.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("���ATK", "images/Friendly/���ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("���BH", "images/Friendly/���BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����", "images/Friendly/����.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����ATK", "images/Friendly/����ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����BH", "images/Friendly/����BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����", "images/Friendly/����.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����ATK", "images/Friendly/����ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����BH", "images/Friendly/����BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("�챺����", "images/Friendly/����.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("�챺����ATK", "images/Friendly/����ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("�챺����BH", "images/Friendly/����BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));

	//�� �̹���
	IMAGEMANAGER->addFrameImage("��Ź", "images/Enemy/��Ź.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����", "images/Enemy/����.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����", "images/Enemy/����.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����", "images/Enemy/����.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����", "images/Enemy/����.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����ATK", "images/Enemy/����ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("����BH", "images/Enemy/����BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("������", "images/Enemy/����.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("������ATK", "images/Enemy/����ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("������BH", "images/Enemy/����BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("���ú�", "images/Enemy/�ú�.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("���ú�ATK", "images/Enemy/�ú�ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("���ú�BH", "images/Enemy/�ú�BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("���⺴", "images/Enemy/�⺴.bmp", 48, 672, 1, 14, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("���⺴ATK", "images/Enemy/�⺴ATK.bmp", 64, 1024, 1, 16, true, RGB(247, 0, 255));
	IMAGEMANAGER->addFrameImage("���⺴BH", "images/Enemy/�⺴BH.bmp", 48, 288, 1, 6, true, RGB(247, 0, 255));

	//�� �̹���
	IMAGEMANAGER->addImage("����Face", "images/UI/Face/����face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����Face", "images/UI/Face/����face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("��ȫFace", "images/UI/Face/��ȫface.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����Face", "images/UI/Face/����face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����Face", "images/UI/Face/����face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("���ĵ�Face", "images/UI/Face/���ĵ�face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("���Ŀ�Face", "images/UI/Face/���Ŀ�face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����Face", "images/UI/Face/����face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����Face", "images/UI/Face/����face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("���Face", "images/UI/Face/���face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����Face", "images/UI/Face/����face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����Face", "images/UI/Face/����face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("�챺����Face", "images/UI/Face/�챺����face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("��ŹFace", "images/UI/Face/��Źface.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����Face", "images/UI/Face/����face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����Face", "images/UI/Face/����face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����Face", "images/UI/Face/����face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("����Face", "images/UI/Face/����face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("������Face", "images/UI/Face/������face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("���ú�Face", "images/UI/Face/���ú�face.bmp", 64, 80, true, RGB(247, 0, 255));
	IMAGEMANAGER->addImage("���⺴Face", "images/UI/Face/���⺴face.bmp", 64, 80, true, RGB(247, 0, 255));
}
