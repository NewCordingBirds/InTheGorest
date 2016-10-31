#include "stdafx.h"
#include "InMapUI.h"


CInMapUI::CInMapUI(CDevice* _pDevice)
: CUI(_pDevice)
, m_pServerMgr(CServer::GetInstance())
, m_pInMapMgr(CInMapMgr::GetInstance())
{
	Init_PlayerObserver();
}

CInMapUI::~CInMapUI()
{
	Release_PlayerObserver();
}

void CInMapUI::Init_PlayerObserver()
{
	m_pPlayerObserver = CPlayerObserver::Create();
	CInfoSubject::GetInstance()->Subscribe(m_pPlayerObserver);
}

void CInMapUI::Release_PlayerObserver()
{
	CInfoSubject::GetInstance()->Unsubscribe(m_pPlayerObserver);
}

bool CInMapUI::m_bGameEnd = false;

void CInMapUI::CheckGameEnd()
{
	if (CInMapMgr::INMAP_STATE_END == m_pInMapMgr->GetInMapState())
		m_bGameEnd = true;
	else m_bGameEnd = false;
}


