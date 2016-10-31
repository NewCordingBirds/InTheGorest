#include "stdafx.h"
#include "EnemyObserver.h"


CEnemyObserver::CEnemyObserver()
{
	//ZeroMemory(&tInitPlayer, sizeof(SC_InitPlayer) * MAX_USER_NUM);
	//for (int i = 0; i < MAX_USER_NUM; ++i)
	//	tInitPlayer[i].ID = -1;

	//ZeroMemory(&tPlayerPos, sizeof(SC_PlayerPos) * MAX_USER_NUM);
	//ZeroMemory(&tPlayertotate, sizeof(CS_Rotate)* MAX_USER_NUM);
}

CEnemyObserver::~CEnemyObserver()
{

}

CEnemyObserver* CEnemyObserver::Create()
{
	CEnemyObserver* pObserver = new CEnemyObserver;

	//for (int i = 0; i < MAX_USER_NUM; ++i)
	//	pObserver->Update(CInfoSubject::DATA_TYPE_ENEMY_INITPLAYER, i);


	return pObserver;
}

void CEnemyObserver::Update(int _iMessage,
							UINT _nIdx /*= 0*/)
{
	vector<void*>* pData = m_pInfoSubject->GetDatalist(_iMessage);
	//NULL_CHECK(pData);
	if (pData == NULL)
		return;

	//switch (_iMessage)
	//{
	//case CInfoSubject::DATA_TYPE_ENEMY_INITPLAYER:
	//	tInitPlayer[_nIdx] = *((SC_InitPlayer*)((*pData)[_nIdx]));
	//	break;

	//case CInfoSubject::DATA_TYPE_ENEMY_PLAYERPOS:
	//	tPlayerPos[_nIdx] = *((SC_PlayerPos*)((*pData)[_nIdx]));
	//	break;
	//case CInfoSubject::DATA_TYPE_ENEMY_PLAYERROTATE:
	//	tPlayertotate[_nIdx] = *((SC_PlayerRotate*)((*pData)[_nIdx]));
	//	break;
	//default:
	//	break;
	//}
}