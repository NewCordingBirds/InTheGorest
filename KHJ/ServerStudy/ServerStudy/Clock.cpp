#include <array>
#include <stdio.h>
#include "Clock.h"
#include <wchar.h>

Clock::Clock(){
	serverStartTick_ = this->systemTick();
}
Clock::~Clock(){

}
time_t Clock::strToTick(wstring str, WCHAR *fmt){
	int year = 0;
	int month = 0;
	int day = 0;
	int hour = 0;
	int minute = 0;
	int second = 0;

	swscanf_s(str.c_str(), fmt, &year, &month, &day, &hour, &minute, &second);

	tm time = { second, minute, hour, day, month - 1, year - 1900 };

	return mktime(&time);
}

time_t Clock::serverStartTick(){
	return serverStartTick_;
}
time_t Clock::systemTick(){
	return system_clock::to_time_t(system_clock::now());
}
wstring Clock::tickTostr(time_t tick, WCHAR *fmt){
	array<WCHAR, 128> timeStr;

	tm time;
	localtime_s(&time, &tick);
	wcsftime(timeStr.data(), timeStr.size(), fmt, &time);

	return timeStr.data();
}

wstring Clock::nowTime(WCHAR *fmt)
{
	return this->tickTostr(this->systemTick(), fmt);
}
wstring Clock::nowDayTime(WCHAR *fmt){
	return this->tickTostr(this->systemTick(), fmt);
}
wstring Clock::nowTimeWithMilliSec(WCHAR *fmt){
#if 0
	timePoint now = system_clock::now();
	timePoint oldSecond = system_clock::from_time_t(this->systemTick());
	
	duration<double> milliSeconds = now - oldSecond;
	array<WCHAR, sizeof(double)>milliStr;
	swprintf(milliStr, L"%03d", (int)(milliSeconds.count() * 1000));
#else
	high_resolution_clock::time_point point = high_resolution_clock::now();
	milliseconds ms = duration_cast<milliseconds>(point.time_since_epoch());

	seconds s = duration_cast<seconds>(ms);
	time_t t = s.count();
	std::size_t fractionalSeconds = ms.count() % 1000;
	array<WCHAR, sizeof(double)>milliStr;
	sprintf((char*)&milliStr, "%03d", (int)fractionalSeconds);

#endif
	wstring timeString = this->tickTostr(this->systemTick(), fmt);
	timeString += L".";
	timeString += milliStr.data();
	return timeString;
}

wstring Clock::today(){
	return this->tickTostr(this->systemTick(), DATE_FORMAT);
}

wstring Clock::tomorrow(){
	return this->tickTostr(this->systemTick() + DAY_TO_TICK(1), DATE_FORMAT);
}

wstring Clock::yesterday(){
	return this->tickTostr(this->systemTick() - DAY_TO_TICK(1), DATE_FORMAT);
}

DayOfTheWeek Clock::todayOfTheWeek(){
	tm time;
	time_t tick = this->systemTick();
	localtime_s(&time, &tick);
	return (DayOfTheWeek)time.tm_wday;
}