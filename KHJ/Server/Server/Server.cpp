#include "Server.h"

HANDLE hIOCP;
PLAYER CServer::client[8] = { 0, };

//패킷	
SC_Player CServer::m_pos;
SC_Player CServer::m_playerpacket;
SC_State CServer::gamestate;

CPlayer CServer::m_player;
CGameManager CServer::GM;

CServer* CServer::m_serverInstance = NULL;

CServer::CServer(){
	usernum = 0;
}
CServer::~CServer(){}

void CServer::PlayerInit(int id){
	ZeroMemory(&client[id].my_overapped, sizeof(OVERAPPED_EX));
	client[id].in_use = false;
	client[id].my_overapped.operation_type = OP_RECV;
	client[id].my_overapped.wsabuf.buf = client[id].my_overapped.IOCPbuf;
	client[id].my_overapped.wsabuf.len = sizeof(client[id].my_overapped.IOCPbuf);
}

void CServer::SendPacket(int id, void *packet)
{
	int packet_size = reinterpret_cast<unsigned char *>(packet)[0];

	OVERAPPED_EX *send_over = new OVERAPPED_EX;
	ZeroMemory(send_over, sizeof(OVERAPPED_EX));
	send_over->operation_type = OP_SEND;
	send_over->wsabuf.buf = send_over->IOCPbuf;
	send_over->wsabuf.len = packet_size;
	unsigned long io_size;

	memcpy(send_over->IOCPbuf, packet, packet_size);

	WSASend(client[id].sock, &send_over->wsabuf, 1,
		&io_size, NULL, &send_over->overapped, NULL);
}

int CServer::GetNewClient_ID()
{
	for (int i = 0; i < MAXUSER; ++i)
		if (client[i].in_use == false){
			PlayerInit(i);
			return i;
		}
	cout << "USER FULL ERROR!!";
	exit(-1);
	return -1;
}

void CServer::ProcessPacket(char* packet, int id){

	m_pos.x = client[id].x;
	m_pos.y = client[id].y;

	CS_key *key = reinterpret_cast<CS_key*> (packet);
	//플레이어 키값 받을 시 플레이어 위치 및 여러가지 값 바꿔주기
	if (CS_KEY == key->type){
		m_pos.size = sizeof(m_pos);
		m_pos.type = SC_PLAYER;
		m_pos.ID = id;
		m_pos = m_player.PlayerPos(key->movetype, m_pos);
	}

	client[id].x = m_pos.x;
	client[id].y = m_pos.y;

	printf("%d, %d\n", client[id].x, client[id].y);

	for (int i = 0; i <= usernum; ++i){
	//	EnterCriticalSection(&cs);
		SendPacket(i, &m_pos);
	//	LeaveCriticalSection(&cs);
	}

	// state를 계속 해서 보내줌
	for (int i = 0; i <= usernum; ++i)
		SendPacket(i, &gamestate);

}

