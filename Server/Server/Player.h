#include "Protocol.h"


class CPlayer{
private:
	int m_HP;
	int m_state;
	float m_speed;
	float m_Accel;
	int m_order;
	float x, y, z;
	float m_AccelTime;
	float m_time;
	float m_frametime;
	float bouncespeed;
	int usernum;
	D3DXVECTOR3 Pl;
	//static float timer[MAXUSER];
	CTimer timer[MAXUSER];

public:
	 
	CPlayer();
	~CPlayer();

	map <int, PLAYER> client;
	 
	enum state{
		normal = 1,
		dizzy = 2,
		booster = 3,
	};
	enum triger{
		noneTrigger,
		ForwordTrigger,
		FDecelTrigger,
		BackwordTrigger,
		BDecelTrigger,
		boosterTrigger, 
		LAvoidTrigger,
		RAvoidTrigger,
		FcolTrigger,
		BcolTrigger,
	};

	int m_room;		// 방 번호

	bool avoid;
	int         m_iPTrigger; 

	SC_InitPlayer PlayerAccept(int);
	D3DXVECTOR3 PlayerPos(DWORD, int);				// 플레이어 위치
	SC_RemovePlayer RemovePlayer();								// 플레이어 지워주기
	void BasicAccel(int, float, int);
	void BasicDecel(int, float, int);
	SC_AvoidPlayer PlayerAvoid(int, DWORD);
	D3DXVECTOR3 UseBooster(int);									// 부스터 사용
	D3DXVECTOR3 BoosterColl(int, D3DXVECTOR3);					// 위치 변경, 클라에서 오는 회전값 받아서 처리
	void CollWall(int);									// 벽 충돌시 튕기기
	D3DXVECTOR3 temporary(DWORD, SC_PlayerPos, int);

	//static void EnterCS(){
	//	EnterCriticalSection(&cs);
	//}
	//static void LeaveCS(){
	//	LeaveCriticalSection(&cs);
	//}
	//static CPlayer* m_getInstance;
	
	static CPlayer& getInstance() {
		static CPlayer uniqueInstance;
		return uniqueInstance;
	}
};