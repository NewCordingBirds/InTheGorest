#include "GameManager.h"
#include "Timer.h"

CGameManager::CGameManager(){}
CGameManager::~CGameManager(){}

SC_State CGameManager::GameState(SC_State packet, int Pnum){
	CTimer playtimer;						// 초를 세서 게임 상태 변경

	static bool Start = false;

	switch (packet.state){
	case STANDBY:
	{
		printf("Standby");

		if (6 < Pnum)
			packet.state = READY;
		return packet;
	}
	case READY:	{							// 전 인원 접속 시 게임 카운트
		printf("Ready");
		Start = playtimer.TimeCount(3);
		if (Start)				// 카운트 3초
			packet.state = PLAYING;
		return packet;
	}

	case PLAYING:{							// 스타트~ 종료 지점
		printf("Start");
		break;
	}

	case END:								// 한 플레이어라도 종료 지점 도착 시 게임 종료
		// 1등 플레이어 도착
		bool End = playtimer.TimeCount(5);
		if (End){
			// 자동으로 순위 매기고 끝
		}
		// 이 후 5초 플레이
		break;

	}

}