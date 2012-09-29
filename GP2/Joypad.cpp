#include "Joypad.h"

CJoypad::CJoypad(int index)
{
	m_iIndex=index;
	ZeroMemory( &m_JoypadState, sizeof(XINPUT_STATE) );
}

CJoypad::~CJoypad()
{
}

void CJoypad::update()
{
	ZeroMemory( &m_JoypadState, sizeof(XINPUT_STATE) );
    DWORD dwResult= XInputGetState( m_iIndex, &m_JoypadState );
	if( dwResult == ERROR_SUCCESS )
	{ 
		//controller connected
		m_fLeftThumbstickX=m_JoypadState.Gamepad.sThumbLX;
		m_fLeftThumbstickY=m_JoypadState.Gamepad.sThumbLY;

		m_fRightThumbstickX=m_JoypadState.Gamepad.sThumbRX;
		m_fRightThumbstickY=m_JoypadState.Gamepad.sThumbRY;
	}else{
		// Controller is not connected 
	}
}