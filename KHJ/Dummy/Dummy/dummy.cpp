//#include <windows.h>
#pragma comment(lib, "ws2_32")

#include <winsock2.h>
#include <stdlib.h> 
#include <stdio.h>
#include <time.h>
#include "..\..\Server\Server\Protocol.h"

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512

#define MAX 380
#define MIN 20

PLAYER pl;
PLAYER bufP;
SOCKET sock;

int playertype = 0;					// 1 -> ���� 2-> Ÿ��

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
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0){
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass = "Mouse";

void ProcessPacket(char *ptr){
	switch (ptr[0]){
	
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

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
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

	HPEN Mypen, Oldpen;

	static int i, j;				//ü���� ����, ����
	static int CH_x, CH_y;			//�ɸ��� ��ǥ
	static int direction;
	static int num;					//��� ��ȣ
	
	static bool pillow[5];			//��� 5�� ���� ����
	static bool have_pillow;
	static bool keyup;
	
	static int pillow_x[5], pillow_y[5];	//����� ��ġ

	switch (iMessage) {
	case WM_CREATE:

		// ���� �ʱ�ȭ
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
			return 1;

		// socket()
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET) err_quit("socket()");

		// connect()
		SOCKADDR_IN serveraddr;
		ZeroMemory(&serveraddr, sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
		serveraddr.sin_port = htons(SERVERPORT);
		retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR) err_quit("connect()");

		// ������ ��ſ� ����� ����
		char buf[BUFSIZE + 1];
		int len;
		return 0;
		keyup = false;
	
	case WM_KEYDOWN:
		switch (wParam){	
		
		case VK_RIGHT:
			direction = 1;
			if (CH_x < MAX){
				CH_x += 2;
			}
			break;

		case VK_LEFT:
			direction = 2;
			if (CH_x > MIN){
				CH_x -= 2;
			}

			break;

		case VK_UP:
			direction = 3;
			if (CH_y > MIN){
			CH_y -= 2;
		}
			break;

		case VK_DOWN:
			direction = 4;
			if (CH_y < MAX){
				
				CH_y += 2;
			}
			break;

		}
		keyup = true;
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_KEYUP:
		keyup = false;
		return 0;


	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//���� ����
		Rectangle(hdc, 20, 20, 400, 400);		

		//�ӽ� �ɸ���
		Ellipse(hdc, CH_x, CH_y, CH_x+20, CH_y+20);		//ĳ���� ��ü(����)
		if (1 == playertype)
			Mypen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			
		else
			Mypen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			
		Oldpen = (HPEN)SelectObject(hdc, Mypen);


		if (direction == 1){								//���� �ﰢ��(������)
			MoveToEx(hdc, CH_x + 20, CH_y, NULL);
			LineTo(hdc, CH_x + 20, CH_y + 20);
			MoveToEx(hdc, CH_x + 20, CH_y + 20, NULL);
			LineTo(hdc, CH_x + 30, CH_y + 10);
			MoveToEx(hdc, CH_x + 30, CH_y + 10, NULL);
			LineTo(hdc, CH_x + 20, CH_y);
		}if (direction == 2){								//���� �ﰢ��(����)
			MoveToEx(hdc, CH_x, CH_y, NULL);
			LineTo(hdc, CH_x, CH_y + 20);
			MoveToEx(hdc, CH_x, CH_y + 20, NULL);
			LineTo(hdc, CH_x - 10, CH_y + 10);
			MoveToEx(hdc, CH_x -10, CH_y + 10, NULL);
			LineTo(hdc, CH_x, CH_y);
		}if (direction == 3){								//���� �ﰢ��(��)
			MoveToEx(hdc, CH_x, CH_y, NULL);
			LineTo(hdc, CH_x+20, CH_y);
			MoveToEx(hdc, CH_x + 20, CH_y, NULL);
			LineTo(hdc, CH_x + 10, CH_y - 10);
			MoveToEx(hdc, CH_x + 10, CH_y - 10, NULL);
			LineTo(hdc, CH_x, CH_y);
		}if (direction == 4){								//���� �ﰢ��(�Ʒ�)
			MoveToEx(hdc, CH_x, CH_y+20, NULL);
			LineTo(hdc, CH_x + 20, CH_y + 20);
			MoveToEx(hdc, CH_x + 20, CH_y + 20, NULL);
			LineTo(hdc, CH_x + 10, CH_y + 30);
			MoveToEx(hdc, CH_x + 10, CH_y + 30, NULL);
			LineTo(hdc, CH_x, CH_y+20);
		}

		SelectObject(hdc, Oldpen);
		DeleteObject(Mypen);

		EndPaint(hWnd, &ps);
		return 0;

	case WM_TIMER:
		if (TRUE == keyup){
			retval = send(sock, (char*)&pl, sizeof(PLAYER), 0);
			if (retval == SOCKET_ERROR){
				err_display("send()");
			}

		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hWnd, 1);

		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

DWORD WINAPI ClientMain(LPVOID arg){
	int retval;

	while (1){
		retval = recvn(sock, (char*)&bufP, sizeof(PLAYER), 0);
		if (retval == SOCKET_ERROR){
			err_display("recv()");
			break;
		}
		if (retval == 0)
			break;
		if (0 != retval){
			AllocConsole();
			freopen("CONOUT$", "wt", stdout);
			printf("���� �Ǿ����ϴ�!\n");

		}
	}
	
	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}