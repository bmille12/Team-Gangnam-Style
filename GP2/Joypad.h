#pragma once

#include <Windows.h>
#include <XInput.h>

class CJoypad
{
public:
	CJoypad(int index);
	~CJoypad();

	void update();
private:
	XINPUT_STATE m_JoypadState;
	int m_iIndex;

	float m_fLeftThumbstickX;
	float m_fLeftThumbstickY;
	float m_fLeftThumbstickXNormalized;
	float m_fLeftThumbstickYNormalized;

	float m_fRightThumbstickX;
	float m_fRightThumbstickY;
	float m_fRightThumbstickXNormalized;
	float m_fRightThumbstickYNormalized;

	float m_fRightTrigger;
	float m_fLeftTrigger;
};