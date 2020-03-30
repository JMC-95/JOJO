#include "stdafx.h"
#include "gameNode.h"

gameNode::gameNode()
{
}

gameNode::~gameNode()
{
}

HRESULT gameNode::init()
{
	_hdc = GetDC(m_hWnd);
	_managerInit = false;

	return S_OK;
}

HRESULT gameNode::init(bool managerInit)
{
	_hdc = GetDC(m_hWnd);
	_managerInit = managerInit;

	if (managerInit)
	{
		KEYMANAGER->init();
		IMAGEMANAGER->init();
		TIMEMANAGER->init();
		SCENEMANAGER->init();
		SOUNDMANAGER->init();
		ANIMATIONMANAGER->init();
		EFFECTMANAGER->init();
		TXTDATA->init();
	}

	return S_OK;
}

void gameNode::release()
{
	if (_managerInit)
	{
		//타이머 해제
		KillTimer(m_hWnd, 1);

		KEYMANAGER->release();
		IMAGEMANAGER->release();
		TIMEMANAGER->release();
		SCENEMANAGER->release();
		SOUNDMANAGER->release();
		ANIMATIONMANAGER->release();
		EFFECTMANAGER->release();

		KEYMANAGER->releaseSingleton();
		IMAGEMANAGER->releaseSingleton();
		TIMEMANAGER->releaseSingleton();
		SCENEMANAGER->releaseSingleton();
		SOUNDMANAGER->releaseSingleton();
		ANIMATIONMANAGER->releaseSingleton();
		EFFECTMANAGER->releaseSingleton();
		RND->releaseSingleton();
		TXTDATA->releaseSingleton();
	}

	ReleaseDC(m_hWnd, _hdc);
}

void gameNode::update()
{
}

void gameNode::render()
{
}

void gameNode::render(HDC hdc)
{
}

LRESULT gameNode::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (SCENEMANAGER->getCurSceneKey() != "MapTool")
	{
		switch (iMessage)
		{
		case WM_MOUSEMOVE:
			m_ptMouse.x = LOWORD(lParam);
			m_ptMouse.y = HIWORD(lParam);
			break;
		case WM_KEYDOWN:
			switch (wParam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}

		return (DefWindowProc(hWnd, iMessage, wParam, lParam));
	}
	else
	{
		return SCENEMANAGER->getCurScene()->MainProc(hWnd, iMessage, wParam, lParam);
	}
}
