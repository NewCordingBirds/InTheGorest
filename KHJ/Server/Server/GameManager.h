#pragma once

#include "Protocol.h"
#include "Timer.h"					// 상태 타이머

using namespace std;

class CGameManager{
public :	
	CGameManager();
	~CGameManager();

	int m_room;
	
	CTimer playtimer;						// 초를 세서 게임 상태 변경
	
	SC_State GameState(SC_State, int);		//게임 스타트 관리
	// 접속 인원과 

	static CGameManager& getInstance()
	{
		static CGameManager instance;
		return instance;
	}
};