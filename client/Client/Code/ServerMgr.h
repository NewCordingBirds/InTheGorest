#ifndef ServerMgr_h__
#define ServerMgr_h__

#include "InfoSubject.h"
#include "PlayerObserver.h"


class CServer{
private:
	//static CServer* m_serverinstance;
	DECLARE_SINGLETON(CServer);

public:
	void err_quit(char *msg);
	void err_display(char *msg);
	void ProcessPacket(char *ptr);
	void ReadPacket(SOCKET sock);
	void InitServer(HWND, string _srtIP);					//connect()
	void Update_Send_Key();			// 패킷에 키 값 넣어주는 거
	void Update_Send_Rotate();
	void Update_Send_CurMapLength();
	void RecvData();					// 받은 패킷 뿌려주기

	SOCKET Mysock;
	WSABUF sendWSA;
	WSABUF recvWSA;

	char send_buf[MAX_BUFFER_SIZE];
	char recv_buf[MAX_BUFFER_SIZE];
	char packet_buf[MAX_BUFFER_SIZE];

	DWORD packet_size;
	int savePacketsize;
	int myID;       
	int is_exist[ROOM_MAXUSER];

public :
	char*	GetSendBuf()		{ return send_buf; }
	void	SendServerData(char* _pData);

	CRITICAL_SECTION cs;
	
private : // 어차피 여기에 다 담아져 있는데 굳이 옵저버를 써야할까?
	CPlayerObserver*		m_pPlayerObserver;

	SC_InitPlayer			m_tInitPlayer;
	SC_PlayerPos			m_tPlayerPos;
	SC_AvoidPlayer			m_tPlayeravoid;
	SC_Item					m_tItemExist;
	SC_Time					m_tTime;
	//SC_Avoidammo			m_tAAmmo;

	SC_InitPlayer			m_tInitEnemy[ROOM_MAXUSER];
	SC_PlayerPos			m_tEnemyPos[ROOM_MAXUSER];
	SC_PlayerRotate			m_tEnemyRotate[ROOM_MAXUSER];
	SC_AvoidPlayer			m_tEnemyAvoid[ROOM_MAXUSER];
	SC_RemovePlayer			m_tEnemyRemove[ROOM_MAXUSER];
	SC_ServerAni			m_tEnemyAnimation[ROOM_MAXUSER];
	SC_Endboost				m_tEnemyEndBoost[ROOM_MAXUSER];
	SC_Shoot				m_tEnemyBullet[ROOM_MAXUSER];
	
	/*
	#define STANDBY					0
	#define READY					1
	#define PLAYING					2
	#define END						3
	*/
	SC_State				m_tState;


public :
	SC_InitPlayer	GetInitPlayer()		{ return m_tInitPlayer; }
	SC_PlayerPos	GetPlayerPos()		{ return m_tPlayerPos; }
	SC_AvoidPlayer	GetPlayerAvoid()	{ return m_tPlayeravoid; }
	SC_Time			GetTime()			{ return m_tTime; }

	SC_InitPlayer	GetInitEnemy(UINT _nIdx)			{ return m_tInitEnemy[_nIdx]; }
	SC_PlayerPos	GetEnemyPos(UINT _nIdx)				{ return m_tEnemyPos[_nIdx]; }
	SC_PlayerRotate	GetEnemyRotate(UINT _nIdx)			{ return m_tEnemyRotate[_nIdx]; }
	SC_AvoidPlayer	GetEnemyAvoid(UINT _nIdx)			{ return m_tEnemyAvoid[_nIdx]; }
	SC_RemovePlayer	GetEnemyRemovePlayer(UINT _nIdx)	{ return m_tEnemyRemove[_nIdx]; }
	SC_ServerAni	GetEnemyAnimation(UINT _nIdx)		{ return m_tEnemyAnimation[_nIdx]; }
	SC_Endboost		GetEnemyEndBoost(UINT _nIdx)		{ return m_tEnemyEndBoost[_nIdx]; }
	SC_Shoot		GetShoot(UINT _nIdx)				{ return m_tEnemyBullet[_nIdx]; }
	SC_State		GetState()							{ return m_tState; }
	SC_Item			GetItemExist()						{ return m_tItemExist; }

	void			ClearShoot(UINT _nIdx)				{ ZeroMemory(&m_tEnemyBullet[_nIdx], sizeof(SC_Shoot)); m_tEnemyBullet[_nIdx].ID = -1; }


public:
	CServer();
	~CServer();

};
#endif // ServerMgr_h__