#include "Protocol.h"

class CMap{
public:
	CMap();
	~CMap();
public:
	map <int, COLLOBJ> mapblock;
public:
	void SetMap();
};