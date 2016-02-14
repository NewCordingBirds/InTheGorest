#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <set>
#include <WinSock2.h>
#include <Windows.h>
#include <queue>

using namespace std;

#include "protocol.h"

const int NUM_THREADS = 6;
const int STARTING_X = 4;
const int STARTING_Y = 4;

HANDLE hIOCP;

const int OP_SEND = 0;
const int OP_RECV = 1;
const int OP_MOVE = 2;

struct OVERAPPED_EX {
	WSAOVERLAPPED overapped;
	int operation_type;
	WSABUF wsabuf;
	char IOCPbuf[MAX_BUFF_SIZE];
	char PacketBuf[MAX_PACKET_SIZE];
	unsigned int prev_received;
	unsigned int curr_packet_size;
};

struct PLAYER {
	int x;
	int y;
	SOCKET sock;
	bool in_use;
	OVERAPPED_EX my_overapped;
	set<int> view_list;
	mutex	vl_lock;
};

struct MONSTER {
	int x;
	int y;
	bool is_alive;
	bool is_awaken;
	//OVER??
	// VIEWLIST???
};

const int EVENT_MOVE = 0;
const int EVENT_CHASE = 1;
const int EVENT_ATTACK = 2;
const int EVENT_HEAL = 3;

struct event_type {
	int obj_id;
	unsigned int wakeup_time;
	int do_event;
	int target_id;
};

class mycomparison
{
	bool reverse;
public:
	mycomparison() {}
	bool operator() (const event_type lhs, const event_type rhs) const
	{
		return (lhs.wakeup_time > rhs.wakeup_time);
	}
};

priority_queue<event_type, vector<event_type>, mycomparison> timer_queue;
mutex tq_lock;

PLAYER players[10];
MONSTER monsters[NUM_OF_NPC];

bool in_range(int me, int object)
{
	int x_me, x_ob;
	if (me < NPC_START) x_me = players[me].x;
	else x_me = monsters[me - NPC_START].x;
	if (object < NPC_START) x_ob = players[object].x;
	else x_ob = monsters[object - NPC_START].x;
	int x_dist = (x_me - x_ob)*(x_me - x_ob);

	int y_me, y_ob;
	if (me < NPC_START) y_me = players[me].y;
	else y_me = monsters[me - NPC_START].y;
	if (object < NPC_START) y_ob = players[object].y;
	else y_ob = monsters[object - NPC_START].y;
	int y_dist = (y_me - y_ob)*(y_me - y_ob);

	if ((x_dist + y_dist) < VIEW_RADIUS * VIEW_RADIUS) return true;
	else return false;
}

