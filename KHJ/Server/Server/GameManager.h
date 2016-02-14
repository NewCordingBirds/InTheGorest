#pragma once

#include "Protocol.h"
#include "Server.h"

using namespace std;

class CGameManager{
	

private :
	CGameManager();
	~CGameManager();

public :
	enum state{
		standby		= 0,
		start		= 1,
		playing		= 2,
		end			= 3,
	};
	int state = 0;
	int m_room;
	
	CServer server;
	
	void GameState(int m_room, int state);		//게임 스타트 관리


public:

	static CGameManager& getInstance()
	{
		static CGameManager instance;
		return instance;
	}
};