#pragma once

#define MAX_NO_JOYPADS 4

class CKeyboard;
class CMouse;
class CJoypad;

class CInput
{
public:
	CInput();
	~CInput();

	CKeyboard * getKeyboard()
	{
		return m_pKeyboard;
	};

	CMouse * getMouse()
	{
		return m_pMouse;
	};

	CJoypad * getJoypad(int playerIndex)
	{
		return m_pJoypads[playerIndex];
	};
private:
	CKeyboard * m_pKeyboard;
	CMouse * m_pMouse;
	CJoypad **m_pJoypads;
};