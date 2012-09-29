#include "Keyboard.h"

CKeyboard::CKeyboard()
{
}

CKeyboard::~CKeyboard()
{
}

bool CKeyboard::isKeyDown(int keycode)
{
	return m_Keys[keycode];
}

bool CKeyboard::isKeyUp(int keycode)
{
	return m_Keys[keycode];
}

bool CKeyboard::keyPressed(int keycode)
{
	return m_Keys[keycode];
}