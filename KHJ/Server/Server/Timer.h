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

	bool TimeCount(int sec);					// �Լ� ���� false /  ���� �� true
	void TimeLimit(int state);					// ����Ʈ �ð��� ���� (���� 1�� 10�� ����Ʈ)

};