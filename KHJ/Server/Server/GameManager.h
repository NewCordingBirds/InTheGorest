#pragma once

#include "Protocol.h"
//#include "Timer.h"					// ���� Ÿ�̸�

using namespace std;

class CGameManager{


public:
	CGameManager();
	~CGameManager();

	SC_State GameState(SC_State, int);		//���� ��ŸƮ ����
	// ���� �ο��� 

	static CGameManager& getInstance()
	{
		static CGameManager instance;
		return instance;
	}

	int m_room;
};