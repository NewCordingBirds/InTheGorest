#include "Protocol.h"

LARGE_INTEGER CTimer::m_Cpuclock;
LARGE_INTEGER CTimer::m_CpuFreq;
LARGE_INTEGER CTimer::m_Cpustop;
LARGE_INTEGER CTimer::m_Fixclock;

float CTimer::m_Time;

CTimer::CTimer()
{
	ZeroMemory(&m_Cpuclock, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_Cpustop, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CpuFreq, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_Fixclock, sizeof(LARGE_INTEGER));
}
CTimer::~CTimer(){}

float CTimer::FramePerSec(){
	//QueryPerformanceFrequency(&m_CpuFreq);

	QueryPerformanceCounter(&m_Cpuclock);
	if (m_Cpuclock.QuadPart - m_Cpustop.QuadPart > m_CpuFreq.QuadPart){
		QueryPerformanceFrequency(&m_CpuFreq);
		m_Cpustop.QuadPart = m_Cpuclock.QuadPart;
	}
	m_Time = (float)(m_Cpuclock.QuadPart - m_Fixclock.QuadPart) / (float)m_CpuFreq.QuadPart;
	
	m_Fixclock = m_Cpuclock;

	return m_Time;
}
bool CTimer::TimeCount(float sec){	
	//return false;
	QueryPerformanceFrequency(&m_CpuFreq);

	QueryPerformanceCounter(&m_Cpuclock);
	while (1){
		QueryPerformanceCounter(&m_Cpustop);
		if (sec <= (m_Cpustop.QuadPart - m_Cpuclock.QuadPart) / m_CpuFreq.QuadPart)
			return true;
	}
	//return false;
}
float CTimer::SetTime(){
	QueryPerformanceCounter(&m_Cpuclock);
	return m_Cpuclock.QuadPart;
}
float CTimer::GetTime(float start){
	QueryPerformanceCounter(&m_Cpustop);
	return  ((m_Cpustop.QuadPart - start) / m_CpuFreq.QuadPart);
}
