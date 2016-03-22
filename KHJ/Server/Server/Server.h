#include "Protocol.h"
#include "Player.h"
#include "GameManager.h"

const int NUM_THREADS = 6;
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

public:
	static void error_display(char *msg, int err_no)
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
	static void NetworkInit()
	{
		WSADATA wsadata;
		WSAStartup(MAKEWORD(2, 2), &wsadata);
	}

	static void PlayerInit(int id);
	static void SendPacket(int id, void *packet);
	static void ProcessPacket(char *packet, int id);
	static void Accept_thread();
	static void worker_thread();
	static void Game_State(SC_State);					// 게임 상태 변경해주기
	void Process_Event(event_type nowevent);
	static int GetNewClient_ID();

};