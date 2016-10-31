#include "stdafx.h"
#include "PlayerObserver.h"


CPlayerObserver::CPlayerObserver()
: m_nHp(MAX_HP)
, m_nRecoterNum(0)
, m_nBulletNum(MAX_BULLET_NUM)

, m_fCurMapLength(0.0f)
{
	//ZeroMemory(&tInitPlayer, sizeof(SC_InitPlayer));
	//tInitPlayer.ID	= -1;

	//ZeroMemory(&tPlayerPos, sizeof(SC_PlayerPos));
	ZeroMemory(&m_tKey, sizeof(CS_Key));
	ZeroMemory(&m_tRotate, sizeof(CS_Rotate));
}

CPlayerObserver::~CPlayerObserver()
{

}

CPlayerObserver* CPlayerObserver::Create()
{
	CPlayerObserver* pObserver = new CPlayerObserver;
//	pObserver->Update(CInfoSubject::DATA_TYPE_PLAYER_INITPLAYER);
// 	pObserver->Update(CInfoSubject::DATA_TYPE_PLAYER_PLAYERPOS);
// 	pObserver->Update(CInfoSubject::DATA_TYPE_PLAYER_KEY);
// 	pObserver->Update(CInfoSubject::DATA_TYPE_PLAYER_ROTATE);

	return pObserver;
}

void CPlayerObserver::Update(int _iMessage,
							UINT _nIdx /*= 0*/)
{
	vector<void*>* pData = m_pInfoSubject->GetDatalist(_iMessage);
	//NULL_CHECK(pData);
	if (pData == NULL)
		return;

	switch (_iMessage)
	{
	//case CInfoSubject::DATA_TYPE_PLAYER_INITPLAYER:
	//	tInitPlayer = *((SC_InitPlayer*)((*pData)[_nIdx]));
	//	break;

	//case CInfoSubject::DATA_TYPE_PLAYER_PLAYERPOS:
	//	tPlayerPos = *((SC_PlayerPos*)((*pData)[_nIdx]));
	//	break;

	case CInfoSubject::DATA_TYPE_PLAYER_KEY :
		m_tKey = *((CS_Key*)((*pData)[_nIdx]));
		break;

	case CInfoSubject::DATA_TYPE_PLAYER_ROTATE:
		m_tRotate = *((CS_Rotate*)((*pData)[_nIdx]));
		break;

	case CInfoSubject::DATA_TYPE_PLAYER_SHOT:
		m_tShoot = *((CS_Shoot*)((*pData)[_nIdx]));
		break;

	case CInfoSubject::DATA_TYPE_PLAYER_BULLET:
		m_nBulletNum = *((WORD*)((*pData)[_nIdx]));
		break;

	case CInfoSubject::DATA_TYPE_PLAYER_HP:
		m_nHp = *((WORD*)((*pData)[_nIdx]));
		break;

	case CInfoSubject::DATA_TYPE_PLAYER_RECOSTER:
		m_nRecoterNum = *((WORD*)((*pData)[_nIdx]));
		break;

	case CInfoSubject::DATA_TYPE_PLAYER_CUR_MAPLENGHT :
		m_fCurMapLength = *((float*)((*pData)[_nIdx]));
		break;

	default:
		break;
	}
}