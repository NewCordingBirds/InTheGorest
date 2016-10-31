#include "stdafx.h"
#include "Enemy.h"

#include "ObjMgr.h"
#include "Bullet.h"

CEnemy::CEnemy(CDevice* _pDevice)
: CRider(_pDevice)
//, m_pEnemyObserver(NULL)
, m_pObjMgr(CObjMgr::GetInstance())
{

}

CEnemy::~CEnemy()
{
	Release();
}

CObj* CEnemy::Create(CDevice* _pDevice)
{
	CObj* pObj = new CEnemy(_pDevice);
	if (FAILED(pObj->Init()))
		::Safe_Delete(pObj);

	return pObj;
}

HRESULT CEnemy::Init()
{
	m_eObjState = CObj::OBJ_STATE_ALIVE;

	m_pObjCol->m_pOwnerPhysics = m_pPhysics;
	m_pObjCol->m_vPlusPos = D3DXVECTOR3(0.f, 1.f, 0.f);
	m_pObjCol->SetScale(D3DXVECTOR3(1.5f, 1.5f, 2.4f));
	m_pObjCol->SetPlusPos(D3DXVECTOR3(0.f, 2.5f, 0.f));


	m_pPhysics->m_vDir = g_vLook;
	m_pPhysics->m_fSpeed = 150.f;
	m_pPhysics->m_vScale = D3DXVECTOR3(0.03f, 0.03f, 0.03f);

	m_pPlayingInfo->m_eAniState = CPlayingInfo::ANI_STATE_RUN;


	//// PlayerObserver -------------
	//m_pEnemyObserver = CEnemyObserver::Create();
	//CInfoSubject::GetInstance()->Subscribe(m_pEnemyObserver);


	return S_OK;
}

int CEnemy::Update()
{
	switch (m_eObjState)
	{
	case CObj::OBJ_STATE_NONE:
		break;

	case CObj::OBJ_STATE_IDLE:
		break;

	case CObj::OBJ_STATE_ALIVE:

		switch (m_pPlayingInfo->m_ePlayingState)
		{

		case CPlayingInfo::PLAYING_STATE_STANDBY:
			m_pPlayingInfo->m_ePlayingState = CPlayingInfo::PLAYING_STATE_READY;
			break;

		case CPlayingInfo::PLAYING_STATE_READY:
			break;

		case CPlayingInfo::PLAYING_STATE_PLAYING:
			UpdateServerData_Physics();
			break;

		case CPlayingInfo::PLAYING_STATE_GOAL:
			break;

		default:
			break;
		}

		CObj::Update_Component();
		break;

	case CObj::OBJ_STATE_DIE:
		break;
	}

	return m_eObjState;
}

void CEnemy::Render()
{
	m_pPhysics->Render();
	m_pTexture->Render();

#ifdef _DEBUG
	if (!(g_dwClientDebugMode & CLIENT_DEBUG_MODE_NO_ANIMATION))
#endif
		m_pDynamicModel->PlayAnimation(m_pPlayingInfo->m_eAniState);

	if (m_bRenderObjCol_Other)
		m_pObjCol->Render();
}

void CEnemy::UpdateServerData_Physics()
{
	SC_PlayerPos tEnemyPos = m_pServerMgr->GetEnemyPos(m_pUserInfo->m_iID);
	m_pPhysics->m_vPos = tEnemyPos.move;

	SC_PlayerRotate tEnemyRotate = m_pServerMgr->GetEnemyRotate(m_pUserInfo->m_iID);
	m_pPhysics->m_vRadian = tEnemyRotate.rad;
}

void CEnemy::Release()
{

}

void CEnemy::CreateBullet(D3DXVECTOR3 _vDir)
{
	D3DXVECTOR3 vDir(0.f, 0.f, 0.f);
	
	vDir = _vDir;

	CObj*	pObj = CBullet::Create(
		m_pDevice,
		m_pPhysics->m_vPos + (m_pObjCol->m_fRadius * m_pPhysics->m_vDir),
		vDir);

	m_pObjMgr->AddObj(CLayer::LAYERTYPE_GAMELOGIC, L"Bullet_Enemy", pObj);
}