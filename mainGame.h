#pragma once
#include "gameNode.h"

class mainGame :  public gameNode
{
public:
	mainGame();
	~mainGame();

	virtual HRESULT init();				//�ʱ�ȭ
	virtual void release();				//����
	virtual void update();				//�����ϴ°�
	virtual void render(/*HDC hdc*/);	//�׸��°�

	void images();
};