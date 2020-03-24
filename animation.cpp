#include "stdafx.h"
#include "animation.h"

animation::animation()
	:_frameNum(0), _frameWidth(0), _frameHeight(0), _loop(false),
	_elapsedSec(0), _nowPlayIndex(0), _play(false)
{
}

animation::~animation()
{
}

HRESULT animation::init(int totalW, int totalH, int frameW, int frameH)
{
	//���� ������ ����
	_frameWidth = frameW;
	int frameWidthNum = totalW / _frameWidth;

	//���� ������ ����
	_frameHeight = frameH;
	int frameHeightNum = totalH / _frameHeight;

	//�� ������ ��
	_frameNum = frameWidthNum * frameHeightNum;

	//������ ��ġ ��� ���� ����.
	_frameList.clear();

	for (int i = 0; i < frameHeightNum; i++)
	{
		for (int j = 0; j < frameWidthNum; j++)
		{
			POINT framePos;
			framePos.x = j * _frameWidth;
			framePos.y = i * _frameHeight;

			//������ ��ġ ��� �߰��ϱ�
			_frameList.push_back(framePos);
		}
	}

	//�⺻ ���������� ��������.
	setDefPlayFrame();

	return S_OK;
}

void animation::setDefPlayFrame(bool reverse, bool loop)
{
	//��������
	_loop = loop;
	//��� �ʱ�ȭ
	_playList.clear();

	if (reverse)
	{
		//������ �� 1�� �ε��� ����
		if (_loop)
		{
			//������
			for (int i = 0; i < _frameNum; i++)
			{
				_playList.push_back(i);
			}
			//������
			for (int i = _frameNum - 1; i > 0; i--)
			{
				_playList.push_back(i);
			}

		}
		//������ �� 0������
		else
		{
			//��
			for (int i = 0; i < _frameNum; i++)
			{
				_playList.push_back(i);
			}
			//��
			for (int i = _frameNum - 1; i >= 0; i--)
			{
				_playList.push_back(i);
			}
		}
	}
	else
	{
		//������
		for (int i = 0; i < _frameNum; i++)
		{
			_playList.push_back(i);
		}
	}

}

void animation::setPlayFrame(int* playArr, int arrLen, bool loop)
{
	_loop = loop;
	_playList.clear();

	for (int i = 0; i < arrLen; i++)
	{
		_playList.push_back(playArr[i]);
	}

}

//���۰� ��
void animation::setPlayFrame(int start, int end, bool reverse, bool loop)
{
	_start = start;
	_end = end;
	_loop = loop;
	_playList.clear();

	//���۰� ���� �������(�����ӱ���) ��� ���� ����
	if (_start == _end)
	{
		_playList.clear();
		stop();
		return;
	}
	//���� �������� �� �����Ӻ��� ũ��
	else if (_start > _end)
	{
		//������ �� 1��������
		if (reverse)
		{
			if (_loop)
			{
				//��
				for (int i = _start; i >= _end; i--)
				{
					_playList.push_back(i);
				}
				//��
				for (int i = _end + 1; i < _start; i++)
				{
					_playList.push_back(i);
				}
			}
			//������ �� 0������
			else
			{
				//��
				for (int i = _start; i >= _end; i--)
				{
					_playList.push_back(i);
				}
				//��
				for (int i = _end + 1; i <= _start; i++)
				{
					_playList.push_back(i);
				}
			}
		}
		else
		{
			for (int i = _start; i <= _end; i--)
			{
				_playList.push_back(i);
			}
		}
	}
	//���������� ���������� �� �����Ӻ��� �۴ٸ�
	else
	{
		if (reverse)
		{
			//������ �� 1��
			if (_loop)
			{
				for (int i = _start; i <= _end; i++)
				{
					_playList.push_back(i);
				}
				for (int i = _end - 1; i > _start; i--)
				{
					_playList.push_back(i);
				}
			}
			//������ �� 0�� ����
			else
			{
				//��
				for (int i = _start; i <= _end; i++)
				{
					_playList.push_back(i);
				}
				//��
				for (int i = _end - 1; i <= _start; i--)
				{
					_playList.push_back(i);
				}
			}
		}
		else
		{
			//��
			for (int i = _start; i <= _end; i++)
			{
				_playList.push_back(i);
			}
		}
	}
}

