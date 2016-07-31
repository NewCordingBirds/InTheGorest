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
//	//case READY:{							// �� �ο� ���� �� ���� ī��Ʈ
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
//	//case PLAYING:{							// ��ŸƮ~ ���� ����
//	//	printf("Start");
//	//	break;
//	//}
//	//case END:								// �� �÷��̾�� ���� ���� ���� �� ���� ����
//	//	// 1�� �÷��̾� ����
//	//	//end_signal = m_timer->TimeCount(5.0f);
//	//	if (endsignal){
//	//		// �ڵ����� ���� �ű�� ��
//	//	}
//	//	// �� �� 5�� �÷���
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