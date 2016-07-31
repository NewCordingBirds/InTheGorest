#include "GameManager.h"

CGameManager::CGameManager()
{
}
CGameManager::~CGameManager(){}

SC_State CGameManager::InitGameManager(){
	SC_State state;
	state.size = sizeof(SC_State);
	state.type = SC_GAMESTATE;
	state.gamestate = STANDBY;
	state.timecount = 4;

	return state;
}

//BYTE CGameManager::GameState(BYTE packet, int Pnum){
//	bool startsignal = false;
//	bool endsignal = false;
//	//switch (packet){
//	//case STANDBY:{
//	//	if (2 <= Pnum){
//	//		packet = READY;
//	//	}
//	//	else{
//	//		printf("Standby");
//	//		return packet;
//	//	}
//	//}
//	//case READY:{							// 전 인원 접속 시 게임 카운트
//	//	
//	//	printf("Ready");
//	//	//printf("%d", start_signal);
//	//	//return packet;
//	//	startsignal = CTimer::TimeCount(3.0f);
//	//	if (startsignal){
//	//		packet = PLAYING;
//	//	}
//	//	else
//	//		return packet;
//	//}
//	//case PLAYING:{							// 스타트~ 종료 지점
//	//	printf("Start");
//	//	break;
//	//}
//	//case END:								// 한 플레이어라도 종료 지점 도착 시 게임 종료
//	//	// 1등 플레이어 도착
//	//	//end_signal = m_timer->TimeCount(5.0f);
//	//	if (endsignal){
//	//		// 자동으로 순위 매기고 끝
//	//	}
//	//	// 이 후 5초 플레이
//	//	break;
//	//}
//	//return packet;
//}

SC_State CGameManager::GameReady(){
	SC_State state;
	state.size = sizeof(SC_State);
	state.type = SC_GAMESTATE;
	state.gamestate = READY;
	return state;
}
SC_State CGameManager::GameEnd(){
	SC_State gamestate;
	gamestate.size = sizeof(SC_GAMESTATE);
	gamestate.type = SC_GAMESTATE;
	gamestate.timecount = 0;
	gamestate.gamestate = END;

	return gamestate;
}