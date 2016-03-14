#pragma once

#include "Protocol.h"

using namespace std;

//template <class T>
class CTimer{

public:	
	CTimer();
	~CTimer();

	DWORD startsec;
	int startmin;
	int countsec;
	int min;
	int sec;

	int m_time;

	bool TimeCount(int sec);					// 함수 들어가면 false /  나올 때 true
	void TimeLimit(int state);					// 리미트 시간을 받음 (게임 1판 10분 리미트)

};