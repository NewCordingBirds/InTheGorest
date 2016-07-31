
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

	int m_room;		// �� ��ȣ

	bool avoid;
	int         m_iPTrigger; 

	SC_InitPlayer PlayerAccept(int);
	D3DXVECTOR3 PlayerPos(DWORD, int);				// �÷��̾� ��ġ
	SC_RemovePlayer RemovePlayer();								// �÷��̾� �����ֱ�
	void BasicAccel(int, float, int);
	void BasicDecel(int, float, int);
	SC_AvoidPlayer PlayerAvoid(int, DWORD);
	D3DXVECTOR3 UseBooster(int);									// �ν��� ���
	D3DXVECTOR3 BoosterColl(int, D3DXVECTOR3);					// ��ġ ����, Ŭ�󿡼� ���� ȸ���� �޾Ƽ� ó��
	void CollWall(int);									// �� �浹�� ƨ���
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