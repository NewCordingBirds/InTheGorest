#include "stdafx.h"
#include "ServerMgr.h"
#include <conio.h>

extern HWND g_hWnd;

IMPLEMENT_SINGLETON(CServer)


CServer::CServer()
:packet_size(0)
, savePacketsize(0)
, myID(0)

{
	ZeroMemory(&send_buf, sizeof(send_buf));
	ZeroMemory(&recv_buf, sizeof(recv_buf));
	ZeroMemory(&packet_buf, sizeof(packet_buf));
	ZeroMemory(&sendWSA, sizeof(sendWSA));
	ZeroMemory(&recvWSA, sizeof(recvWSA));

	m_pPlayerObserver = CPlayerObserver::Create();
	CInfoSubject::GetInstance()->Subscribe(m_pPlayerObserver);


	ZeroMemory(&m_tInitPlayer, sizeof(SC_InitPlayer));
	ZeroMemory(&m_tPlayerPos, sizeof(SC_PlayerPos));

	ZeroMemory(&m_tInitEnemy, sizeof(SC_InitPlayer) * ROOM_MAXUSER);
	ZeroMemory(&m_tEnemyPos, sizeof(SC_PlayerPos) * ROOM_MAXUSER);
	ZeroMemory(&m_tEnemyRotate, sizeof(SC_PlayerRotate) * ROOM_MAXUSER);
	ZeroMemory(&m_tEnemyAvoid, sizeof(SC_AvoidPlayer)*ROOM_MAXUSER);
	ZeroMemory(&m_tEnemyAnimation, sizeof(SC_ServerAni) * ROOM_MAXUSER);
	ZeroMemory(&m_tEnemyEndBoost, sizeof(SC_Endboost) * ROOM_MAXUSER);
	ZeroMemory(&m_tEnemyBullet, sizeof(SC_Shoot) * ROOM_MAXUSER);

	ZeroMemory(&m_tState, sizeof(SC_State));
	ZeroMemory(&m_tTime, sizeof(SC_Time));


	m_tItemExist.isExist = false;
	m_tItemExist.itemid = -1;

	for (int i = 0; i < MAX_USER_NUM; ++i)
	{
		m_tInitEnemy[i].ID = -1;
		m_tEnemyPos[i].ID = -1;
		m_tEnemyAnimation[i].ID = -1;
		m_tEnemyBullet[i].ID = -1;
	}

	InitializeCriticalSection(&cs);

}

CServer::~CServer()
{
	CInfoSubject::GetInstance()->Unsubscribe(m_pPlayerObserver);
	::Safe_Delete(m_pPlayerObserver);

	DeleteCriticalSection(&cs);
}

