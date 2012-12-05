#include "Input.h"
#include "Keyboard.h"
#include "Mouse.h"

CInput::CInput()
{
	
}

CInput::~CInput()
{
	if (m_pKeyboard)
	{
		delete m_pKeyboard;
		m_pKeyboard=0;
	}
	if (m_pMouse)
	{
		delete m_pMouse;
		m_pMouse=0;
	}
	if(m_pJoypads)
	{
		for(int i=0;i<MAX_NO_JOYPADS;i++)
		{
			if(m_pJoypads[i])
			{
				delete m_pJoypads[i];
			}

			delete [] m_pJoypads;
		}
	}
}

void CInput::init()
{
	m_pKeyboard=new CKeyboard();
	m_pMouse=new CMouse();
}