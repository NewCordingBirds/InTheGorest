#pragma once
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <Windows.h>
#include <cstdio>
#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <mutex>
#include <thread>

#define MAXUSER					1000
#define SERVER_PORT				9000

#define MAX_BUFFER_SIZE			4000
#define MAX_PACKET_SIZE			255

#define START_X					10
#define START_Y					10

// 이동키값
#define KEY_UP					1
#define KEY_DOWN				2
#define KEY_RIGHT				3
#define KEY_LEFT				4
//게임 상태
#define STANDBY					0
#define READY					1
#define PLAYING					2
#define END						3

//C->S
#define CS_KEY					1
#define CS_ROTATE				2
#define CS_ITEMGET				3
#define CS_SHOOT				4

//S->C
#define SC_GAMESTATE			0
#define SC_PLAYER				1
#define SC_ITEM					2
#define SC_TIMER				3
#define SC_SHOOT				4
#define SC_REMOVE_PLAYER		5

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
	int x;								//임시로 float -> int
	int y;
	int z;
	float rotateX;
	float rotateY;
	SOCKET sock;
	bool in_use;
	OVERAPPED_EX my_overapped;
	//set<int> view_list;						// 추후 필요시 사용 예정
	//mutex	vl_lock;							// 대신 크리티컬 섹션 사용
};

#pragma pack (push, 1)

// 패킷[0] = size
// 패킷[1] = type

struct CS_key{						// 키 값 -> 키 조작
	BYTE size;
	BYTE type;						// 1		
	int direction;					// 방향(일단 더미에서만)
	int movetype;					// 1234
};
struct CS_Rotate{					// 플레이어 회전 -> 마우스 조작
	BYTE size;
	BYTE type;						//2
	float rotateX;
	float rotateY;
};

struct CS_ItemGet{					// 아이템 획득
	BYTE size;
	BYTE type;						//3
};

struct CS_ShootKey{					// 총 쏘는 키 받기
	BYTE size;
	BYTE type;						// 4;
};

//////////////////////////////////////////////

struct SC_State{					// 게임 전체 상태
	BYTE size;
	BYTE type;						// 0
	BYTE state;						// 0, 1, 2, 3
};
struct SC_Player{					// 플레이어 위치
	BYTE size;	
	BYTE type;						//1					
	float x;
	float y;
	float z;
	float rotate_x;					//좌우
	float rotate_y;					//상하
	float rotate_z;					//
	int ID;		
	int state;						//케릭터 상태 -> 추후에 이펙트 추가 시 사용
};

struct SC_Item{						// 아이템 위치
	BYTE size;
	BYTE type;						// 2
	float x;
	float y;
	float z;
	bool isExist;
};

struct SC_Timer{
	BYTE size;
	BYTE type;						// 3
	float time;
};

struct SC_Shoot{
	BYTE size;							
	BYTE type;						// 4
	int ID;
};

struct SC_RemovePlayer{
	BYTE size;
	BYTE type;						//5
	int ID;
};

////////////

struct SC_Room{
	BYTE type;
	BYTE size;
	int num;
	int ID[8];
};

#pragma pack (pop)