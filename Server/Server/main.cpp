#include "Server.h"

void main(){
	vector <thread *> worker_threads;

	_wsetlocale(LC_ALL, L"korean");
	CServer::GetInstance()->NetworkInit();
	CServer::GetInstance()->InitGame();

	for (int i = 0; i < MAXUSER; ++i)
		CServer::GetInstance()->PlayerInit(i);

	hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	for (int i = 0; i < NUM_THREADS; ++i)
		worker_threads.push_back(new thread{ [](){
		CServer::GetInstance()->Worker_thread();
	} });

	thread acceptthread = thread{ [](){
		CServer::GetInstance()->Accept_thread();
	} };
	thread timerthread = thread{ [](){
		CServer::GetInstance()->Timer_thread();
	} };
	//thread collision = thread{ [](){
	//	CServer::GetInstance()->Coll_Thread();
	//} };

	while (true){ Sleep(1000); }

	for (auto th : worker_threads){
		th->join();
		delete th;
	}
	acceptthread.join();
	CServer::GetInstance()->CleanUp();

}