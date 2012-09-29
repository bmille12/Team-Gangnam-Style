#include "Input.h"
#include "Keyboard.h"

CInput::CInput()
{
	m_pKeyboard=new CKeyboard();
}

CInput::~CInput()
{
	if (m_pKeyboard)
	{
		delete m_pKeyboard;
		m_pKeyboard=0;
	}
}