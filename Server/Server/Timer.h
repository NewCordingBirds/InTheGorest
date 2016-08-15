

class CTimer{
public:
	CTimer();
	~CTimer();

public:
	static LARGE_INTEGER		m_Cpuclock;
	static LARGE_INTEGER		m_Cpustop;
	static LARGE_INTEGER		m_CpuFreq;
	static LARGE_INTEGER		m_Fixclock;

	//static LARGE_INTEGER		m_Start;
	//static LARGE_INTEGER		m_TimerFrq;

public:	
	static bool TimeCount(float);					// �Լ� ���� false /  ���� �� true
	static float FramePerSec();
	static float GetTime(float);
	static float SetTime();

	static float m_Time;
};