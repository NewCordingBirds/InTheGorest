#include "Protocol.h"

using namespace std;

//template<class T>

CTimer::CTimer(){}
CTimer::~CTimer(){}

void CTimer :: TimeCount(){
	DWORD startsec = 0;
	int startmin = 0;
	int countsec = 0;				//계속 변하는 sec값
	int min = 0;
	int sec = 0;
	int state;

	CGameManager GM;

	if (1 == GM.state){
		startsec = GetTickCount64() / 1000;

	}
	while (2 == GM.state)
	{
		countsec = (GetTickCount64() /1000 ) - startsec;
		
		sec = countsec % 60;
		min = countsec / 60;
		printf("%d : %d", min, sec);
		system("cls");
	}
}