void CServer::Accept_thread(){
	struct sockaddr_in listen_addr;
	struct sockaddr_in client_addr;

	if (STANDBY == gamestate.type){
	
		SOCKET listen_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL,
			0, WSA_FLAG_OVERLAPPED);

		ZeroMemory(&listen_addr, sizeof(listen_addr));

		// listen
		listen_addr.sin_family = AF_INET;
		listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		listen_addr.sin_port = htons(SERVER_PORT);
		ZeroMemory(&listen_addr.sin_zero, 8);

		int ret = ::bind(listen_socket, reinterpret_cast<sockaddr*>(&listen_addr),
			sizeof(listen_addr));
		if (SOCKET_ERROR == ret){
			error_display("BIND", WSAGetLastError());
			exit(-1);
		}
		ret = listen(listen_socket, 8);
		if (SOCKET_ERROR == ret) {
			error_display("LISTEN", WSAGetLastError());
			exit(-1);
		}

		while (true){
			int addr_size = sizeof(client_addr);
			SOCKET client_socket = WSAAccept(listen_socket, reinterpret_cast<sockaddr*>(&client_addr),
				&addr_size, NULL, NULL);
			if (INVALID_SOCKET == client_socket){
				error_display("Accept", WSAGetLastError());
				exit(-1);
			}

			// 플레이어 접속 / 위치 / 상태
			int id = GetNewClient_ID();
			client[id].sock = client_socket;

			printf("접속 %s \n ID : %d \n", inet_ntoa(client_addr.sin_addr), id);

			CreateIoCompletionPort(reinterpret_cast<HANDLE>(client_socket),
				hIOCP, id, 0);
			unsigned long recv_flag = 0;

			ret = WSARecv(client_socket, &client[id].my_overapped.wsabuf, 1, NULL,
				&recv_flag, &client[id].my_overapped.overapped, NULL);

			if (SOCKET_ERROR == ret){
				int err_code = WSAGetLastError();
				if (WSA_IO_PENDING != err_code){
					error_display("Accept(WSARecv):", err_code);
					exit(-1);
				}
			}
			++usernum;					// 접속 시 유저 수
			// 서버에서 클라로 보내줄 플레이어 패킷
			// SC_Player
			m_playerpacket = m_player.PlayerAccept(id, m_playerpacket);
			client[id].x = m_playerpacket.x;
			client[id].y = m_playerpacket.y;
			client[id].in_use = true;

			for (int i = 0; i <= usernum; ++i){
				SendPacket(i, &m_playerpacket);
			}

			// 이전에 들어와 있는 패킷을 다시 보내주는 작업
			for (int i = 0; i <= usernum; ++i){
				if (false == client[i].in_use) continue;
				if (i == id)continue;
				m_playerpacket.ID = i;
				m_playerpacket.x = client[i].x;
				m_playerpacket.y = client[i].y;
				SendPacket(id, &m_playerpacket);
			}
			
			///////////////////////////////////////////////////// 유저 수 count다시 짜기

			gamestate.size = sizeof(SC_State);
			gamestate.type = SC_GAMESTATE;
			gamestate = GM.GameState(gamestate, usernum);
			
			for (int i = 0; i <= usernum; ++i)
				SendPacket(i, &gamestate);
		}
	}
}
void CServer::Process_Event(event_type nowevent){
	switch (nowevent.do_event){
	case EVENT_MOVE:
	{
		OVERAPPED_EX *event_over = new OVERAPPED_EX;
		event_over->operation_type = OP_MOVE;
		PostQueuedCompletionStatus(hIOCP, 1, nowevent.id,
			reinterpret_cast<LPOVERLAPPED>(event_over));
		break;
	}
	default:
		printf("Unknown Event Type Detected! \n");
	}
}

void CServer::worker_thread(){

	while (true)
	{
		unsigned long io_size;
		unsigned long key;
		OVERAPPED_EX *over_ex;
		GetQueuedCompletionStatus(hIOCP, &io_size, &key, 
			reinterpret_cast<LPOVERLAPPED*> (&over_ex), INFINITE);
	
		// 에러로 인한 접속 종료
		if (0 == io_size){
			SC_RemovePlayer repacket;
			repacket.ID = key;
			repacket.size = sizeof(repacket);
			repacket.type = SC_REMOVE_PLAYER;
			for (int i = 0; i < 8; ++i){
				if (key == i) continue;
				if (false == client[i].in_use) continue;
				SendPacket(i, &repacket);
			}
		}
		//recv
		if (OP_RECV == over_ex->operation_type){
			int rest_size = io_size;
			char* buf = over_ex->IOCPbuf;
			int packet_size = over_ex->curr_packet_size;
			while (0 < rest_size){
				if (0 == packet_size)
					packet_size = buf[0];
				int remain = packet_size - over_ex->prev_received;

				//패킷을 만들기에 공간이 부족한 경우
				if (remain > rest_size){
					memcpy(over_ex->PacketBuf + over_ex->prev_received,
						buf, rest_size);
					over_ex->prev_received += rest_size;
				}
				// 패킷 만들기에 충분
				else{
					memcpy(over_ex->PacketBuf + over_ex->prev_received,
						buf, remain);
					// 프로세스패킷 함수 ( 클라에서 받아온 정보 패킷 처리)
					ProcessPacket(over_ex->PacketBuf, key);
					rest_size -= remain;
					packet_size = 0;
					over_ex->prev_received = 0;
				}
			}
			over_ex->curr_packet_size = packet_size;
			unsigned long recv_flag = 0;
			WSARecv(client[key].sock, &over_ex->wsabuf, 1, NULL,
				&recv_flag, &over_ex->overapped, NULL);
		}
		//SEND
 		else if (OP_SEND == over_ex->operation_type)
			delete over_ex;
	}
}

void main(){
	//CServer server;
	//	InitializeCriticalSection(&cs);x

	vector <thread *> worker_threads;

	_wsetlocale(LC_ALL, L"korean");
	CServer::GetInstance()->NetworkInit();

	for (int i = 0; i < 9; ++i) 
		CServer::GetInstance()->PlayerInit(i);
	
	hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	for (int i = 0; i < NUM_THREADS; ++i)
		//worker_threads.push_back(new thread{ CServer::GetInstance()->worker_thread() });
		worker_threads.push_back(new thread{ [](){
											CServer::GetInstance()->worker_thread();
											} });

	thread acc = thread{ [](){
						CServer::GetInstance()->Accept_thread();
							} };

	//DeleteCriticalSection(&cs);

	while (true){
		Sleep(1000);
	}

}