#include <iostream>
#include "MemoryPool.h"
#include "SingleTon.h"
#include "Clock.h"

using namespace std;

//class CDataMP : public CMemoryPool < CDataMP > {
//private:
//	char a[1024];
//	char b[1024];
//
//};
//void main(){
//	//CDataMP 
//	for (int i = 0; i < 1000000; i++){
//		CDataMP *pData = new CDataMP();
//
//		delete pData;
//	}
//
//
//}
int main(int argc, WCHAR* argv[]){
	printf("yesterday %ws \n", CLOCK.yesterday().c_str());
	printf("today %ws, \n", CLOCK.today().c_str());
	printf("tomorrow %ws, \n", CLOCK.tomorrow().c_str());
	printf("today is [%d]week of the day \n", CLOCK.todayOfTheWeek());

	printf("system = %llu\n", NOW_TICK());

	for (int i = 0; i < 10; ++i){
		printf("%wd \n", CLOCK.nowTimeWithMilliSec().c_str());
	}
	return 0;
}