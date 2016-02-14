#pragma once

#include <chrono>
#include <ctime>
#include <string>
#include "SingleTon.h"

#define CLOCK					Clock::getInstance()
#define NOW_TICK				CLOCK.systemTick
#define NOW_STRING				CLOCK.nowTime

#define TICK_MIN				(60)
#define TICK_HOUR				(TICK_MIN * 60)
#define TICK_DAY				(TICK_HOUR * 24)

#define TICK_TO_MIN(x)			(x / TICK_MIN)
#define MIN_TO_TICK(x)			(x * TICK_MIN)
#define TICK_TO_HOUR(x)			(x / TICK_HOUR)
#define HOUR_TO_TICK(x)			(x * TICK_HOUR)
#define TICK_TO_DAY(x)			(x / TICK_DAY)
#define DAY_TO_TICK(x)			(x * TICK_DAY)

typedef enum{
	DAY_SUNDAY		= 0,
	DAY_MONDAY		= 1,
	DAY_TUESDAY		= 2,
	DAY_WEDNESDAY	= 3,
	DAY_THURSDAY	= 4,
	DAY_FRIDAY		= 5,
	DAY_SATURDAY	= 6,
}DayOfTheWeek;

#define DATETIME_FORMAT			L"D%y-%m-%d / T%H:%M:%S"
#define DATE_FORMAT				L"%y-%m-%d"
#define TIME_FORMAT				L"%H:%m:%S"
#define DB_TIME_FORMAT			L"%4d-%2d-%2d %2d:%2d:%2d"

using namespace std::chrono;
using namespace std;

typedef system_clock::time_point timePoint;

class Clock : public SingleTon < Clock >
{
	time_t serverStartTick_;

	wstring tickTostr(time_t tick, WCHAR *fmt);

public:
	Clock();
	~Clock();

	time_t serverStartTick();
	time_t systemTick();
	time_t strToTick(wstring str, WCHAR *fmt = DB_TIME_FORMAT);

	wstring nowTime(WCHAR * fmt = TIME_FORMAT);
	wstring nowDayTime(WCHAR *fmt = DATETIME_FORMAT);
	wstring nowTimeWithMilliSec(WCHAR *fmt = DATETIME_FORMAT);
	

	wstring today();
	wstring tomorrow();
	wstring yesterday();

	DayOfTheWeek todayOfTheWeek();
};