void CServer::ProcessPacket(char *ptr)
{
	static bool first_time = true;
	static int id;

	switch (ptr[1])
	{
	case SC_INITPLAYER:
	{
		SC_InitPlayer *Initplayer = reinterpret_cast<SC_InitPlayer*>(ptr);
		// 첫 입장 시 위치
		id = Initplayer->ID;

		if (first_time){
			first_time = false;
			myID = id;
		}
		if (myID == id)
		{
			m_tInitPlayer = *Initplayer;
			m_tPlayerPos.ID = m_tInitPlayer.ID;
			m_tPlayerPos.move = m_tInitPlayer.move;

		}
		else
		{
			m_tInitEnemy[Initplayer->ID] = *Initplayer;
			m_tEnemyPos[Initplayer->ID].ID = m_tInitEnemy[Initplayer->ID].ID;
			m_tEnemyPos[Initplayer->ID].move = m_tInitEnemy[Initplayer->ID].move;
		}
		break;
	}
	case SC_GAMESTATE:
	{
		SC_State *GameState = reinterpret_cast<SC_State*>(ptr);
		m_tState = *GameState;

		if (STANDBY == GameState->gamestate) break;			// 아직 플레이어 다 안들어옴			
		else if (READY == GameState->gamestate){

		}
		else if (PLAYING == GameState->gamestate){

		}
		else {
		}
		break;
	}
	case SC_PLAYERPOS:
	{
		SC_PlayerPos* playerpack = reinterpret_cast<SC_PlayerPos*>(ptr);
		id = playerpack->ID;
		if (myID == id)
		{
			m_tPlayerPos = *playerpack;
		}
		else
		{
			m_tEnemyPos[playerpack->ID] = *playerpack;
		}
		break;
	}
	case SC_PLAYERROTATE:
	{
		SC_PlayerRotate* playerrotate = reinterpret_cast<SC_PlayerRotate*>(ptr);
		id = playerrotate->ID;

		if (myID == id)
		{
			break;
		}
		else
		{
			m_tEnemyRotate[playerrotate->ID] = *playerrotate;
		}
		break;
	}

	case SC_AVOIDPLAYER:{
		SC_AvoidPlayer *playeravoid = reinterpret_cast<SC_AvoidPlayer*>(ptr);

		m_tPlayeravoid = *playeravoid;
		break;
	}
	case SC_ITEMEXIST:{
		SC_Item *item = reinterpret_cast<SC_Item*>(ptr);

		m_tItemExist = *item;
		//AllocConsole();
		//_cprintf("%d  id : %d \n", item->isExist, item->itemid);
		break;
	}
	case SC_TIME:
	{
		SC_Time* time = reinterpret_cast<SC_Time*>(ptr);
		m_tTime = *time;

		break;
	}
	case SC_REMOVE_PLAYER:
	{
		SC_RemovePlayer* remove = reinterpret_cast<SC_RemovePlayer*>(ptr);
		if (myID == id)
		{
			break;
		}
		else
		{
			m_tEnemyRemove[remove->ID] = *remove;
		}
		break;

	}
	case SC_SERVERANI:
	{
		SC_ServerAni* animation = reinterpret_cast<SC_ServerAni*>(ptr);
		if (myID == id)
		{
			break;
		}
		else
		{
			m_tEnemyAnimation[animation->ID] = *animation;
		}

	}
	break;
	case SC_ENDBOOST:{
		SC_Endboost* endboost = reinterpret_cast<SC_Endboost*>(ptr);
		if (myID == id)
		{
			break;
		}
		else
		{
			m_tEnemyEndBoost[endboost->ID] = *endboost;
		}
	}
					 break;

	case SC_SHOT:{
		SC_Shoot *shootpacket = reinterpret_cast<SC_Shoot*>(ptr);
		if (myID == shootpacket->ID){
			break;
		}
		else{
			m_tEnemyBullet[shootpacket->ID] = *shootpacket;
			//_cprintf("kkkkkkk");
		}
		break;
	}case SC_DIE:{
		SC_Die *diepacket = reinterpret_cast<SC_Die*>(ptr);



		break;
	}
	}

}