void error_display(char *msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("%s :", msg);
	wprintf(L"에러%s\n", lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int curr_time()
{
	return GetTickCount();
}


void PlayerInit(int id)
{
	ZeroMemory(&players[id].my_overapped, sizeof(OVERAPPED_EX));
	players[id].in_use = false;
	players[id].my_overapped.operation_type = OP_RECV;
	players[id].my_overapped.wsabuf.buf = players[id].my_overapped.IOCPbuf;
	players[id].my_overapped.wsabuf.len = sizeof(players[id].my_overapped.IOCPbuf);
	players[id].view_list.clear();
}

void SendPacket(int id, void *packet)
{
	int packet_size = reinterpret_cast<unsigned char *>(packet)[0];

	OVERAPPED_EX *send_over = new OVERAPPED_EX;
	ZeroMemory(send_over, sizeof(OVERAPPED_EX));
	send_over->operation_type = OP_SEND;
	send_over->wsabuf.buf = send_over->IOCPbuf;
	send_over->wsabuf.len = packet_size;
	unsigned long io_size;

	memcpy(send_over->IOCPbuf, packet, packet_size);

	WSASend(players[id].sock, &send_over->wsabuf, 1,
		&io_size, NULL, &send_over->overapped, NULL);
}

void AddTimer(int obj_id, int do_event, int wakeup_time)
{
	event_type new_event;
	new_event.do_event = do_event;
	new_event.obj_id = obj_id;
	new_event.wakeup_time = wakeup_time + curr_time();
	tq_lock.lock();
	timer_queue.push(new_event);
	tq_lock.unlock();
}

void awake_monster(int obj_id)
{
	monsters[obj_id - NPC_START].is_awaken = true;
	AddTimer(obj_id, EVENT_MOVE, 1000);
}

void ProcessPacket(char *packet, int id){
	int x = players[id].x;
	int y = players[id].y;
	switch (packet[1])
	{
	case CS_UP: y--; break;
	case CS_DOWN: y++; break;
	case CS_LEFT: x--; break;
	case CS_RIGHT: x++; break;
	default: cout << "INVALID PACKET FROM [" << id << "]\n"; exit(-1);
	}
	if (x < 0) x = 0; else if (x >= BOARD_WIDTH) x = BOARD_WIDTH - 1;
	if (y < 0) y = 0; else if (y >= BOARD_HEIGHT) y = BOARD_HEIGHT - 1;
	players[id].x = x;
	players[id].y = y;

	sc_packet_pos pos_packet;
	pos_packet.size = sizeof(pos_packet);
	pos_packet.type = SC_POS;
	pos_packet.id = id;
	pos_packet.x = x;
	pos_packet.y = y;

	SendPacket(id, &pos_packet);

	players[id].vl_lock.lock();
	set<int> old_list = players[id].view_list;
	players[id].vl_lock.unlock();
	set<int> new_list;
	for (int i = 0; i < NPC_START + NUM_OF_NPC; ++i){
		if (i == MAX_USER) i = NPC_START;
		if (i < MAX_USER) {
			if (false == players[i].in_use) continue;
		}
		else {
			if (false == monsters[i - NPC_START].is_alive) continue;
		}
		if (id == i) continue;
		if (!in_range(id, i)) continue;
		new_list.insert(i);
	}

	for (int obj : new_list) {
		if (0 == old_list.count(obj)) {
			players[id].vl_lock.lock();
			players[id].view_list.insert(obj);
			players[id].vl_lock.unlock();
			sc_packet_put_player put_player_packet;
			put_player_packet.id = obj;
			put_player_packet.size = sizeof(sc_packet_put_player);
			put_player_packet.type = SC_PUT_PLAYER;
			if (obj < NPC_START) {
				put_player_packet.x = players[obj].x;
				put_player_packet.y = players[obj].y;
			}
			else {
				put_player_packet.x = monsters[obj - NPC_START].x;
				put_player_packet.y = monsters[obj - NPC_START].y;
			}
			SendPacket(id, &put_player_packet);

			if (obj >= NPC_START) continue;

			players[obj].vl_lock.lock();
			if (0 != players[obj].view_list.count(id)) {
				SendPacket(obj, &pos_packet);
			}
			else {
				players[obj].view_list.insert(id);
				sc_packet_put_player put_player_packet;
				put_player_packet.id = id;
				put_player_packet.size = sizeof(sc_packet_put_player);
				put_player_packet.type = SC_PUT_PLAYER;
				put_player_packet.x = players[id].x;
				put_player_packet.y = players[id].y;
				SendPacket(obj, &put_player_packet);
			}
			players[obj].vl_lock.unlock();
		}
		else {  // 내 뷰리스트에 obj가 이미 있었다면
			if (obj >= NPC_START) continue;

			players[obj].vl_lock.lock();
			if (0 != players[obj].view_list.count(id)) {
				SendPacket(obj, &pos_packet);
			}
			else {
				players[obj].view_list.insert(id);
				sc_packet_put_player put_player_packet;
				put_player_packet.id = id;
				put_player_packet.size = sizeof(sc_packet_put_player);
				put_player_packet.type = SC_PUT_PLAYER;
				put_player_packet.x = players[id].x;
				put_player_packet.y = players[id].y;
				SendPacket(obj, &put_player_packet);
			}
			players[obj].vl_lock.unlock();
		}
	}  // 나의 새로운 뷰리스트에 있는 object처리

	for (int obj : old_list) {
		// new_list에 존재하지 않는 객체 처리
		if (0 != new_list.count(obj)) continue;
		sc_packet_remove_player remove_player_packet;
		remove_player_packet.id = obj;
		remove_player_packet.size = sizeof(sc_packet_remove_player);
		remove_player_packet.type = SC_REMOVE_PLAYER;
		SendPacket(id, &remove_player_packet);
		players[id].vl_lock.lock();
		players[id].view_list.erase(obj);
		players[id].vl_lock.unlock();

		if (obj >= NPC_START) continue;

		players[obj].vl_lock.lock();
		if (0 == players[obj].view_list.count(id)) {
			players[obj].vl_lock.unlock();
			continue;
		}
		players[obj].view_list.erase(id);
		remove_player_packet.id = id;
		SendPacket(obj, &remove_player_packet);
		players[obj].vl_lock.unlock();
	}


	//	for (int i = 0; i<MAX_USER; ++i)
	//		if (true == players[i].in_use)
	//			SendPacket(i, &pos_packet);
}

int GetNewClient_ID()
{
	for (int i = 0; i < MAX_USER; ++i)
		if (players[i].in_use == false){
			PlayerInit(i);
			return i;
		}
	cout << "USER FULL ERROR!!";
	exit(-1);
	return -1;
}

void accept_thread()
{
	struct sockaddr_in listen_addr;
	struct sockaddr_in client_addr;

	SOCKET listen_socket = WSASocket(AF_INET, SOCK_STREAM,
		IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	ZeroMemory(&listen_addr, sizeof(listen_addr));
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	listen_addr.sin_port = htons(MY_SERVER_PORT);
	ZeroMemory(&listen_addr.sin_zero, 8);
	int ret = ::bind(listen_socket,
		reinterpret_cast<sockaddr *>(&listen_addr),
		sizeof(listen_addr));
	if (SOCKET_ERROR == ret) {
		error_display("BIND", WSAGetLastError());
		exit(-1);
	}
	ret = listen(listen_socket, 10);
	if (SOCKET_ERROR == ret) {
		error_display("LISTEN", WSAGetLastError());
		exit(-1);
	}
	while (true) {
		int addr_size = sizeof(client_addr);
		SOCKET client_socket = WSAAccept(listen_socket,
			reinterpret_cast<sockaddr *>(&client_addr),
			&addr_size, NULL, NULL);
		if (INVALID_SOCKET == client_socket) {
			error_display("ACCEPT", WSAGetLastError());
			exit(-1);
		}
		int id = GetNewClient_ID();
		players[id].sock = client_socket;
		players[id].x = STARTING_X;
		players[id].y = STARTING_Y;
		CreateIoCompletionPort(
			reinterpret_cast<HANDLE>(client_socket),
			hIOCP, id, 0);
		unsigned long recv_flag = 0;
		ret = WSARecv(client_socket,
			&players[id].my_overapped.wsabuf, 1, NULL, &recv_flag,
			&players[id].my_overapped.overapped, NULL);
		if (SOCKET_ERROR == ret) {
			int err_code = WSAGetLastError();
			if (WSA_IO_PENDING != err_code) {
				error_display("ACCEPT(WSARecv):", err_code);
				exit(-1);
			}
		}
		// 패킷 담기
		sc_packet_put_player put_player_packet;
		put_player_packet.id = id;
		put_player_packet.type = SC_PUT_PLAYER;
		put_player_packet.size = sizeof(put_player_packet);
		put_player_packet.x = players[id].x;
		put_player_packet.y = players[id].y;
		SendPacket(id, &put_player_packet);

		for (int i = 0; i < MAX_USER; ++i){
			if (false == players[i].in_use) continue;
			if (id == i) continue;
			if (!in_range(id, i)) continue;
			players[i].vl_lock.lock();
			players[i].view_list.insert(id);
			players[i].vl_lock.unlock();
			SendPacket(i, &put_player_packet);
		}
		for (int i = 0; i < NPC_START + NUM_OF_NPC; ++i){
			if (i == MAX_USER) i = NPC_START;
			if (i < MAX_USER) {
				if (false == players[i].in_use) continue;
			}
			else {
				if (false == monsters[i - NPC_START].is_alive) continue;
			}

			if (id == i) continue;
			if (!in_range(id, i)) continue;

			if (i >= NPC_START) {
				if (false == monsters[i - NPC_START].is_awaken)
					awake_monster(i);
			}
			players[id].vl_lock.lock();
			players[id].view_list.insert(i);
			players[id].vl_lock.unlock();
			put_player_packet.id = i;
			if (i < MAX_USER) {
				put_player_packet.x = players[i].x;
				put_player_packet.y = players[i].y;
			}
			else {
				put_player_packet.x = monsters[i - NPC_START].x;
				put_player_packet.y = monsters[i - NPC_START].y;
			}
			SendPacket(id, &put_player_packet);
		}
		players[id].in_use = true;
	}
}

void NetworkInit()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
}

void Initialize_NPC()
{
	for (int i = 0; i < NUM_OF_NPC; ++i){
		monsters[i].is_alive = true;
		monsters[i].is_awaken = false;
		//monsters[i].x = rand() % BOARD_WIDTH;
		//monsters[i].y = rand() % BOARD_HEIGHT;
		monsters[i].x = i % BOARD_WIDTH;
		monsters[i].y = i % BOARD_HEIGHT;
		// AddTimer(i + NPC_START, EVENT_MOVE, 1000);
	}
}

const int HEARTBEAT_DURATION = 1000;
#include <chrono>
using namespace chrono;

void heart_beat(int id)
{
	if (false == monsters[id - NPC_START].is_alive) return;
	int new_x = monsters[id - NPC_START].x + (rand() % 3) - 1;
	int new_y = monsters[id - NPC_START].y + (rand() % 3) - 1;
	// if (false == CAN_GO(new_x, new_y)) return;

	set <int> old_view;
	for (int i = 0; i < MAX_USER; ++i)
	{
		if (false == players[i].in_use) continue;
		if (in_range(i, id)) old_view.insert(i);
	}

	monsters[id - NPC_START].x = new_x;
	monsters[id - NPC_START].y = new_y;

	set <int> new_view;
	for (int i = 0; i < MAX_USER; ++i)
	{
		if (false == players[i].in_use) continue;
		if (in_range(i, id)) new_view.insert(i);
	}

	sc_packet_put_player pp_packet;
	pp_packet.id = id;
	pp_packet.size = sizeof(sc_packet_put_player);
	pp_packet.type = SC_PUT_PLAYER;
	pp_packet.x = new_x;
	pp_packet.y = new_y;

	sc_packet_pos pos_packet;
	pos_packet.id = id;
	pos_packet.size = sizeof(sc_packet_pos);
	pos_packet.type = SC_POS;
	pos_packet.x = new_x;
	pos_packet.y = new_y;

	for (int i : old_view) {
		if (new_view.count(i)) {
			// old에도 있고 new에도 있다.
			players[i].vl_lock.lock();
			if (players[i].view_list.count(id) == 0) {
				players[i].view_list.insert(id);
				SendPacket(i, &pp_packet);
			}
			else {
				SendPacket(i, &pos_packet);
			}
			players[i].vl_lock.unlock();
		}
		else {
			players[i].vl_lock.lock();
			if (players[i].view_list.count(id) != 0){
				sc_packet_remove_player rem_packet;
				rem_packet.type = SC_REMOVE_PLAYER;
				rem_packet.size = sizeof(sc_packet_remove_player);
				rem_packet.id = id;
				SendPacket(i, &rem_packet);
				players[i].view_list.erase(id);
			}
			players[i].vl_lock.unlock();
		}
	}


	for (int i : new_view){
		if (0 != old_view.count(i)) continue;

		players[i].vl_lock.lock();
		if (0 == players[i].view_list.count(id)){
			SendPacket(i, &pp_packet);
			players[i].view_list.insert(id);
		}
		else SendPacket(i, &pos_packet);
		players[i].vl_lock.unlock();
	}
	// ACCEPT하고 PLAYER MOVE할 때도 몬스터추가 필요
}

void hb_thread()
{
	while (true){
		auto start_time = high_resolution_clock::now();
		for (int i = NPC_START; i < NPC_START + NUM_OF_NPC; ++i)
			heart_beat(i);
		auto duration = high_resolution_clock::now() - start_time;
		Sleep(HEARTBEAT_DURATION -
			duration_cast<milliseconds>(duration).count());
	}
}

void Process_Event(event_type curr_event)
{
	switch (curr_event.do_event) {
	case EVENT_MOVE:
	{
		OVERAPPED_EX *event_over = new OVERAPPED_EX;
		event_over->operation_type = OP_MOVE;
		PostQueuedCompletionStatus(hIOCP, 1, curr_event.obj_id,
			reinterpret_cast<LPOVERLAPPED>(event_over));
		//heart_beat(curr_event.obj_id);
		//AddTimer(curr_event.obj_id, EVENT_MOVE, 1000);
		break;
	}
	default:
		cout << "Unknown Event Type Detected!\n";
		exit(-1);
	}
}

void Timer_Thread()
{
	while (true) {
		Sleep(1);
		tq_lock.lock();
		if (timer_queue.size() == 0) {
			tq_lock.unlock();
			continue;
		}
		event_type top_event = timer_queue.top();
		int temp = curr_time();
		while (top_event.wakeup_time <= curr_time()) {
			timer_queue.pop();
			tq_lock.unlock();
			Process_Event(top_event);
			tq_lock.lock();
			if (timer_queue.size() == 0) {
				goto loop_end;
			}
			top_event = timer_queue.top();
		}
	loop_end:
		tq_lock.unlock();
	}
}


void worker_thread()
{
	while (true)
	{
		unsigned long io_size;
		unsigned long key;
		OVERAPPED_EX *over_ex;
		GetQueuedCompletionStatus(hIOCP, &io_size, &key,
			reinterpret_cast<LPOVERLAPPED *>(&over_ex), INFINITE);
		// ERROR
		// 접속종료
		if (io_size == 0) {
			closesocket(players[key].sock);
			sc_packet_remove_player rem_packet;
			rem_packet.id = key;
			rem_packet.size = sizeof(rem_packet);
			rem_packet.type = SC_REMOVE_PLAYER;
			for (int i = 0; i < MAX_USER; ++i){
				if (key == i) continue;
				if (false == players[i].in_use) continue;
				SendPacket(i, &rem_packet);
			}
			players[key].in_use = false;
			continue;
		}
		if (over_ex->operation_type == OP_RECV){
			// RECV
			int rest_size = io_size;
			char *buf = over_ex->IOCPbuf;
			int packet_size = over_ex->curr_packet_size;
			while (0 < rest_size) {
				if (0 == packet_size) packet_size = buf[0];
				int remain = packet_size - over_ex->prev_received;
				if (remain > rest_size) {
					// 패킷 만들기에는 부족하다.
					memcpy(over_ex->PacketBuf + over_ex->prev_received,
						buf, rest_size);
					over_ex->prev_received += rest_size;
					break;
				}
				else {
					// 패킷을 만들 수 있다.
					memcpy(over_ex->PacketBuf + over_ex->prev_received,
						buf, remain);
					ProcessPacket(over_ex->PacketBuf, key);
					rest_size -= remain;
					packet_size = 0;
					over_ex->prev_received = 0;
				}
			}
			over_ex->curr_packet_size = packet_size;
			unsigned long recv_flag = 0;
			WSARecv(players[key].sock,
				&over_ex->wsabuf, 1, NULL,
				&recv_flag, &over_ex->overapped, NULL);
		}
		else if (over_ex->operation_type == OP_SEND) {
			delete over_ex;
			// SEND
		}
		else if (over_ex->operation_type == OP_MOVE) {
			heart_beat(key);
			AddTimer(key, EVENT_MOVE, 1000);
		}
		else {
			cout << "Unknown Operation Type in worker thread!\n";
			exit(-1);
		}
	}

}


int main(){
	vector <thread *> worker_threads;

	_wsetlocale(LC_ALL, L"korean");
	NetworkInit();
	for (int i = 0; i < MAX_USER; ++i) PlayerInit(i);

	hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	for (int i = 0; i < NUM_THREADS; ++i)
		worker_threads.push_back(new thread{ worker_thread });

	Initialize_NPC();

	// thread heart_beat_thread = thread{ hb_thread };

	thread timer_thread = thread{ Timer_Thread };

	thread accept_threads = thread{ accept_thread };

	while (true) {
		Sleep(1000);
		// if (g_server_shutdown == true) break;
	}
}