#pragma once

#include "Protocol.h"
using namespace std;

class CItem{

public:	
	CItem();
	~CItem();

	bool isExist;									//아이템 존재 여부

	void ItemSet(float x, float y, float z);
	void ItemAvoid(bool m_exist);

};