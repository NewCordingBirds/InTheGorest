#include "stdafx.h"
#include "UIObserver.h"


CUIObserver::CUIObserver()
: m_iTargetingState(0)
, m_vDirToTarget(-0.1f, -0.1f, -0.1f)
{

}

CUIObserver::~CUIObserver()
{

}

CUIObserver* CUIObserver::Create()
{
	CUIObserver* pObserver = new CUIObserver;
	//	pObserver->Update(CInfoSubject::DATA_TYPE_PLAYER_INITPLAYER);

	return pObserver;
}

void CUIObserver::Update(int _iMessage,
	UINT _nIdx /*= 0*/)
{
	vector<void*>* pData = m_pInfoSubject->GetDatalist(_iMessage);
	//NULL_CHECK(pData);
	if (pData == NULL)
		return;

	switch (_iMessage)
	{
	case CInfoSubject::DATA_TYPE_UI_STATIE:
		m_iTargetingState = *((int*)((*pData)[_nIdx]));
		break;

	case CInfoSubject::DATA_TYPE_UI_DIR_TO_TARGET:
		m_vDirToTarget = *((D3DXVECTOR3*)((*pData)[_nIdx]));
		break;

	default:
		break;
	}
}