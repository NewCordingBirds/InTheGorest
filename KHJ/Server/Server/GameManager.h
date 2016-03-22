#pragma once

#include "Protocol.h"
//#include "Timer.h"					// 상태 타이머

using namespace std;

static class CGameManager{

protected:
	CGameManager();
	~CGameManager();

public :	
	int m_room;
	
	SC_State GameState(SC_State, int);		//게임 스타트 관리
	// 접속 인원과 

	static CGameManager& getInstance()
	{
		static CGameManager instance;
		return instance;
	}
};