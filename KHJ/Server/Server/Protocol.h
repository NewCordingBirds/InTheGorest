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

// �̵�Ű��
#define KEY_UP					1
#define KEY_DOWN				2
#define KEY_RIGHT				3
#define KEY_LEFT				4
//���� ����
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
	int x;								//�ӽ÷� float -> int
	int y;
	int z;
	float rotateX;
	float rotateY;
	SOCKET sock;
	bool in_use;
	OVERAPPED_EX my_overapped;
	//set<int> view_list;						// ���� �ʿ�� ��� ����
	//mutex	vl_lock;							// ��� ũ��Ƽ�� ���� ���
};

#pragma pack (push, 1)

// ��Ŷ[0] = size
// ��Ŷ[1] = type

struct CS_key{						// Ű �� -> Ű ����
	BYTE size;
	BYTE type;						// 1		
	int direction;					// ����(�ϴ� ���̿�����)
	int movetype;					// 1234
};
struct CS_Rotate{					// �÷��̾� ȸ�� -> ���콺 ����
	BYTE size;
	BYTE type;						//2
	float rotateX;
	float rotateY;
};

struct CS_ItemGet{					// ������ ȹ��
	BYTE size;
	BYTE type;						//3
};

struct CS_ShootKey{					// �� ��� Ű �ޱ�
	BYTE size;
	BYTE type;						// 4;
};

//////////////////////////////////////////////

struct SC_State{					// ���� ��ü ����
	BYTE size;
	BYTE type;						// 0
	BYTE state;						// 0, 1, 2, 3
};
struct SC_Player{					// �÷��̾� ��ġ
	BYTE size;	
	BYTE type;						//1					
	float x;
	float y;
	float z;
	float rotate_x;					//�¿�
	float rotate_y;					//����
	float rotate_z;					//
	int ID;		
	int state;						//�ɸ��� ���� -> ���Ŀ� ����Ʈ �߰� �� ���
};

struct SC_Item{						// ������ ��ġ
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