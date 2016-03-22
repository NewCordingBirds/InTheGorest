#pragma once

#include "Protocol.h"
using namespace std;

class CItem{

private:
	CItem();
	~CItem();

public:
	bool isExist;									//아이템 존재 여부

	void ItemSet(int x, int y, int z);
	void ItemAvoid(bool m_exist);

};