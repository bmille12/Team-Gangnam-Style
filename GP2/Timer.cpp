#include "Timer.h"

CTimer::CTimer()
{
	m_LastTime=0;
	m_CurrentTime=0;
	m_bUseHighPrecision=false;
	m_fElapsedTime=0.0f;
}

CTimer::~CTimer()
{

}

void CTimer::start()
{
	if (QueryPerformanceCounter((LARGE_INTEGER*)&m_LastTime))
	{
		m_bUseHighPrecision=true;
		QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency);
	}
	else
	{
		//use get time
	}
}

void CTimer::update()
{
	if (m_bUseHighPrecision)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&m_CurrentTime);
		m_fElapsedTime=(float)((m_CurrentTime-m_LastTime))*1.0f/(float)m_Frequency;
		m_LastTime=m_CurrentTime;
	}
	else
	{

	}
}

float CTimer::getElapsedTime()
{
	return m_fElapsedTime;
}