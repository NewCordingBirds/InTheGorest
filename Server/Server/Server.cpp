#include "Server.h"

HANDLE hIOCP;
//패킷	
SC_PlayerPos	CServer::m_playerpos;
SC_PlayerRotate CServer::m_playerrotate;
SC_State		CServer::m_state;

CServer*		CServer::m_serverInstance = NULL;

CServer::CServer()
	:usernum(0),
	readynum(0),
	readystate(false),
	dummypos(false),
	starttime(0),
	rank(0),
	endsignal(false),
	keyvalue(0),
	rotateendtime(0),
	rotatestarttime(0)
{
	InitializeCriticalSection(&cs);
}
CServer::~CServer(){
	DeleteCriticalSection(&cs);
}

void CServer::PlayerInit(int id){
	ZeroMemory(&CPlayer::getInstance().client[id].my_overapped, sizeof(OVERAPPED_EX));
	CPlayer::getInstance().client[id].in_use = false;
	CPlayer::getInstance().client[id].my_overapped.operation_type = OP_RECV;
	CPlayer::getInstance().client[id].my_overapped.wsabuf.buf = CPlayer::getInstance().client[id].my_overapped.IOCPbuf;
	CPlayer::getInstance().client[id].my_overapped.wsabuf.len = sizeof(CPlayer::getInstance().client[id].my_overapped.IOCPbuf);
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
	if (true == CPlayer::getInstance().client[id].in_use )
		WSASend(CPlayer::getInstance().client[id].sock, &send_over->wsabuf, 1,
			&io_size, NULL, &send_over->overapped, NULL);
}
int CServer::GetNewClient_ID()
{
	for (int i = 0; i < MAXUSER; ++i)
		if (CPlayer::getInstance().client[i].in_use == false){
			PlayerInit(i);
			return i;
		}
	cout << "USER FULL ERROR!!";
	exit(-1);
	return -1;
}
void CServer::AddTimer(int id, int do_event, int wakeup)
{
	event_type new_event;
	new_event.do_event = do_event;
	new_event.id = id;
	new_event.time = wakeup + GetTickCount();
	EnterCriticalSection(&cs);
	timerQ.push(new_event);
	LeaveCriticalSection(&cs);
}
void CServer::SendPos(int i){

}
void CServer::ProcessPacket(char* packet, int id){
	switch (packet[1]){
	case CS_KEY:{
		CS_Key *key = reinterpret_cast<CS_Key*> (packet);
		SC_PlayerPos playerpos;
		keyvalue = key->movetype;

		
		playerpos.ID = id;
		playerpos.size = sizeof(SC_PlayerPos);
		playerpos.type = SC_PLAYERPOS;

		//EnterCriticalSection(&CPlayer::getInstance().client[id].cs);
		playerpos.move = CPlayer::getInstance().PlayerPos(key->movetype, id);
		//playerpos.move = CPlayer::getInstance().temporary(key->movetype, playerpos, id);
		CPlayer::getInstance().client[id].position = playerpos.move;
		//LeaveCriticalSection(&CPlayer::getInstance().client[id].cs);
		//printf("%d \n", id);
		for (int i = 0; i <= usernum; ++i)
			SendPacket(i, &playerpos);

		//SC_AvoidPlayer avoidplayer;
		//avoidplayer.size = sizeof(SC_AvoidPlayer);
		//avoidplayer.type = SC_AVOIDPLAYER;
		//avoidplayer.endtriger = CPlayer::getInstance().client[id].avoid;
		//SendPacket(id, &avoidplayer);
		//if (CPlayer::getInstance.[id].avoid){
		//	CPlayer::getInstance.[id].avoid = false;
		//}
		//if (PLAYING == m_state.gamestate)
		//	CountTime();

		break;
	}
	case CS_ROTATE:{

			m_playerrotate.dir = CPlayer::getInstance().client[id].direction;
			m_playerrotate.rad = CPlayer::getInstance().client[id].radian;
			CS_Rotate *rotate = reinterpret_cast<CS_Rotate*>(packet);
			SC_PlayerRotate playerrotate;

			//EnterCriticalSection(&cs);
			playerrotate.size = sizeof(SC_PlayerRotate);
			playerrotate.type = SC_PLAYERROTATE;
			playerrotate.ID = id;
			playerrotate.dir = rotate->dir;
			playerrotate.rad = rotate->rad;

			CPlayer::getInstance().client[id].direction = playerrotate.dir;
			CPlayer::getInstance().client[id].radian = playerrotate.rad;
			//LeaveCriticalSection(&cs);

			for (int i = 0; i <= usernum; ++i){
				SendPacket(i, &playerrotate);
			}	
			break;
	}
	case CS_READY:{
		CS_Ready* playerready = reinterpret_cast<CS_Ready*>(packet);
		readynum++;
		printf("%d \n", readynum);
		if (readynum == usernum){
			m_state = m_cGM->GameReady();
			readystate = true;
		}
		CPlayer::getInstance().client[id].previousframe = GetTickCount64();
		for (int i = 0; i <= usernum; ++i){
			SendPacket(i, &m_state);
		}
		break;
	}
	case CS_ITEMGET:{
		CS_ItemGet* itemget = reinterpret_cast<CS_ItemGet*>(packet);
		SC_Item itempacket;
		
		itempacket = m_cItem->AvoidItem(itemget->itemnum);
		AddTimer(itemget->itemnum, EVENT_RESP, 3000);
		//printf("충돌 %d \n", itemget->itemnum);
		for (int i = 0; i <= usernum; ++i)
			SendPacket(i, &itempacket);
		break;
	}
	case CS_COLLWALL:{
		CS_CollWall* wallcoll = reinterpret_cast<CS_CollWall*>(packet);
		SC_PlayerPos wallpos;
		
		//EnterCriticalSection(&cs);
		wallpos.ID = id;
		//wallpos.move = CPlayer::CollWall(id);
		if (CPlayer::getInstance().client[id].trigger == CPlayer::getInstance().ForwordTrigger
			|| CPlayer::getInstance().client[id].trigger == CPlayer::getInstance().FDecelTrigger){
			CPlayer::getInstance().client[id].trigger = CPlayer::getInstance().FcolTrigger;
			CPlayer::getInstance().client[id].presstime = 1.f;
		}
		else if (CPlayer::getInstance().client[id].trigger == CPlayer::getInstance().BackwordTrigger
			|| CPlayer::getInstance().client[id].trigger == CPlayer::getInstance().BDecelTrigger){
			CPlayer::getInstance().client[id].trigger = CPlayer::getInstance().BcolTrigger;
			CPlayer::getInstance().client[id].presstime = 1.f;
		}
		wallpos.move = CPlayer::getInstance().PlayerPos(COLLISION, id);
		wallpos.size = sizeof(SC_PlayerPos);
		wallpos.type = SC_PLAYERPOS;
		//LeaveCriticalSection(&cs);

		for (int i = 0; i <= usernum; ++i){
			SendPacket(i, &wallpos);
		}

		break;
	}
	case CS_COLLBOOSTER:{
		CS_CollBooster *collbooster = reinterpret_cast<CS_CollBooster*>(packet);

		//EnterCriticalSection(&cs);
		CPlayer::getInstance().client[id].direction = collbooster->direction;
		CPlayer::getInstance().client[id].boostertimming = true;
		CPlayer::getInstance().client[id].boostersort = BOOSTERCOLL;
		CPlayer::getInstance().client[id].accel = 100.0f;

		boosterdirection = collbooster->direction;
		AddTimer(id, EVENT_DURA, 0);
		//LeaveCriticalSection(&cs);

		break;
	}
	case CS_BOOSTERITEM:{
		CS_UseBooster* usebooster = reinterpret_cast<CS_UseBooster*>(packet);
		
		//EnterCriticalSection(&cs);
		CPlayer::getInstance().client[id].boostertimming = true;
		CPlayer::getInstance().client[id].boostersort = BOOSTERUSE;
		AddTimer(id, EVENT_DURA, 0);
		//LeaveCriticalSection(&cs);

		break;
	}case CS_CLIENTANI:{
		CS_ClientAni* anicl = reinterpret_cast<CS_ClientAni*>(packet);
		SC_ServerAni aniser;
		
		//EnterCriticalSection(&cs);
		aniser.size = sizeof(SC_ServerAni);
		aniser.type = SC_SERVERANI;
		aniser.aninum = anicl->aninum;
		aniser.ID = id;
		//LeaveCriticalSection(&cs);
		
		for (int i = 0; i < usernum; ++i)
			SendPacket(i, &aniser);
		break;
	}case CS_GOAL:{
		CS_Goal* goal = reinterpret_cast<CS_Goal*>(packet);
		SC_Ranking ranking;

		//EnterCriticalSection(&cs);
		rank++;
		ranking.size = sizeof(SC_Ranking);
		ranking.type = SC_RANKING;
		ranking.ranking = rank;
		ranking.ID = id;
		//LeaveCriticalSection(&cs);

		for (int i = 0; i < usernum; ++i)
			SendPacket(i, &ranking);

		if (1 == rank){
			for (int i = 0; i < usernum; ++i)
				AddTimer(i, OP_END, 10000);				//game종료 10초 카운트
														//timer를 for문에?!
		}
		++rank;
		break;
	}
	case CS_SHOT:{
		CS_Shoot *ammo = reinterpret_cast<CS_Shoot*>(packet);
		SC_Shoot ammoPos;

		ammoPos.ID = id;
		ammoPos.size = sizeof(SC_Shoot);
		ammoPos.type = SC_SHOT;
		ammoPos.AMMOpos = ammo->AMMOpos;
		ammoPos.ammonum = ammo->ammonum;

		for (int i = 0; i < usernum; ++i)
			SendPacket(i, &ammoPos);

		break;
	}
	case CS_AVOIDAMMO:{
		CS_Avoidammo *csAvoid = reinterpret_cast<CS_Avoidammo*>(packet);
		SC_Avoidammo scAvoid;

		scAvoid.id = id;
		scAvoid.size = sizeof(SC_Avoidammo);
		scAvoid.type = SC_AVOIDAMMO;
		scAvoid.ammonum = scAvoid.ammonum;
	
		for (int i = 0; i < usernum; ++i)
			SendPacket(i, &scAvoid);

		break;
	}
	}
	

}
void CServer::ItemRegen(int id){
	SC_Item itemregen;
	itemregen.isExist = true;
	itemregen.itemid = id;
	itemregen.size = sizeof(SC_Item);
	itemregen.type = SC_ITEMEXIST;

	printf("리스폰 %d \n", id);
	for (int i = 0; i < usernum; ++i){
		SendPacket(i, &itemregen);
	}
}
void CServer::DurationColl(int id){
	while (CPlayer::getInstance().client[id].boostertimming){
		if (BOOSTERUSE == CPlayer::getInstance().client[id].boostersort){
			SC_PlayerPos pos;
			//EnterCriticalSection(&cs);
			pos.move = CPlayer::getInstance().UseBooster(id);
			pos.ID = id;
			pos.size = sizeof(SC_PlayerPos);
			pos.type = SC_PLAYERPOS;
			CPlayer::getInstance().client[id].position = pos.move;
			//LeaveCriticalSection(&cs);
			for (int i = 0; i < usernum; ++i){
				//printf("%d \n", id);
				SendPacket(i, &pos);
			}
		}
		else if (BOOSTERCOLL == CPlayer::getInstance().client[id].boostersort){
			SC_PlayerPos pos;
			pos.move = CPlayer::getInstance().BoosterColl(id, boosterdirection);
			pos.ID = id;
			pos.size = sizeof(SC_PlayerPos);
			pos.type = SC_PLAYERPOS;
			CPlayer::getInstance().client[id].position = pos.move;
			for (int i = 0; i < usernum; ++i){
				SendPacket(i, &pos);
			}
		}
		else{
			SC_PlayerPos pos;
			//pos.move = CPlayer::getInstance().CollWall(id);
			pos.size = sizeof(SC_PlayerPos);
			pos.type = SC_PLAYERPOS;
			CPlayer::getInstance().client[id].position = pos.move;
			for (int i = 0; i < usernum; ++i){
				SendPacket(i, &pos);
			}
		}
	}
}
void CServer::EndDuration(int id){
	CPlayer::getInstance().client[id].boostertimming = false;

	SC_Endboost endboost;
	//EnterCriticalSection(&cs);
	endboost.ID = id;
	endboost.size = sizeof(SC_Endboost);
	endboost.type = SC_ENDBOOST;
	//LeaveCriticalSection(&cs);
	for (int i = 0; i < usernum; ++i){
		SendPacket(i, &endboost);
	}

}
void CServer::Timer_thread(){
	while (1){
		if (readystate){
			for (int i = 4; i > 0; --i){
				CTimer::TimeCount(1.f);
				m_state.timecount = i - 1;
				printf("%d \n", i - 1);
				if (i == 1)
				{
					m_state.gamestate = PLAYING;
					readystate = false;
					starttime = CTimer::SetTime();
					printf("playing\n");
				}
				for (int p = 0; p < ROOM_MAXUSER; ++p){
					SendPacket(p, &m_state);
				}
			}
		}
		else{
			//for (int i = 0; i < ITEM_NUM; ++i)
			//{
			//	if (!m_cItem->item[i].in_use){
			//		//EnterCriticalSection(&cs);
			//		m_cItem->item[i].in_use = CTimer::TimeCount(3.f);
			//		//LeaveCriticalSection(&cs);
			//		if (m_cItem->item[i].in_use)
			//			ItemRegen(i);
			//	}
			//}
			//for (int id = 0; id > usernum; ++id){
			//	if (CPlayer::getInstance().client[id].boostertimming){
			//		if (BOOSTERUSE == CPlayer::getInstance().client[id].boostersort){
			//			SC_PlayerPos pos;
			//			pos.move = CPlayer::getInstance().UseBooster(id);
			//			pos.ID = id;
			//			pos.size = sizeof(SC_PlayerPos);
			//			pos.type = SC_PLAYERPOS;
			//			CPlayer::getInstance().client[id].position = pos.move;
			//			for (int i = 0; i < usernum; ++i){
			//				SendPacket(i, &pos);
			//			}
			//		}
			//		else if (BOOSTERCOLL == CPlayer::getInstance().client[id].boostersort){
			//			SC_PlayerPos pos;
			//			pos.move = CPlayer::getInstance().BoosterColl(id, CPlayer::getInstance().client[id].direction);
			//			pos.ID = id;
			//			pos.size = sizeof(SC_PlayerPos);
			//			pos.type = SC_PLAYERPOS;
			//			printf("%.2f,%.2f,%.2f \n", pos.move.x, pos.move.y, pos.move.z);
			//			CPlayer::getInstance().client[id].position = pos.move;
			//			for (int i = 0; i < usernum; ++i){
			//				SendPacket(i, &pos);
			//			}
			//		}
			//		else{
			//			SC_PlayerPos pos;
			//			pos.move = CPlayer::getInstance().CollWall(id);
			//			CPlayer::getInstance().client[id].position = pos.move;
			//			for (int i = 0; i < usernum; ++i){
			//				SendPacket(i, &pos);
			//			}
			//		}
			//	}
			//}
			Sleep(1);
			if (timerQ.size() == 0){
				continue;
			}
			topEv = timerQ.top();
			while (topEv.time <= GetTickCount()){
				EnterCriticalSection(&cs);
				timerQ.pop();
				LeaveCriticalSection(&cs);
				Process_Event(topEv);
				if (timerQ.size() == 0)
					break;
				else{
					EnterCriticalSection(&cs);
					topEv = timerQ.top();
					LeaveCriticalSection(&cs);
				}
			}
		}
	}
	CTimer::TimeCount(0.03f);
}
void CServer::Accept_thread(){
	struct sockaddr_in listen_addr;
	struct sockaddr_in client_addr;

		SOCKET listen_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL,
			0, WSA_FLAG_OVERLAPPED);

		ZeroMemory(&listen_addr, sizeof(listen_addr));

		// listen
		listen_addr.sin_family = AF_INET;
		listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		listen_addr.sin_port = htons(SERVER_PORT);
		ZeroMemory(&listen_addr.sin_zero, sizeof(listen_addr.sin_zero));

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
			CPlayer::getInstance().client[id].sock = client_socket;

			printf("접속 %s \n ID : %d \n", inet_ntoa(client_addr.sin_addr), id);

			CreateIoCompletionPort(reinterpret_cast<HANDLE>(client_socket),
				hIOCP, id, 0);
			unsigned long recv_flag = 0;

			ret = WSARecv(client_socket, &CPlayer::getInstance().client[id].my_overapped.wsabuf, 1, NULL,
				&recv_flag, &CPlayer::getInstance().client[id].my_overapped.overapped, NULL);

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
			SC_InitPlayer initplayer;
			ZeroMemory(&initplayer, sizeof(initplayer));
			initplayer = CPlayer::getInstance().PlayerAccept(id);
			CPlayer::getInstance().client[id].position = initplayer.move;
			CPlayer::getInstance().client[id].in_use = true;
			CPlayer::getInstance().client[id].accel = initplayer.accel;
			CPlayer::getInstance().client[id].speed = initplayer.speed;
			CPlayer::getInstance().client[id].avoidspeed = 0;
			CPlayer::getInstance().client[id].presstime = 0;
			CPlayer::getInstance().client[id].boostertimming = false;
			CPlayer::getInstance().client[id].boostersort = 0;
			CPlayer::getInstance().client[id].boostertime = 0;
			CPlayer::getInstance().client[id].trigger = 0;
			

			for (int i = 0; i <= usernum; ++i){
				SendPacket(i, &initplayer);
			}

			// 이전에 들어와 있는 패킷을 다시 보내주는 작업
			for (int i = 0; i <= usernum; ++i){
				if (false == CPlayer::getInstance().client[i].in_use) continue;
				if (i == id)continue;
				initplayer.ID = i;
				initplayer.move = CPlayer::getInstance().client[i].position;
				SendPacket(id, &initplayer);
			}
			///////////////////////////////////////////////////// 유저 수 count다시 짜기
			// 스테이트 다시짜기
			//SC_State m_state;
			m_state = m_cGM->InitGameManager();

			for (int i = 0; i <= usernum; ++i)
				SendPacket(i, &m_state);
			
		}
}
void CServer::Process_Event(event_type nowevent){
	switch (nowevent.do_event){
	case EVENT_RESP:
	{
		OVERAPPED_EX *event_over = new OVERAPPED_EX;
		event_over->operation_type = OP_RESP;
		PostQueuedCompletionStatus(hIOCP, 1, nowevent.id,
			reinterpret_cast<LPOVERLAPPED>(event_over));
		break;
	}
	case EVENT_DURA:{
		OVERAPPED_EX *event_over = new OVERAPPED_EX;
		event_over->operation_type = OP_DURA;
		PostQueuedCompletionStatus(hIOCP, 1, nowevent.id,
			reinterpret_cast<LPOVERLAPPED>(event_over));
		break;
	}
	case EVENT_EDURA:{
		OVERAPPED_EX *event_over = new OVERAPPED_EX;
		event_over->operation_type = OP_EDURA;
		PostQueuedCompletionStatus(hIOCP, 1, nowevent.id,
			reinterpret_cast<LPOVERLAPPED>(event_over));
		break;
	}case EVENT_END:{
		OVERAPPED_EX *event_over = new OVERAPPED_EX;
		event_over->operation_type = OP_END;
		PostQueuedCompletionStatus(hIOCP, 1, nowevent.id,
			reinterpret_cast<LPOVERLAPPED>(event_over));
		break;
	}
	default:
		printf("Unknown Event Type Detected! \n");
	}
}
void CServer::CountTime(){								//계속해서 시간을 보내줌
	//while (PLAYING == m_state.gamestate){
		SC_Time time;
		time.size = sizeof(SC_Time);
		time.type = SC_TIME;
		time.time = CTimer::GetTime(starttime);
		
		//printf("%.2f\n", time.time);
		for (int i = 0; i < usernum; ++i){
			SendPacket(i, &time);
		}

}
void CServer::GameEnd(int id){
	SC_State gamestate;

	gamestate = m_cGM->GameEnd();

	SendPacket(id, &gamestate);
}
void CServer::Worker_thread(){

	while (true)
	{
		unsigned long io_size;
		unsigned long key;
		OVERAPPED_EX *over_ex;
		GetQueuedCompletionStatus(hIOCP, &io_size, &key, 
			reinterpret_cast<LPOVERLAPPED*> (&over_ex), INFINITE);
	
		// 에러로 인한 접속 종료
		if (0 == io_size){
			--usernum;
			SC_RemovePlayer repacket;
			repacket.ID = key;
			repacket.size = sizeof(repacket);
			repacket.type = SC_REMOVE_PLAYER;
			for (int i = 0; i < usernum; ++i){
				if (key == i) continue;
				if (false == CPlayer::getInstance().client[i].in_use) continue;
				SendPacket(i, &repacket);
			}
			printf("ID %d 접속 종료\n", key);
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
					buf += remain;
				}
			}
			unsigned long recv_flag = 0;
			WSARecv(CPlayer::getInstance().client[key].sock, &over_ex->wsabuf, 1, NULL,
				&recv_flag, &over_ex->overapped, NULL);
		}
		//SEND
 		else if (OP_SEND == over_ex->operation_type)
			delete over_ex;
		else if (OP_RESP == over_ex->operation_type){
			ItemRegen(key);
		}
		else if (OP_DURA == over_ex->operation_type){
			DurationColl(key);
		}
		else if (OP_EDURA == over_ex->operation_type){
			EndDuration(key);
		}
		else if (OP_END == over_ex->operation_type){
			GameEnd(key);
		}
	}
}
void CServer::CleanUp()
{
	WSACleanup();
}