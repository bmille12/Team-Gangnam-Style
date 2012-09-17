#pragma once

#include <windows.h>

class CTimer
{
public:
	CTimer();
	~CTimer();

	void start();
	void update();
	float getElapsedTime();
private:
	__int64 m_LastTime;
	__int64 m_CurrentTime;
	__int64 m_Frequency;
	float m_fElapsedTime;
	bool m_bUseHighPrecision;
};