void CServer::ReadPacket(SOCKET sock)
{
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

void CServer::InitServer(HWND hwnd, string _strIP)
{

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	// socket()
	Mysock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
	if (Mysock == INVALID_SOCKET){
		int err_code = WSAGetLastError();
		printf("Sock err() : %d", err_code);

	}
	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;

	if (7 > _strIP.size())
		serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	else
		serveraddr.sin_addr.s_addr = inet_addr(_strIP.c_str());

	serveraddr.sin_port = htons(SERVER_PORT);
	int retval = WSAConnect(Mysock, (sockaddr *)&serveraddr, sizeof(serveraddr),
		NULL, NULL, NULL, NULL);
	if (retval == SOCKET_ERROR) {
		int err_code = WSAGetLastError();
		printf("Connect err() : %d", err_code);
	}

	WSAAsyncSelect(Mysock, hwnd, WM_SOCKET, FD_CLOSE | FD_READ);

	sendWSA.buf = send_buf;
	sendWSA.len = MAX_BUFFER_SIZE;
	recvWSA.buf = recv_buf;
	recvWSA.len = MAX_BUFFER_SIZE;
}

void CServer::Update_Send_Key()
{
	int ret;
	DWORD iobyte;

	static DWORD nStandardTime = GetTickCount();
	static DWORD nTimer = GetTickCount();
	nTimer = GetTickCount();

	if (nTimer - nStandardTime >= 20)
	{
		nStandardTime = nTimer;
	}
	else
		return;

	CS_Key tKey = m_pPlayerObserver->GetKey();
	CS_Key* direction = reinterpret_cast<CS_Key*>(send_buf);

	sendWSA.buf = reinterpret_cast<char*>(&tKey);
	sendWSA.len = sizeof(CS_Key);

	ret = WSASend(Mysock, &(sendWSA), 1, &iobyte, 0, NULL, NULL);

	if (ret){
		int err_code = WSAGetLastError();
		printf("Error Sending position packet[%d]\n", err_code);
	}
	ZeroMemory(&sendWSA, sizeof(sendWSA));
}
void CServer::Update_Send_Rotate()
{
	int ret;
	DWORD iobyte;

	//static DWORD nStandard = GetTickCount();
	//static DWORD nowTime = GetTickCount();
	//nowTime = GetTickCount();

	//if (nowTime - nStandard >= 30)
	//{
	//	nStandard = nowTime;
	//}
	//else
	//	return;

	CS_Rotate tRotate = m_pPlayerObserver->GetRotate();
	CS_Rotate *rotate = reinterpret_cast<CS_Rotate*>(send_buf);

	//EnterCriticalSection(&cs);
	//rotate->dir = tRotate.dir;
	/*rotate->rad = tRotate.rad;
	rotate->size = tRotate.size;
	rotate->type = tRotate.type;*/
	//LeaveCriticalSection(&cs);

	sendWSA.buf = reinterpret_cast<char*>(&tRotate);
	sendWSA.len = sizeof(CS_Rotate);

	ret = WSASend(Mysock, &(sendWSA), 1, &iobyte, 0, NULL, NULL);

	if (ret){
		int err_code = WSAGetLastError();
		printf("Error Sending rotation packet[%d]\n", err_code);
	}
	ZeroMemory(&sendWSA, sizeof(sendWSA));
}

void CServer::Update_Send_CurMapLength()
{
	// (수정)
	// 나중에 계속 진행률 보내주어야하는 것

	//int ret;
	//DWORD iobyte;

	//CS_Rotate tRotate = m_pPlayerObserver->GetRotate();
	//CS_Rotate *rotate = reinterpret_cast<CS_Rotate*>(send_buf);

	//sendWSA.buf = reinterpret_cast<char*>(&tRotate);
	//sendWSA.len = sizeof(CS_Rotate);

	//ret = WSASend(Mysock, &(sendWSA), 1, &iobyte, 0, NULL, NULL);

	//if (ret){
	//	int err_code = WSAGetLastError();
	//	printf("Error Sending rotation packet[%d]\n", err_code);
	//}
	//ZeroMemory(&sendWSA, sizeof(sendWSA));
}

void CServer::SendServerData(char* _pData)
{
	int ret;
	DWORD iobyte;

	switch (_pData[1]){
	case CS_READY:{
		CS_Ready *data = reinterpret_cast<CS_Ready*>(_pData);
		CS_Ready *Ready = reinterpret_cast<CS_Ready*>(send_buf);

		//EnterCriticalSection(&cs);
		Ready->type = data->type;
		Ready->size = data->size;
		//LeaveCriticalSection(&cs);

		sendWSA.len = sizeof(CS_Ready);

		ret = WSASend(Mysock, &(sendWSA), 1, &iobyte, 0, NULL, NULL);

		if (ret){
			int err_code = WSAGetLastError();
			printf("Error Sending ready packet[%d]\n", err_code);
		}
		ZeroMemory(&sendWSA, sizeof(sendWSA));

		break;
	}case CS_ITEMGET:{
		CS_ItemGet *data = reinterpret_cast<CS_ItemGet*>(_pData);
		//CS_ItemGet *getitem = reinterpret_cast<CS_ItemGet*>(send_buf);

		//getitem->type = data->type;
		//getitem->size = data->size;
		//getitem->itemnum = data->itemnum;

		sendWSA.buf = _pData;
		sendWSA.len = sizeof(CS_ItemGet);

		ret = WSASend(Mysock, &(sendWSA), 1, &iobyte, 0, NULL, NULL);

		if (ret){
			int err_code = WSAGetLastError();
			printf("Error Sending ready packet[%d]\n", err_code);
		}
		ZeroMemory(&sendWSA, sizeof(sendWSA));
		break;
	}case CS_BOOSTERITEM:{
		CS_UseBooster *data = reinterpret_cast<CS_UseBooster*>(_pData);
		//CS_CollBooster *collbooster = reinterpret_cast<CS_CollBooster*>(send_buf);

		//getitem->type = data->type;
		//getitem->size = data->size;
		//getitem->itemnum = data->itemnum;

		sendWSA.buf = _pData;
		sendWSA.len = sizeof(CS_UseBooster);

		ret = WSASend(Mysock, &(sendWSA), 1, &iobyte, 0, NULL, NULL);

		if (ret){
			int err_code = WSAGetLastError();
			printf("Error Sending ready packet[%d]\n", err_code);
		}
		ZeroMemory(&sendWSA, sizeof(sendWSA));
		break;
	}case CS_COLLBOOSTER:{
		CS_CollBooster *data = reinterpret_cast<CS_CollBooster*>(_pData);

		sendWSA.buf = _pData;
		sendWSA.len = sizeof(CS_CollBooster);

		ret = WSASend(Mysock, &(sendWSA), 1, &iobyte, 0, NULL, NULL);

		if (ret){
			int err_code = WSAGetLastError();
			printf("Error Sending ready packet[%d]\n", err_code);
		}
		ZeroMemory(&sendWSA, sizeof(sendWSA));
		break;
	}case CS_COLLWALL:{
		CS_CollWall *data = reinterpret_cast<CS_CollWall*>(_pData);

		sendWSA.buf = _pData;
		sendWSA.len = sizeof(CS_CollWall);

		ret = WSASend(Mysock, &(sendWSA), 1, &iobyte, 0, NULL, NULL);

		if (ret){
			int err_code = WSAGetLastError();
			printf("Error Sending ready packet[%d]\n", err_code);
		}
		ZeroMemory(&sendWSA, sizeof(sendWSA));
		break;
	}case CS_CLIENTANI:{
		CS_ClientAni* data = reinterpret_cast<CS_ClientAni*>(_pData);

		sendWSA.buf = _pData;
		sendWSA.len = sizeof(CS_ClientAni);

		ret = WSASend(Mysock, &(sendWSA), 1, &iobyte, 0, NULL, NULL);

		if (ret){
			int err_code = WSAGetLastError();
			printf("Error Sending ready packet[%d]\n", err_code);
		}
		ZeroMemory(&sendWSA, sizeof(sendWSA));
		break;
	}case CS_SHOT:{
		//CS_Shoot * data = reinterpret_cast<CS_Shoot*>(_pData);
		//CS_Shoot * packet = reinterpret_cast<CS_Shoot*>(send_buf);

		sendWSA.buf = _pData;
		sendWSA.len = sizeof(CS_Shoot);
		//packet->size = sizeof(packet);
		//packet->type = CS_SHOT;
		//packet->ammonum = data->ammonum;

		ret = WSASend(Mysock, &(sendWSA), 1, &iobyte, 0, NULL, NULL);

		if (ret){
			int err_code = WSAGetLastError();
			printf("Error Sending ready packet[%d]\n", err_code);
		}
		ZeroMemory(&sendWSA, sizeof(sendWSA));
		break;
	}case CS_DIE:{
		sendWSA.buf = _pData;
		sendWSA.len = sizeof(CS_Die);

		ret = WSASend(Mysock, &(sendWSA), 1, &iobyte, 0, NULL, NULL);

		if (ret){
			int err_code = WSAGetLastError();
			printf("Error Sending ready packet[%d]\n", err_code);
		}
		ZeroMemory(&sendWSA, sizeof(sendWSA));
		break;
	}
	}
}