#include "Timer.h"

CTimer::CTimer(){
	startmin = 0;
	startsec = 0;
	countsec = 0;				//��� ���ϴ� sec��
	min = 0;
	sec = 0;
}
CTimer::~CTimer(){}

bool CTimer::TimeCount(int sec){					// �ð��� �޾� �� ��ŭ ī��Ʈ
	
	startsec = GetTickCount64() / 1000;

	while (1){
		countsec = (GetTickCount64() / 1000) - startsec;
		
		if (sec == countsec){
			return true;
		}
	}
	return false;
}

void CTimer::TimeLimit(int state){

	//countsec = (GetTickCount64() / 1000) - startsec;

	//sec = countsec % 60;
	//min = countsec / 60;
	//printf("%d : %d", min, sec);
	//system("cls");
}