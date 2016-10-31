#include "stdafx.h"
#include "InMapMgr.h"

#include "Player.h"
#include "StaticCamera.h"
#include "Enemy.h"
#include "ObjMgr.h"

#include "ServerMgr.h"

#include "Grade.h"
#include "Notify.h"
#include <conio.h>

IMPLEMENT_SINGLETON(CInMapMgr)


CInMapMgr::CInMapMgr()
: m_eInMapState(INMAP_STATE_LOADING)
, m_eInMapProcess(INMAP_PROCESS_IDLE)

, m_nCurLap(1)

, m_pServerMgr(CServer::GetInstance())
, m_pObjMgr(CObjMgr::GetInstance())
, m_pPlayer(NULL)

, m_pUI_Grade(NULL)
, m_pUI_Notify(NULL)
{
	for (int i = 0; i < MAX_USER_NUM; ++i)
		m_pEnemy[i] = NULL;

	m_tItem.isExist = false;
	m_tItem.itemid = -1;
}

CInMapMgr::~CInMapMgr()
{
	ClearItem();
}

void CInMapMgr::Init()
{
	//// EnemyObserver -------------
	//m_pEnemyObserver = CEnemyObserver::Create();
	//CInfoSubject::GetInstance()->Subscribe(m_pEnemyObserver);

	//m_eInMapState = INMAP_STATE_LOADING;
}

