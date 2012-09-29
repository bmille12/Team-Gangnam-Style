#include "Mouse.h"

#include <Windows.h>

CMouse::CMouse()
{
	m_fMouseAbsX=0.0f;
	m_fMouseAbsY=0.0f;
	m_fMouseDeltaX=0.0f;
	m_fMouseDeltaY=0.0f;
	ZeroMemory(&m_bMouseButtons,sizeof(m_bMouseButtons));
}

CMouse::~CMouse()
{
}