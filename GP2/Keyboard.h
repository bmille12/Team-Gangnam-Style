#pragma once

#define MAX_NO_OF_KEYS 72

class CKeyboard
{
public:
	CKeyboard();
	~CKeyboard();

	bool isKeyDown(int keycode);
	bool isKeyUp(int keycode);

	bool keyPressed(int keycode);
private:
	bool m_Keys[MAX_NO_OF_KEYS];
};