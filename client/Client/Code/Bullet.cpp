#include "stdafx.h"
#include "Bullet.h"

#include "StaticModel.h"
#include "Texture.h"
#include "ResourceMgr.h"

#include "ColMgr.h"

const float CBullet::MAX_DISTANCE		= 500.f;

CBullet::CBullet(CDevice* _pDevice)
: CObj(_pDevice)
, m_iID(-1)

, m_pStaticModel(NULL)
, m_pTexture(NULL)

, m_pObjCol(NULL)
, m_vStartPos(0.f, 0.f, 0.f)
{

}

CBullet::~CBullet()
{
	Release();
}

CObj* CBullet::Create(CDevice* _pDevice,
					 const D3DXVECTOR3& _vPos,
					 const D3DXVECTOR3& _vDir)
{
	CObj* pObj = new CBullet(_pDevice);
	if (FAILED(pObj->Init()))
		::Safe_Delete(pObj);

	((CBullet*)pObj)->Init_Other(_vPos, _vDir);
	
	return pObj;
}

HRESULT CBullet::Init()
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

void CBullet::Init_Other(const D3DXVECTOR3& _vPos,
					const D3DXVECTOR3& _vDir)
{
	m_vStartPos = _vPos;
	m_pPhysics->m_vPos = _vPos;
	m_pPhysics->m_vDir = _vDir;
	m_pPhysics->m_vScale = D3DXVECTOR3(0.02f, 0.02f, 0.02f);


	m_eObjState = CObj::OBJ_STATE_ALIVE;

	m_pObjCol->m_pOwnerPhysics = m_pPhysics;
	m_pObjCol->m_vPlusPos = D3DXVECTOR3(0.f, 0.5f, 0.f);
	m_pObjCol->SetScale(D3DXVECTOR3(1.f, 1.f, 1.f));
}

int CBullet::Update()
{
	switch (m_eObjState)
	{
	case CObj::OBJ_STATE_NONE:
		break;

	case CObj::OBJ_STATE_IDLE:
		break;

	case CObj::OBJ_STATE_ALIVE:
		Move();
		CheckDie();
		CObj::Update_Component();
		break;

	case CObj::OBJ_STATE_DIE:
		break;
	}

	return m_eObjState;
}

void CBullet::Render()
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

void CBullet::CheckDie()
{
	if (MAX_DISTANCE <= D3DXVec3Length(&(m_pPhysics->m_vPos - m_vStartPos)))
		m_eObjState = CObj::OBJ_STATE_DIE;

	//if (CColMgr::GetInstance()->CheckColObjToObj(CColMgr::COL_TARGET_ENEMY, m_pObjCol))
	//	m_eObjState = CObj::OBJ_STATE_DIE;
}

void CBullet::Move()
{
	m_pPhysics->m_vPos
		+= m_pPhysics->m_fSpeed * m_pPhysics->m_vDir * CTimer::GetInstance()->GetTime();
}

void CBullet::SendDataToServer(D3DXVECTOR3 _vDir)
{
	CS_Shoot tShoot;
	tShoot.type = CS_SHOT;
	tShoot.size = sizeof(CS_Shoot);
	tShoot.ammonum = 0;
	tShoot.direction = _vDir;

	CServer::GetInstance()->SendServerData(reinterpret_cast<char*>(&tShoot));
}

void CBullet::Release()
{

}

