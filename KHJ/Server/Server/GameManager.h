#pragma once

#include "Protocol.h"
//#include "Timer.h"					// 상태 타이머

using namespace std;

class CGameManager{


public:
	CGameManager();
	~CGameManager();

	SC_State GameState(SC_State, int);		//게임 스타트 관리
	// 접속 인원과 

	static CGameManager& getInstance()
	{
		static CGameManager instance;
		return instance;
	}

	int m_room;
};