//#include "Protocol.h"

#include "Player.h"
#include "GameManager.h"
#include "Item.h"

const int NUM_THREADS = 6;
const int STARTING_X = 4;
const int STARTING_Y = 4;

const int OP_SEND = 0;
const int OP_RECV = 1;
const int OP_MOVE = 2;
const int OP_RESP = 3;
const int OP_DURA = 4;
const int OP_EDURA = 5;
const int OP_TIME = 6;
const int OP_END = 7;

extern HANDLE hIOCP;

const int EVENT_MOVE = 0;
const int EVENT_CHASE = 1;
const int EVENT_ATTACK = 2;
const int EVENT_HEAL = 3;
const int EVENT_RESP = 4;
const int EVENT_DURA = 5;
const int EVENT_EDURA = 6;
const int EVENT_TIME = 7;
const int EVENT_END = 8;

class CServer{
private:
	CServer();
	~CServer();

	int usernum;			// 들어와 있는 접속자 수	
	int readynum;			// Ready된 플레이어 수
	int rank;

	float rotatestarttime, rotateendtime;

	bool readystate;		// timer에서 ready 수행 할때
	bool dummypos;
	bool endsignal;
	DWORD keyvalue;			//이거 백퍼 꼬임


	CGameManager*				m_cGM;
	CPlayer*					m_cplayer;
	CTimer*						m_ctimer;
	CItem*						m_cItem;
	mycomparison*				mycompare;

	priority_queue <event_type, vector<event_type>, mycomparison> timerQ;
	//priority_queue <event_type, vector<event_type>, mycomparison> itemQ;

	event_type topEv;
	
	D3DXVECTOR3 boosterdirection;

	CRITICAL_SECTION cs;
	set<int> itemid;

	//패킷	
	//static SC_Player m_pos;
	static SC_PlayerPos			m_playerpos;
	static SC_PlayerRotate		m_playerrotate;
	static SC_State				m_state;

	static CServer*				m_serverInstance;				// 싱글톤 패턴

public:
	float starttime;

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
	void InitGame(){
		m_cItem->SetItem();
	}

	void PlayerInit(int id);
	void SendPacket(int id, void *packet);
	void ProcessPacket(char *packet, int id);
	void Accept_thread();
	void Worker_thread();
	void Game_State(SC_State);					// 게임 상태 변경해주기
	void Process_Event(event_type nowevent);
	int GetNewClient_ID();
	void Timer_thread();
	void HeartBeat(int id);
	void ItemRegen(int);
	void DurationColl(int);
	void AddTimer(int, int, int);
	void EndDuration(int);
	void CountTime();
	void GameEnd(int id);
	void SendPos(int id);
	void CleanUp();

	static CServer* GetInstance(){
		if (NULL == m_serverInstance)
		{
			m_serverInstance = new CServer;
		}
		return m_serverInstance;
	}

};