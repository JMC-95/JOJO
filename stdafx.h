// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.

#pragma once
#include "targetver.h"

// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// openFileName
#include <commdlg.h>

// STL
#include <vector>
#include <map>
#include <string>
#include <stack>

// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#define WIN32_LEAN_AND_MEAN

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

//======================================
//##내가 만든 헤더파일은 이곳에 추가하자.##
//======================================

#include "commonMacroFunction.h"
#include "keyManager.h"
#include "imageManager.h"
#include "timeManager.h"
#include "sceneManager.h"
#include "soundManager.h"
#include "animationManager.h"
#include "effectManager.h"
#include "randomFunction.h"
#include "utils.h"
#include "txtData.h"
#include "progressBar.h"

#include "PlayerManager.h"
#include "EnemyManager.h"
#include "FriendlyManager.h"
#include "collisionManager.h"

using namespace UTIL;

//======================================
//##			매니저들			  ##
//======================================

#define KEYMANAGER			keyManager::getSingleton()
#define IMAGEMANAGER		imageManager::getSingleton()
#define TIMEMANAGER			timeManager::getSingleton()
#define SCENEMANAGER		sceneManager::getSingleton()
#define SOUNDMANAGER		soundManager::getSingleton()
#define ANIMATIONMANAGER	animationManager::getSingleton()
#define EFFECTMANAGER		effectManager::getSingleton()
#define RND					randomFunction::getSingleton()
#define TXTDATA				txtData::getSingleton()
#define SUBWIN				SubWindow::getSingleton()

#define PLAYERMANAGER		PlayerManager::getSingleton()
#define ENEMYMANAGER		EnemyManager::getSingleton()
#define FRIENDMANAGER		FriendlyManager::getSingleton()
#define COLLISIONMANAGER	CollisionManager::getSingleton()

//======================================
//##			디파인문				  ##
//======================================

#define WINNAME		(LPTSTR)(TEXT("삼국지 조조전"))
#define WINSTARTX	100
#define WINSTARTY	20
#define WINSIZEX	1200
#define WINSIZEY	960
#define WINSTYLE	WS_CAPTION | WS_SYSMENU

//======================================
//## 메인게임 릴리즈에서 사용하는 녀석	##
//======================================

#define SAFE_DELETE(p) {if(p) {delete(p); (p) = nullptr;}}

//================================================
//## 전역변수(인스턴스, 윈도우 핸들, 마우스 좌표)	##
//================================================

extern HINSTANCE m_hInstance;
extern HWND m_hWnd;
extern POINT m_ptMouse;
extern bool isOfnCheck;
