#include "stdafx.h"
#include "EnemyBullet.h"

#include "StaticModel.h"
#include "Texture.h"
#include "ResourceMgr.h"

#include "ColMgr.h"
#include <conio.h>

const float CEnemyBullet::MAX_DISTANCE = 500.f;

CEnemyBullet::CEnemyBullet(CDevice* _pDevice)
	: CObj(_pDevice)
	, m_pStaticModel(NULL)
	, m_pTexture(NULL)

	, m_pObjCol(NULL)
	, m_vStartPos(0.f, 0.f, 0.f)
{

}

CEnemyBullet::~CEnemyBullet()
{
	Release();
}

CObj* CEnemyBullet::Create(CDevice* _pDevice,
	const D3DXVECTOR3& _vPos,
	const D3DXVECTOR3& _vDir)
{
	CObj* pObj = new CEnemyBullet(_pDevice);
	if (FAILED(pObj->Init()))
		::Safe_Delete(pObj);

	((CEnemyBullet*)pObj)->Init_Other(_vPos, _vDir);

	return pObj;
}

HRESULT CEnemyBullet::Init()
{
	// Texture ---------------
	m_pTexture = dynamic_cast<CTexture*>(
		CResourceMgr::GetInstance()->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC, CResourceMgr::RESOURCE_TYPE_TEXTURE, L"Texture_Bullet"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture", m_pTexture));


	// Model Buffer ---------------------
#ifdef _DEBUG
	if (!(g_dwClientDebugMode & CLIENT_DEBUG_MODE_NO_ANIMATION))
	{
#endif
		m_pStaticModel = dynamic_cast<CStaticModel*>(CResourceMgr::GetInstance()->CloneResource(
			CResourceMgr::RESOURCE_ATTRI_DYNAMIC, CResourceMgr::RESOURCE_TYPE_MODEL, L"Model_Bullet"));
		NULL_CHECK_RETURN(m_pStaticModel, E_FAIL);
		m_mapComponent.insert(make_pair(L"Buffer", m_pStaticModel));
#ifdef _DEBUG
	}
#endif


	// ObjCol ---------------
	m_pObjCol = dynamic_cast<CObjCol*>(CObjCol::Create(m_pDevice));
	NULL_CHECK_RETURN(m_pObjCol, E_FAIL);
	m_mapComponent.insert(make_pair(L"ObjCol", m_pObjCol));


	// Physics --------
	m_pPhysics->m_fSpeed = 500.f;


	return S_OK;
}

void CEnemyBullet::Init_Other(const D3DXVECTOR3& _vPos,
	const D3DXVECTOR3& _vDir)
{
	m_vStartPos = _vPos;
	m_pPhysics->m_vPos = _vPos;
	m_pPhysics->m_vDir = _vDir;
	m_pPhysics->m_vScale = D3DXVECTOR3(0.02f, 0.02f, 0.02f);


	m_eObjState = CObj::OBJ_STATE_ALIVE;

	m_pObjCol->m_pOwnerPhysics = m_pPhysics;
	m_pObjCol->m_vPlusPos = D3DXVECTOR3(0.f, 0.5f, 0.f);
	m_pObjCol->SetScale(D3DXVECTOR3(0.5f, 0.5f, 0.5f));
}

int CEnemyBullet::Update()
{
	switch (m_eObjState)
	{
	case CObj::OBJ_STATE_NONE:
		break;

	case CObj::OBJ_STATE_IDLE:
		break;

	case CObj::OBJ_STATE_ALIVE:
		CheckDie();
		CObj::Update_Component();
		UpdateServerData_Physics();
		break;

	case CObj::OBJ_STATE_DIE:
		break;
	}

	return m_eObjState;
}

void CEnemyBullet::Render()
{
	m_pPhysics->Render();

#ifdef _DEBUG
	if (!(g_dwClientDebugMode & CLIENT_DEBUG_MODE_NO_ANIMATION))
	{
#endif
		m_pTexture->Render();
		m_pStaticModel->Render();
#ifdef _DEBUG
	}
#endif

	if (m_bRenderObjCol_Other)
		m_pObjCol->Render();
}

void CEnemyBullet::CheckDie()
{
	if (MAX_DISTANCE <= D3DXVec3Length(&(m_pPhysics->m_vPos - m_vStartPos)))
		m_eObjState = CObj::OBJ_STATE_DIE;

	if (CColMgr::GetInstance()->CheckColObjToObj(CColMgr::COL_TARGET_ENEMY, m_pObjCol))
		m_eObjState = CObj::OBJ_STATE_DIE;
}

void CEnemyBullet::Release()
{

}

void CEnemyBullet::UpdateServerData_Physics()
{
	SC_Shoot tShootpos = m_pServerMgr->GetShoot(m_pUserInfo->m_iID);
	m_pPhysics->m_vPos = tShootpos.AMMOpos;
	_cprintf("???");
}
