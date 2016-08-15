#include "Protocol.h"

//using namespace std;

class CGameManager{

public:
	CGameManager();
	~CGameManager();
	
	SC_State InitGameManager();
	BYTE GameState(BYTE, int);		//게임 스타트 관리
	SC_State GameReady();
	SC_State GameEnd();

	bool end_signal;

	static CGameManager& getInstance() {
		static CGameManager uniqueInstance;
		return uniqueInstance;
	}

};