#include "Protocol.h"
#include "Player.h"
#include "GameManager.h"

const int NUM_THREADS = 10;
const int STARTING_X = 4;
const int STARTING_Y = 4;

const int OP_SEND = 0;
const int OP_RECV = 1;
const int OP_MOVE = 2;

extern HANDLE hIOCP;

const int EVENT_MOVE = 0;
const int EVENT_CHASE = 1;
const int EVENT_ATTACK = 2;
const int EVENT_HEAL = 3;

struct event_type{
	int id;
	unsigned int time;
	int do_event;
	int target;
};

class CServer{
private:
	CServer();
	~CServer();

	int usernum;			// 들어와 있는 접속자 수	
	
	static CPlayer m_player;
	static CGameManager GM;

	static PLAYER client[8];

	//패킷	
	static SC_Player m_pos;
	static SC_Player m_playerpacket;
	static SC_State gamestate;

	static CServer* m_serverInstance;				// 싱글톤 패턴

public:
	CRITICAL_SECTION cs;					//크리티컬 섹션( 동기화시 사용 예정 )

	void error_display(char *msg, int err_no)
	{
		
		WCHAR *lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, err_no,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf, 0, NULL);
		printf("%s :", msg);
		wprintf(L"에러%s\n", lpMsgBuf);
		LocalFree(lpMsgBuf);
	}
	void NetworkInit()
	{
		WSADATA wsadata;
		WSAStartup(MAKEWORD(2, 2), &wsadata);
	}

	void PlayerInit(int id);
	void SendPacket(int id, void *packet);
	void ProcessPacket(char *packet, int id);
	void Accept_thread();
	void worker_thread();
	void Game_State(SC_State);					// 게임 상태 변경해주기
	void Process_Event(event_type nowevent);
	int GetNewClient_ID();

	static CServer* GetInstance(){
		if (NULL == m_serverInstance)
		{
			m_serverInstance = new CServer;
		}
		return m_serverInstance;
	}

};