#pragma once

#include "Protocol.h"
//#include "Timer.h"					// ���� Ÿ�̸�

using namespace std;

static class CGameManager{

protected:
	CGameManager();
	~CGameManager();

public :	
	int m_room;
	
	SC_State GameState(SC_State, int);		//���� ��ŸƮ ����
	// ���� �ο��� 

	static CGameManager& getInstance()
	{
		static CGameManager instance;
		return instance;
	}
};