void animation::setFPS(int framePerSec)
{
	_frameUpdateSec = 1.0f / framePerSec;
}

void animation::frameUpdate(float elapsedTime)
{
	//�÷��� ����?
	if (_play)
	{
		_elapsedSec += elapsedTime;

		//������ ������Ʈ �ð��� �Ǿ��ٸ�
		if (_elapsedSec >= _frameUpdateSec)
		{
			_elapsedSec -= _frameUpdateSec;
			_nowPlayIndex++;

			if (_nowPlayIndex == _playList.size())
			{
				if (_loop)
				{
					_nowPlayIndex = 0;
				}
				else
				{
					//����
					//if (PLAYERMANAGER->getAgjin()->getIsAtk())
					//{
					//	_nowPlayIndex = 0;
					//	_play = false;

					//	PLAYERMANAGER->getAgjin()->setIsTurn(false);
					//	PLAYERMANAGER->getAgjin()->setIsSelect(false);
					//	PLAYERMANAGER->getAgjin()->setIsTarget(false);
					//	PLAYERMANAGER->getAgjin()->setIsAtk(false);
					//	PLAYERMANAGER->getAgjin()->setIsCancel(false);
					//}
					////���ĵ�
					//else if (PLAYERMANAGER->getHahudon()->getIsAtk())
					//{
					//	_nowPlayIndex = 0;
					//	_play = false;

					//	PLAYERMANAGER->getHahudon()->setIsTurn(false);
					//	PLAYERMANAGER->getHahudon()->setIsSelect(false);
					//	PLAYERMANAGER->getHahudon()->setIsTarget(false);
					//	PLAYERMANAGER->getHahudon()->setIsAtk(false);
					//	PLAYERMANAGER->getHahudon()->setIsCancel(false);
					//}
					////���Ŀ�
					//else if (PLAYERMANAGER->getHahuyeon()->getIsAtk())
					//{
					//	_nowPlayIndex = 0;
					//	_play = false;

					//	PLAYERMANAGER->getHahuyeon()->setIsTurn(false);
					//	PLAYERMANAGER->getHahuyeon()->setIsSelect(false);
					//	PLAYERMANAGER->getHahuyeon()->setIsTarget(false);
					//	PLAYERMANAGER->getHahuyeon()->setIsAtk(false);
					//	PLAYERMANAGER->getHahuyeon()->setIsCancel(false);
					//}
					////����
					//else if (PLAYERMANAGER->getIjeon()->getIsAtk())
					//{
					//	_nowPlayIndex = 0;
					//	_play = false;

					//	PLAYERMANAGER->getIjeon()->setIsTurn(false);
					//	PLAYERMANAGER->getIjeon()->setIsSelect(false);
					//	PLAYERMANAGER->getIjeon()->setIsTarget(false);
					//	PLAYERMANAGER->getIjeon()->setIsAtk(false);
					//	PLAYERMANAGER->getIjeon()->setIsCancel(false);
					//}
					////��ȫ
					//else if (PLAYERMANAGER->getJohong()->getIsAtk())
					//{
					//	_nowPlayIndex = 0;
					//	_play = false;

					//	PLAYERMANAGER->getJohong()->setIsTurn(false);
					//	PLAYERMANAGER->getJohong()->setIsSelect(false);
					//	PLAYERMANAGER->getJohong()->setIsTarget(false);
					//	PLAYERMANAGER->getJohong()->setIsAtk(false);
					//	PLAYERMANAGER->getJohong()->setIsCancel(false);
					//}
					////����
					//else if (PLAYERMANAGER->getJoin()->getIsAtk())
					//{
					//	_nowPlayIndex = 0;
					//	_play = false;

					//	PLAYERMANAGER->getJoin()->setIsTurn(false);
					//	PLAYERMANAGER->getJoin()->setIsSelect(false);
					//	PLAYERMANAGER->getJoin()->setIsTarget(false);
					//	PLAYERMANAGER->getJoin()->setIsAtk(false);
					//	PLAYERMANAGER->getJoin()->setIsCancel(false);
					//}
					////����
					//else if (PLAYERMANAGER->getJojo()->getIsAtk())
					//{
					//	_nowPlayIndex = 0;
					//	_play = false;

					//	PLAYERMANAGER->getJojo()->setIsHit(true);
					//	PLAYERMANAGER->getJojo()->setIsTurn(false);
					//	PLAYERMANAGER->getJojo()->setIsSelect(false);
					//	PLAYERMANAGER->getJojo()->setIsTarget(false);
					//	PLAYERMANAGER->getJojo()->setIsAtk(false);
					//	PLAYERMANAGER->getJojo()->setIsCancel(false);
					//}
					////����
					//else if (FRIENDMANAGER->getDogyeom()->getIsAtk())
					//{
					//	_nowPlayIndex = 0;
					//	_play = false;

					//	FRIENDMANAGER->getDogyeom()->setIsTurn(false);
					//	FRIENDMANAGER->getDogyeom()->setIsSelect(false);
					//	FRIENDMANAGER->getDogyeom()->setIsTarget(false);
					//	FRIENDMANAGER->getDogyeom()->setIsAtk(false);
					//	FRIENDMANAGER->getDogyeom()->setIsCancel(false);
					//}
					////����
					//else if (FRIENDMANAGER->getGwanu()->getIsAtk())
					//{
					//	_nowPlayIndex = 0;
					//	_play = false;

					//	FRIENDMANAGER->getGwanu()->setIsTurn(false);
					//	FRIENDMANAGER->getGwanu()->setIsSelect(false);
					//	FRIENDMANAGER->getGwanu()->setIsTarget(false);
					//	FRIENDMANAGER->getGwanu()->setIsAtk(false);
					//	FRIENDMANAGER->getGwanu()->setIsCancel(false);
					//}
					////���
					//else if (FRIENDMANAGER->getJangbi()->getIsAtk())
					//{
					//_nowPlayIndex = 0;
					//_play = false;

					//FRIENDMANAGER->getJangbi()->setIsTurn(false);
					//FRIENDMANAGER->getJangbi()->setIsSelect(false);
					//FRIENDMANAGER->getJangbi()->setIsTarget(false);
					//FRIENDMANAGER->getJangbi()->setIsAtk(false);
					//FRIENDMANAGER->getJangbi()->setIsCancel(false);
					//}
					////����
					//else if (FRIENDMANAGER->getWonso()->getIsAtk())
					//{
					//_nowPlayIndex = 0;
					//_play = false;

					//FRIENDMANAGER->getWonso()->setIsTurn(false);
					//FRIENDMANAGER->getWonso()->setIsSelect(false);
					//FRIENDMANAGER->getWonso()->setIsTarget(false);
					//FRIENDMANAGER->getWonso()->setIsAtk(false);
					//FRIENDMANAGER->getWonso()->setIsCancel(false);
					//}
					////����
					//else if (FRIENDMANAGER->getYubi()->getIsAtk())
					//{
					//_nowPlayIndex = 0;
					//_play = false;

					//FRIENDMANAGER->getYubi()->setIsTurn(false);
					//FRIENDMANAGER->getYubi()->setIsSelect(false);
					//FRIENDMANAGER->getYubi()->setIsTarget(false);
					//FRIENDMANAGER->getYubi()->setIsAtk(false);
					//FRIENDMANAGER->getYubi()->setIsCancel(false);
					//}
					////����
					//else if (ENEMYMANAGER->getYeopo()->getIsAtk())
					//{
					//	_nowPlayIndex = 0;
					//	_play = false;

					//	ENEMYMANAGER->getYeopo()->setIsTurn(false);
					//	ENEMYMANAGER->getYeopo()->setIsSelect(false);
					//	ENEMYMANAGER->getYeopo()->setIsTarget(false);
					//	ENEMYMANAGER->getYeopo()->setIsAtk(false);
					//	ENEMYMANAGER->getYeopo()->setIsCancel(false);
					//}
					//else
					//{
						_nowPlayIndex--;
						_play = false;
					//}
				}
			}
		}
	}
}

void animation::start()
{
	_play = true;
	_nowPlayIndex = 0;
}

void animation::stop()
{
	_play = false;
	_nowPlayIndex = 0;
}

void animation::pause()
{
	_play = false;
}

void animation::resume()
{
	_play = true;
}