void CInMapMgr::Update()
{
	// (수정) 야메리카노
	//CheckInMapState();


	switch (m_eInMapState)
	{
	case INMAP_STATE_LOADING:
		CheckEnemyInitAndCreate();
		CheckInMapState();
		break;

	case INMAP_STATE_READY:
		CheckCountDown();
		CheckInMapState();
		break;

	case INMAP_STATE_PLAYING:
		CheckInMapState();

		switch (m_eInMapProcess)
		{
		case CInMapMgr::INMAP_PROCESS_IDLE:
			m_pUI_Notify->SetCurAnimatingType(CNotify::ANIMATION_TYPE_START);
			SetRiderPlayingState(CPlayingInfo::PLAYING_STATE_PLAYING);
			m_eInMapProcess = CInMapMgr::INMAP_PROCESS_WORK;
			break;

		case CInMapMgr::INMAP_PROCESS_WORK:
			m_pServerMgr->Update_Send_Key();
			m_pServerMgr->Update_Send_Rotate();
			m_pServerMgr->Update_Send_CurMapLength();

			// 여기서 각자꺼를 받는다.

			CheckItemState();
			CheckEnemyAniState();
			CheckCreateEnemyBullet();
			break;

		case CInMapMgr::INMAP_PROCESS_END:
			break;
		default:
			break;
		}
		break;

	case INMAP_STATE_END :
		switch (m_eInMapProcess)
		{
		case CInMapMgr::INMAP_PROCESS_IDLE:
			SetRiderPlayingState(CPlayingInfo::PLAYING_STATE_GOAL);
			m_eInMapProcess = CInMapMgr::INMAP_PROCESS_WORK;
			break;

		case CInMapMgr::INMAP_PROCESS_WORK:
			break;
		case CInMapMgr::INMAP_PROCESS_END:
			break;
		default:
			break;
		}
		break;

	case INMAP_STATE_RESULT:
		switch (m_eInMapProcess)
		{
		case CInMapMgr::INMAP_PROCESS_IDLE:
			break;
		case CInMapMgr::INMAP_PROCESS_WORK:
			break;
		case CInMapMgr::INMAP_PROCESS_END:
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}
}

void CInMapMgr::CheckInMapState()
{
	m_tState = m_pServerMgr->GetState();
	if (m_eInMapState != m_tState.gamestate)
	{
		m_eInMapProcess = CInMapMgr::INMAP_PROCESS_IDLE;
		m_eInMapState = (InMapState)m_tState.gamestate;
	}
}

void CInMapMgr::CheckCountDown()
{
	static bool bFirstCountDown = false;
	static int	iCurCountDownNum = -1;


	if (iCurCountDownNum != m_tState.timecount)
		bFirstCountDown = true;


	if (bFirstCountDown)
	{
		iCurCountDownNum = m_tState.timecount;
		m_pUI_Notify->SetCurAnimatingType(iCurCountDownNum);
		bFirstCountDown = false;
	}
}

void CInMapMgr::SetRiderPlayingState(const CPlayingInfo::PlayingState& _ePlayingState)
{
	m_pPlayer->SetPlayingState(_ePlayingState);

	for (int i = 0; i < MAX_USER_NUM; ++i)
	{
		if (NULL != m_pEnemy[i])
			m_pEnemy[i]->SetPlayingState(_ePlayingState);
	}
}

void CInMapMgr::SetPlayerToReady()
{
	m_pPlayer->SetPlayingState(CPlayingInfo::PLAYING_STATE_STANDBY);

	CS_Ready tReady;
	tReady.type = CS_READY;
	tReady.size = sizeof(CS_Ready);

	CServer::GetInstance()->SendServerData(reinterpret_cast<char*>(&tReady));
}

void CInMapMgr::CheckEnemyInitAndCreate()
{
	SC_InitPlayer tInitPlayer;

	for (int i = 0; i < MAX_USER_NUM; ++i)
	{
		tInitPlayer = m_pServerMgr->GetInitEnemy(i);

		if (m_pEnemy[i] == NULL && -1 < tInitPlayer.ID)
			CreateEnemy(tInitPlayer);
	}
}

void CInMapMgr::CreateEnemy(SC_InitPlayer _tInitPlayer)
{
	CObj* pObj = NULL;

	pObj = CEnemy::Create(CDevice::GetInstance());
	NULL_CHECK(pObj);

	m_pEnemy[_tInitPlayer.ID] = ((CRider*)pObj);
	m_pEnemy[_tInitPlayer.ID]->SetInitPlayer(_tInitPlayer);
	m_pEnemy[_tInitPlayer.ID]->SetPlayingState(CPlayingInfo::PLAYING_STATE_STANDBY);

	m_pObjMgr->AddObj(CLayer::LAYERTYPE_GAMELOGIC, L"Enemy", pObj);
	m_pUI_Grade->AddPlayerGradeInfo(_tInitPlayer.ID);
}

void CInMapMgr::SetItemOnOff(unsigned int _nIdx, bool _bOn)
{
	if (_bOn) m_vecItem[_nIdx]->SetObjState(CObj::OBJ_STATE_ALIVE);
	else m_vecItem[_nIdx]->SetObjState(CObj::OBJ_STATE_IDLE);
}

void CInMapMgr::CheckItemState()
{
	SC_Item tItem = m_pServerMgr->GetItemExist();

	if ((int)(m_tItem.itemid) != (int)(tItem.itemid)
		|| (int)(m_tItem.isExist) != (int)(tItem.isExist))
	{
		m_tItem = tItem;
		SetItemOnOff(tItem.itemid, tItem.isExist);
	}
}

void CInMapMgr::CheckEnemyAniState()
{
	SC_ServerAni tServerAni;
	for (int i = 0; i < MAX_USER_NUM; ++i)
	{
		tServerAni = m_pServerMgr->GetEnemyAnimation(i);

		if (-1 >= tServerAni.ID)
			continue;

		if (NULL != m_pEnemy[i])
			m_pEnemy[i]->SetAnimation(tServerAni.aninum);
	}
}

void CInMapMgr::CheckBoosterEnd()
{
	//SC_Endboost tEndBooster = m_pServerMgr->GetEnd
}

void CInMapMgr::CheckCreateEnemyBullet()
{
	SC_Shoot tEnemyBullet;
	for (int i = 0; i < MAX_USER_NUM; ++i)
	{
		if (i == m_pPlayer->GetUserID())
			continue;

		tEnemyBullet = m_pServerMgr->GetShoot(i);
		if (tEnemyBullet.ID != -1)
		{
			if (NULL != m_pEnemy[i])
				((CEnemy*)m_pEnemy[i])->CreateBullet(tEnemyBullet.rotate);
			m_pServerMgr->ClearShoot(i);
		}
	}
}

void CInMapMgr::SetCurLap(WORD _nLap)
{
	if (2 == _nLap)
		m_pUI_Notify->SetCurAnimatingType(CNotify::ANIMATION_TYPE_FINALLAP);
	
	m_nCurLap = _nLap;
}
