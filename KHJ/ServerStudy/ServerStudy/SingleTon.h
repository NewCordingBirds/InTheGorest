#pragma once
#include <Windows.h>

template <class T>
class SingleTon
{
protected :
	SingleTon() {}
	~SingleTon() {}

public :

	static T& getInstance()
	{
		static T instance;
		return instance;
	}
};