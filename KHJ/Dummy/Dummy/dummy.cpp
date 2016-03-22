//#include <windows.h>
#pragma comment(lib, "ws2_32")
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console" )

#include <winsock2.h>
#include <stdlib.h> 
#include <stdio.h>
#include <time.h>
#include "D:\GraduationWork\InTheGorest\KHJ\Server\Server\Protocol.h"

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512

#define MAX 380
#define MIN 20

#define WM_SOCKET			WM_USER+1

PLAYER enemy[7];				// �ٸ� �÷��̾� 7��
PLAYER pl;							// ����
PLAYER bufP;

int countplayer = 0;						// ������ ��� ��
bool Mych = 0;
bool Enemych[7] = { 0 };

HWND g_hWnd;
SOCKET Mysock;
WSABUF sendWSA;
char send_buf[BUFSIZE];
WSABUF recvWSA;
char recv_buf[BUFSIZE];
char packet_buf[BUFSIZE];
DWORD packet_size = 0;
int savePacketsize = 0;
int myID;
int enemyID[7];
bool GameStart;						// ���� ��ŸƮ

DWORD WINAPI ClientMain(LPVOID arg);

// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// ���� �Լ� ���� ���
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	//printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// ����� ���� ������ ���� �Լ�
//int recvn(SOCKET s, char *buf, int len, int flags)
//{
//	int received;
//	char *ptr = buf;
//	int left = len;
//
//	while (left > 0){
//		received = recv(s, ptr, left, flags);
//		if (received == SOCKET_ERROR)
//			return SOCKET_ERROR;
//		else if (received == 0)
//			break;
//		left -= received;
//		ptr += received;
//	}
//
//	return (len - left);
//}


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass = "Mouse";

void ProcessPacket(char *ptr){
	static bool first_time = true;
	static int id;
	switch (ptr[1]){
	case SC_GAMESTATE:{
		SC_State *game_state = reinterpret_cast<SC_State*>(ptr);
		printf("%d \n", game_state->state);
		if (PLAYING != game_state->state)
			GameStart = false;
		else GameStart = true;
		break;
	}

	case SC_PLAYER:{
		SC_Player *my_packet = reinterpret_cast<SC_Player*>(ptr);
		id = my_packet->ID;

		if (first_time) {
			first_time = false;
			myID = id;
		}
		if (myID == id){					//����
			Mych = true;
			pl.x = my_packet->x;
			pl.y = my_packet->y;
		}
		else{								//Ÿ �÷��̾�
			enemyID[id] = id;
			Enemych[id] = true;

			enemy[id].x = my_packet->x;
			enemy[id].y = my_packet->y;
		}
		InvalidateRect(g_hWnd, NULL, true);
		break;
	}

	case SC_REMOVE_PLAYER:{
		SC_RemovePlayer *replayer = reinterpret_cast<SC_RemovePlayer*>(ptr);
		id = replayer->ID;

		Enemych[id] = false;
		InvalidateRect(g_hWnd, NULL, true);
		break;
	}
	//case SC_ITEM:
	//	break;
	//case SC_TIMER:
	//	break;
	//case SC_SHOOT:
	//	break;
	}
}

