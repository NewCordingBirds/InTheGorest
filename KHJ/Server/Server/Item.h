#pragma once

#include "Protocol.h"
using namespace std;

class CItem{

public:	
	CItem();
	~CItem();

	bool isExist;									//������ ���� ����

	void ItemSet(int x, int y, int z);
	void ItemAvoid(bool m_exist);

};