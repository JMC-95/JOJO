#pragma once

class animation
{
private:
	typedef vector<POINT> _vFrameList;
	typedef vector<int> _vPlayList;			//������ �÷��� ���

	_vFrameList _frameList;					//��������ġ���
	_vPlayList _playList;					//�÷��� ���
	DWORD _nowPlayIndex;					//���� �÷��� �ε���

	int _frameNum;							//������ ����
	int _frameWidth;
	int _frameHeight;
	int _start;
	int _end;

	float _elapsedSec;						//������ ����ð�
	float _frameUpdateSec;					//������ ������Ʈ

	bool _loop;
	bool _play;

public:
	animation();
	~animation();

	HRESULT init(int totalW, int totalH, int frameW, int frameH);
	//ó������ ������ �ִϸ��̼� ����
	void setDefPlayFrame(bool reverse = false, bool loop = false);
	//�迭�� ����� �ִ� �ֵ鸸
	void setPlayFrame(int* playArr, int arrLen, bool loop = false);
	//���۰� �������� ���ؼ�
	void setPlayFrame(int start, int end, bool reverse = false, bool loop = false);

	//�ʴ� ���� Ƚ��
	void setFPS(int framePerSec);

	//������ ������Ʈ Ÿ��
	void frameUpdate(float elapsedTime);

	void death();
	void start();
	void stop();
	void pause();
	void resume();

	inline bool isPlay() { return _play; }
	inline POINT getFramePos() { return _frameList[_playList[_nowPlayIndex]]; }
	inline int getFrameWidth() { return _frameWidth; }
	inline int getFrameHeight() { return _frameHeight; }
};