void ReadPacket(SOCKET sock){
	DWORD iobyte, ioflag = 0;

	int ret = WSARecv(sock, &recvWSA, 1, &iobyte, &ioflag, NULL, NULL);
	if (ret){
		int err_code = WSAGetLastError();
		printf("Recv Error [%d] \n", err_code);
	}
	BYTE *ptr = reinterpret_cast<BYTE*>(recv_buf);

	while (0 != iobyte){
		if (0 == packet_size){
			packet_size = ptr[0];
		}
		if (iobyte + savePacketsize >= packet_size){
			memcpy(packet_buf + savePacketsize, ptr, packet_size - savePacketsize);
			ProcessPacket(packet_buf);
			ptr += packet_size - savePacketsize;
			iobyte -= packet_size - savePacketsize;
			packet_size = 0;
			savePacketsize = 0;
		}
		else{
			memcpy(packet_buf + savePacketsize, ptr, iobyte);

			savePacketsize += iobyte;
			iobyte = 0;
		}
	}
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	int retval;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	g_hWnd = hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,500, 500,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static BOOL bnowDraw = FALSE;
	int retval;
	PAINTSTRUCT ps;
	//static point line;
	static HANDLE hTimer;
	
	HBRUSH MyBrush, OldBrush;
	HPEN Mypen, Oldpen;

	static int CH_x, CH_y;			//�ɸ��� ��ǥ
	static int direction;
	static int num;					//��� ��ȣ

	static bool pillow[5];			//��� 5�� ���� ����
	static bool have_pillow;

	static int hori;				// x��		0-> �ʱ� 1-> + 2-> -
	static int verti;				// y��

	static bool shoot;				// �Ѿ�(�ѹ��� ���� ����)
										// ���������� ��Ҵٴ� ��ȣ�� ���� �� ��
	static int shootCount;


	static int pillow_x[5], pillow_y[5];	//����� ��ġ

	switch (iMessage) {
	case WM_CREATE:

		// ���� �ʱ�ȭ
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
			return 1;

		// socket()
		Mysock =WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
		if (Mysock == INVALID_SOCKET) err_quit("socket()");

		// connect()
		SOCKADDR_IN serveraddr;
		ZeroMemory(&serveraddr, sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
		serveraddr.sin_port = htons(SERVERPORT);
		retval = WSAConnect(Mysock, (sockaddr *)&serveraddr, sizeof(serveraddr),
			NULL, NULL, NULL, NULL);
		if (retval == SOCKET_ERROR) err_quit("connect()");

		WSAAsyncSelect(Mysock, hWnd, WM_SOCKET, FD_CLOSE | FD_READ);

		sendWSA.buf = send_buf;
		sendWSA.len = BUFSIZE;
		recvWSA.buf = recv_buf;
		recvWSA.len = BUFSIZE;

		// ������ ��ſ� ����� ����
		char buf[BUFSIZE + 1];
		int len;
		return 0;
		hori = 0;
		verti = 0;
		shootCount = 0;
		shoot = false;

		//hTimer = (HANDLE)SetTimer(hWnd, 1, 30, NULL);

		return 0;

	case WM_KEYDOWN:{
		
		if (GameStart){
			//CS_ShootKey *shootkey = reinterpret_cast<CS_ShootKey*>(send_buf);
			CS_key *key = reinterpret_cast<CS_key*> (send_buf);
			if (wParam == VK_RIGHT){
				hori = 1;
				verti = 0;
			}
			if (wParam == VK_LEFT){
				hori = 2;
				verti = 0;
			}
			if (wParam == VK_UP){
				verti = 1;
				hori = 0;
			}
			if (wParam == VK_DOWN){
				verti = 2;
				hori = 0;
			}/*if (wParam == VK_SPACE){
				shoot = true;
			}*/
			key->type = CS_KEY;
			key->size = sizeof(CS_key);
			sendWSA.len = sizeof(CS_key);

			int ret;
			DWORD iobyte;
			if (0 != hori){
				if (1 == hori) {
					key->movetype = KEY_RIGHT;
					key->direction = 1;
				}
				else{
					key->movetype = KEY_LEFT;
					key->direction = 2;
				}
			}
			if (0 != verti){
				if (1 == verti) {
					key->movetype = KEY_UP;
					key->direction = 3;
				}
				else {
					key->movetype = KEY_DOWN;
					key->direction = 4;
				}
			}
			ret = WSASend(Mysock, &sendWSA, 1, &iobyte, 0, NULL, NULL);
			if (ret){
				int error_code = WSAGetLastError();
				printf("Error while sending packet[%d]", error_code);
			}


			//InvalidateRect(hWnd, NULL, TRUE);
		}
	}
		return 0;

	case WM_KEYUP:
		shoot = false;
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//���� ����
		Rectangle(hdc, 20, 20, 400, 400);

		//�ӽ� �ɸ���
		if (Mych){
			MyBrush = CreateHatchBrush(HS_BDIAGONAL, RGB(255, 0, 0));
			OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);
			Mypen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			Oldpen = (HPEN)SelectObject(hdc, Mypen);
			Ellipse(hdc, pl.x, pl.y, pl.x + 20, pl.y + 20);		//ĳ���� ��ü(����)
			
			//if (direction == 1){								//���� �ﰢ��(������)
			//	MoveToEx(hdc, pl.x + 20, pl.y, NULL);
			//	LineTo(hdc, pl.x + 20, pl.y + 20);
			//	MoveToEx(hdc, pl.x + 20, pl.y + 20, NULL);
			//	LineTo(hdc, pl.x + 30, pl.y + 10);
			//	MoveToEx(hdc, pl.x + 30, pl.y + 10, NULL);
			//	LineTo(hdc, pl.x + 20, pl.y);
			//}if (direction == 2){								//���� �ﰢ��(����)
			//	MoveToEx(hdc, pl.x, pl.y, NULL);
			//	LineTo(hdc, pl.x, pl.y + 20);
			//	MoveToEx(hdc, pl.x, pl.y + 20, NULL);
			//	LineTo(hdc, pl.x - 10, pl.y + 10);
			//	MoveToEx(hdc, pl.x - 10, pl.y + 10, NULL);
			//	LineTo(hdc, pl.x, pl.y);
			//}if (direction == 3){								//���� �ﰢ��(��)
			//	MoveToEx(hdc, pl.x, pl.y, NULL);
			//	LineTo(hdc, pl.x + 20, pl.y);
			//	MoveToEx(hdc, pl.x + 20, pl.y, NULL);
			//	LineTo(hdc, pl.x + 10, pl.y - 10);
			//	MoveToEx(hdc, pl.x + 10, pl.y - 10, NULL);
			//	LineTo(hdc, pl.x, pl.y);
			//}if (direction == 4){								//���� �ﰢ��(�Ʒ�)
			//	MoveToEx(hdc, pl.x, pl.y + 20, NULL);
			//	LineTo(hdc, pl.x + 20, pl.y + 20);
			//	MoveToEx(hdc, pl.x + 20, pl.y + 20, NULL);
			//	LineTo(hdc, pl.x + 10, pl.y + 30);
			//	MoveToEx(hdc, pl.x + 10, pl.y + 30, NULL);
			//	LineTo(hdc, pl.x, pl.y + 20);
			//}
			SelectObject(hdc, OldBrush);
			SelectObject(hdc, Oldpen);
			DeleteObject(MyBrush);
			DeleteObject(Mypen);
		}
		for (int i = 0; i <  8; ++i){
			if (Enemych[i]){	// Ÿ �÷��̾�

				MyBrush = CreateHatchBrush(HS_BDIAGONAL, RGB(0, 0, 0));
				OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);
				Mypen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				Oldpen = (HPEN)SelectObject(hdc, Mypen);
				Ellipse(hdc, enemy[i].x, enemy[i].y, enemy[i].x + 20, enemy[i].y + 20);		//ĳ���� ��ü(����)

				//if (direction == 1){								//���� �ﰢ��(������)
				//	MoveToEx(hdc, enemy[i].x + 20, enemy[i].y, NULL);
				//	LineTo(hdc, enemy[i].x + 20, enemy[i].y + 20);
				//	MoveToEx(hdc, enemy[i].x + 20, enemy[i].y + 20, NULL);
				//	LineTo(hdc, enemy[i].x + 30, enemy[i].y + 10);
				//	MoveToEx(hdc, enemy[i].x + 30, enemy[i].y + 10, NULL);
				//	LineTo(hdc, enemy[i].x + 20, enemy[i].y);
				//}if (direction == 2){								//���� �ﰢ��(����)
				//	MoveToEx(hdc, enemy[i].x, enemy[i].y, NULL);
				//	LineTo(hdc, enemy[i].x, enemy[i].y + 20);
				//	MoveToEx(hdc, enemy[i].x, enemy[i].y + 20, NULL);
				//	LineTo(hdc, enemy[i].x - 10, enemy[i].y + 10);
				//	MoveToEx(hdc, enemy[i].x - 10, enemy[i].y + 10, NULL);
				//	LineTo(hdc, enemy[i].x, enemy[i].y);
				//}if (direction == 3){								//���� �ﰢ��(��)
				//	MoveToEx(hdc, enemy[i].x, enemy[i].y, NULL);
				//	LineTo(hdc, enemy[i].x + 20, enemy[i].y);
				//	MoveToEx(hdc, enemy[i].x + 20, enemy[i].y, NULL);
				//	LineTo(hdc, enemy[i].x + 10, enemy[i].y - 10);
				//	MoveToEx(hdc, enemy[i].x + 10, enemy[i].y - 10, NULL);
				//	LineTo(hdc, enemy[i].x, enemy[i].y);
				//}if (direction == 4){								//���� �ﰢ��(�Ʒ�)
				//	MoveToEx(hdc, enemy[i].x, enemy[i].y + 20, NULL);
				//	LineTo(hdc, enemy[i].x + 20, enemy[i].y + 20);
				//	MoveToEx(hdc, enemy[i].x + 20, enemy[i].y + 20, NULL);
				//	LineTo(hdc, enemy[i].x + 10, enemy[i].y + 30);
				//	MoveToEx(hdc, enemy[i].x + 10, enemy[i].y + 30, NULL);
				//	LineTo(hdc, enemy[i].x, enemy[i].y + 20);
				//}			
				
				SelectObject(hdc, OldBrush);
				SelectObject(hdc, Oldpen);
				DeleteObject(MyBrush);
				DeleteObject(Mypen);

			}
		}
		EndPaint(hWnd, &ps);
		return 0;

	case WM_SOCKET:
		if (WSAGETSELECTERROR(lParam)){
			closesocket((SOCKET)wParam);
			exit(-1);
			break;
		}
		switch (WSAGETSELECTEVENT(lParam)){
		case FD_READ:
			ReadPacket((SOCKET)wParam);
			break;
		case FD_CLOSE:
			closesocket((SOCKET)wParam);
			exit(-1);
			break;
		}
	default:break;
	

	case WM_DESTROY:
		//KillTimer(hWnd, 1);
		PostQuitMessage(0);

		return 0;	
	
		}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

DWORD WINAPI ClientMain(LPVOID arg){
	//int retval;

	//while (1){
	//	retval = recvn(Mysock, (char*)&bufP, sizeof(PLAYER), 0);
	//	if (retval == SOCKET_ERROR){
	//		err_display("recv()");
	//		break;
	//	}
	//	if (retval == 0)
	//		break;
	//	if (0 != retval){
	//		AllocConsole();
	//		freopen("CONOUT$", "wt", stdout);
	//		printf("���� �Ǿ����ϴ�!\n");

	//	}
	//}

	// closesocket()
	closesocket(Mysock);

	// ���� ����
	WSACleanup();
	return 0;
}