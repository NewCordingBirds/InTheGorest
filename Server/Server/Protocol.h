#pragma once
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <Windows.h>
#include <cstdio>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <mutex>
#include <thread>
#include <queue>
#include <D3DX10math.h>

#include "Timer.h"

using namespace std;

#define MAXUSER					1000
#define ROOM_MAXUSER			8
#define SERVER_PORT				9000
#define SERVERIP				"127.0.0.1"

#define MAX_BUFFER_SIZE			4000
#define MAX_PACKET_SIZE			255

#define ITEM_NUM				53

// 이동키값
const DWORD ANY_KEY			= 0x00000000;
const DWORD KEY_UP			= 0x00000001;
const DWORD KEY_DOWN		= 0x00000002;
const DWORD COLLISION		= 0x00000004;

const DWORD UPKEY_UP		= 0x00000010;
const DWORD UPKEY_DOWN		= 0x00000020;

const DWORD AVOIDKEY_LEFT	= 0x00000100;
const DWORD AVOIDKEY_RIGHT	= 0x00000200;

// 충돌
#define BOOSTERCOLL				1
#define BOOSTERUSE				2
#define WALLCOLL				3

enum GAMESTATE{
STANDBY	=0,
READY	=1,
PLAYING	=2,
END		=3,
};


enum CS_PACKET{
	CS_KEY = 1,
	CS_ROTATE = 2,
	CS_ITEMGET = 3,
	CS_SHOT = 4,
	CS_CLIENTANI = 5,
	CS_READY = 6,
	CS_AVOID = 7,
	CS_COLLBOOSTER = 8,
	CS_BOOSTERITEM = 9,
	CS_COLLWALL = 10,
	CS_GOAL = 11,
	CS_DIE = 12,
};

enum SC_PACKET{
	SC_INITPLAYER = 0,
	SC_GAMESTATE = 1,
	SC_PLAYERPOS = 2,
	SC_PLAYERROTATE = 3,
	SC_ITEMEXIST = 4,
	SC_TIME = 5,
	SC_REMOVE_PLAYER = 6,
	SC_SERVERANI = 7,
	SC_AVOIDPLAYER = 8,
	SC_RANKING = 9,
	SC_ENDBOOST = 10,
	SC_SHOT = 11,
	SC_BULLETCOL = 12,
	SC_DIE = 13,
};

using namespace std;

struct OVERAPPED_EX {
	WSAOVERLAPPED overapped;
	int operation_type;
	WSABUF wsabuf;
	char IOCPbuf[MAX_BUFFER_SIZE];
	char PacketBuf[MAX_PACKET_SIZE];
	unsigned int prev_received;
	unsigned int curr_packet_size;
};

struct PLAYER {
	D3DXVECTOR3 position;
	D3DXVECTOR3 direction;
	D3DXVECTOR3 radian;
	SOCKET sock;
	bool isDie;
	bool in_use;
	OVERAPPED_EX my_overapped;
	float speed;
	float avoidspeed;
	float accel;
	float frametime;
	float previousframe;
	float presstime;
	float boostertime;
	bool boostertimming;
	int boostersort;
	int trigger;
	CRITICAL_SECTION cs;
};

struct COLLOBJ{
	D3DXVECTOR3 position;
	D3DXVECTOR3 blocksize;
	int blocksort;					// map0, booster1
};
struct ITEM{
	int id;
	bool in_use;
};
struct BULLET{
	D3DXVECTOR3 pos;
	int player_id;
	int bullet_id;
};
struct event_type{
	int id;
	unsigned int time;
	int do_event;
	int target;
};

class mycomparison
{
	bool reverse;
public:
	mycomparison() {}
	bool operator() (const event_type lhs, const event_type rhs) const
	{
		return (lhs.time > rhs.time);
	}
};

#pragma pack (push, 1)

// 패킷[0] = size
// 패킷[1] = type

struct CS_Ready{
	BYTE size;
	CS_PACKET type;
};
struct CS_Key{						// 키 값 -> 키 조작
	BYTE size;
	CS_PACKET type;						// 1		
	DWORD movetype;					// 1234
};
struct CS_Rotate{					// 플레이어 회전 -> 마우스 조작
	BYTE size;
	CS_PACKET type;						//2
	D3DXVECTOR3 dir;
	D3DXVECTOR3 rad;
};
struct CS_ItemGet{						// 아이템 획득
	BYTE size;
	CS_PACKET type;							//3
	BYTE itemnum;						// 아이템 번호
};
struct CS_Shoot{						// 총 쏘는 키 받음
	BYTE size;
	CS_PACKET type;							// 4;
	int ammonum;
	D3DXVECTOR3 direction;
};
struct CS_ClientAni{
	BYTE size;
	CS_PACKET type;
	BYTE aninum;
};
struct CS_CollWall{						//벽 충돌	->pos로 넘김
	BYTE size;
	CS_PACKET type;
};
struct CS_CollBooster{					// 부스터존 충돌	->pos / rotate 넘김
	BYTE size;
	CS_PACKET type;
	D3DXVECTOR3 direction;
};
struct CS_UseBooster
{
	BYTE size;
	CS_PACKET type;							// 리커스터 사용	->pos로 넘김
};
struct CS_Goal{
	BYTE size;
	CS_PACKET type;
};
struct CS_Die{
	BYTE size;
	CS_PACKET type;
};

//////////////////////////////////////////////
struct  SC_InitPlayer
{
	BYTE size;
	SC_PACKET type;
	int ID;
	D3DXVECTOR3 move;
	D3DXVECTOR3 direction;
	D3DXVECTOR3 radian;
	int playerstate;
	float speed;
	float accel;
};
struct SC_State{		// 게임 전체 상태
	BYTE size;
	SC_PACKET type;						// 1
	GAMESTATE gamestate;					// 0, 1, 2, 3
	BYTE timecount;					// 4, 3, 2, 1, 
};
struct SC_PlayerPos{					// 플레이어 위치
	BYTE size;
	SC_PACKET type;						// 2	
	int ID;
	D3DXVECTOR3 move;
};
struct SC_PlayerRotate{
	BYTE size;
	SC_PACKET type;
	int ID;
	D3DXVECTOR3 dir;
	D3DXVECTOR3 rad;
};
struct SC_Item{						// 아이템 위치
	BYTE size;
	SC_PACKET type;						// 3
	BYTE itemid;
	bool isExist;
};
struct SC_Shoot{
	BYTE size;
	SC_PACKET type;						// 5
	int ID;
	int ammonum;
	D3DXVECTOR3 rotate;
};
struct SC_RemovePlayer{
	BYTE size;
	SC_PACKET type;						//6
	int ID;
};
struct SC_ServerAni{
	BYTE size;
	SC_PACKET type;						//7
	BYTE aninum;
	int ID;
};
struct SC_AvoidPlayer{
	BYTE size;
	SC_PACKET type;
	bool endtriger;
};
struct SC_Time{
	BYTE size;
	SC_PACKET type;
	float time;						//시간 float값
};
struct SC_Ranking{
	BYTE size;
	SC_PACKET type;
	int ID;
	int ranking;
};
struct SC_Endboost{
	BYTE size;
	SC_PACKET type;
	int ID;
};
struct SC_bulletcol{
	BYTE size;
	SC_PACKET type;
	int ID;
};
struct SC_Die{
	BYTE size;
	SC_PACKET type;
	int ID;
};
////////////

struct SC_Room{
	BYTE size;
	BYTE type;
	int num;
	int ID[8];
};
#pragma pack (pop)