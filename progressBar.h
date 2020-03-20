#pragma once

class progressBar
{
private:
	image* _progressBarFront;	//ü�¹� ���̹���
	image* _progressBarBack;	//ü�¹� ���̹���
	RECT _rcProgress;			//ü�¹� ��Ʈ

	int _x, _y;
	int _width;					//���α���(��༮���� ������)

public:
	progressBar();
	~progressBar();

	HRESULT init(char* frontImage, char* backImage, float x, float y, int width, int height);
	void release();
	void update();
	void render(HDC hdc);
	//������ ����
	void setGauge(float currentGauge, float maxGauge);

	void setX(int x) { _x = x; }
	void setY(int y) { _y = y; }

	RECT getRect() { return _rcProgress; }
};
