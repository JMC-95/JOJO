#pragma once

class Interface
{
private:
	char str[128];

public:
	Interface();
	~Interface();

	void render(HDC hdc);

	void playerMenu(HDC hdc);
	void playerInformation(HDC hdc);
	void friendInformation(HDC hdc);
	void enemyInformation(HDC hdc);
};

