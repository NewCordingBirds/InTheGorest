#include "stdafx.h"
#include "Rider.h"

#include "Component.h"
#include "ResourceMgr.h"


CRider::CRider(CDevice* _pDevice)
: CUnit(_pDevice)
, m_pServerMgr(CServer::GetInstance())
{
	AddComponent();
}

CRider::~CRider()
{

}

void CRider::AddComponent()
{
	// UserInfo ------------
	m_pUserInfo = dynamic_cast<CUserInfo*>(CUserInfo::Create());
	NULL_CHECK(m_pUserInfo);
	m_mapComponent.insert(make_pair(L"UserInfo", m_pUserInfo));

	// PlayingInfo ---------------
	m_pPlayingInfo = dynamic_cast<CPlayingInfo*>(CPlayingInfo::Create());
	NULL_CHECK(m_pPlayingInfo);
	m_mapComponent.insert(make_pair(L"PlayingInfo", m_pPlayingInfo));


	// Model Buffer ---------------
#ifdef _DEBUG
	if (!(g_dwClientDebugMode & CLIENT_DEBUG_MODE_NO_ANIMATION))
	{
#endif
		m_pDynamicModel = dynamic_cast<CDynamicModel*>(CResourceMgr::GetInstance()->CloneResource(
			CResourceMgr::RESOURCE_ATTRI_DYNAMIC, CResourceMgr::RESOURCE_TYPE_MODEL, L"Model_Bird"));
		NULL_CHECK(m_pDynamicModel);
		m_mapComponent.insert(make_pair(L"Model_Dynamic", m_pDynamicModel));
#ifdef _DEBUG
	}
#endif

	// Texture ---------------
	m_pTexture = dynamic_cast<CTexture*>(CResourceMgr::GetInstance()->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC, CResourceMgr::RESOURCE_TYPE_TEXTURE, L"Texture_Bird"));
	NULL_CHECK(m_pTexture);
	m_mapComponent.insert(make_pair(L"Texture", m_pTexture));
}

// void CRider::Render()
// {
// 	m_pObjCol->Render();
// 
// 	m_pPhysics->Render();
// 	m_pTexture->Render();
// 
// #ifdef _DEBUG
// 	if (!(g_dwClientDebugMode & CLIENT_DEBUG_MODE_NO_ANIMATION))
// #endif
// 		m_pDynamicModel->PlayAnimation(m_pPlayingInfo->m_eAniState);
// }

void CRider::PlayAnimation()
{
	switch (m_pPlayingInfo->m_eAniState)
	{
	case CPlayingInfo::ANI_STATE_RUN:
	case CPlayingInfo::ANI_STATE_FALL:
		m_pDynamicModel->PlayAnimation(m_pPlayingInfo->m_eAniState);
		break;
	case CPlayingInfo::ANI_STATE_DAMAGE:
		m_pDynamicModel->PlayAnimation(m_pPlayingInfo->m_eAniState);

		if (m_pDynamicModel->Animend){
			m_pPlayingInfo->m_eAniState = CPlayingInfo::ANI_STATE_RUN;
			m_pDynamicModel->Animend = false;
		}
		break;
	case CPlayingInfo::ANI_STATE_DEAD:
	case CPlayingInfo::ANI_STATE_BOOSTER:
	case CPlayingInfo::ANI_STATE_BREAK:
		if (m_pDynamicModel->Animend){
			m_pDynamicModel->BWPlayAnim(m_pPlayingInfo->m_eAniState);
		}
		else{
			m_pDynamicModel->PlayAnimation(m_pPlayingInfo->m_eAniState);
		}
		break;
	default:
		break;
	}
}

void CRider::SetAnimation(CPlayingInfo::AniState _eAniState)
{
	if (_eAniState == m_pPlayingInfo->m_eAniState)
		return;

	m_pPlayingInfo->m_eAniState = _eAniState;
	m_pDynamicModel->m_fAniPlayTimer = 0;
	m_pDynamicModel->Animend = false;
}

void CRider::SetAnimation(BYTE _nAniState)
{
	if (_nAniState == m_pPlayingInfo->m_eAniState)
		return;

	m_pPlayingInfo->m_eAniState = (CPlayingInfo::AniState)_nAniState;
	m_pDynamicModel->m_fAniPlayTimer = 0;
	m_pDynamicModel->Animend = false;
}

void CRider::SetInitPlayer(const SC_InitPlayer& _tInitPlayer)
{
	if (-1 < m_pUserInfo->m_iID)
		return;

	m_pUserInfo->m_iID = _tInitPlayer.ID;
	m_pPhysics->m_vPos = _tInitPlayer.move;
	//m_pPhysics->m_vRadian = _tInitPlayer.radian;
	//m_pPhysics->m_vDir = D3DXVECTOR3(1.0, 0.f, 0.f);
	//m_pPhysics->m_fSpeed = _tInitPlayer.speed;
	//m_pPhysics->m_fAccel = _tInitPlayer.accel;

	//m_pPlayingInfo->m_ePlayingState = (CPlayingInfo::PlayingState)_tInitPlayer.playerstate;
}