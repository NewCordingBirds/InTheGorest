#include "Timer.h"

CTimer::CTimer(){
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

}