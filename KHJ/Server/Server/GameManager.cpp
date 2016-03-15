#include "GameManager.h"
CGameManager::CGameManager(){}
CGameManager::~CGameManager(){}

SC_State CGameManager::GameState(SC_State packet, int Pnum){
	
	static bool Start = false;
	
	switch (packet.state){
	case STANDBY:
	{
		printf("Standby");

		if (1 < Pnum)
			packet.state = READY;
		return packet;
	}
	case READY:	{							// �� �ο� ���� �� ���� ī��Ʈ
		printf("Ready");
		Start = playtimer.TimeCount(3);
		if (Start)				// ī��Ʈ 3��
			packet.state = PLAYING;
		return packet;
	}

	case PLAYING:{							// ��ŸƮ~ ���� ����
		printf("Start");
		break;
	}

	case END:								// �� �÷��̾�� ���� ���� ���� �� ���� ����
		// 1�� �÷��̾� ����
		bool End = playtimer.TimeCount(5);
		if (End){
			// �ڵ����� ���� �ű�� ��
		}
		// �� �� 5�� �÷���
		break;

	}

}