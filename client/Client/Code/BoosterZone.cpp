#include "stdafx.h"
#include "BoosterZone.h"

#include "Texture.h"
#include "StaticModel.h"

#include "ServerMgr.h"


CBoosterZone::CBoosterZone(CDevice* _pDevice)
: CTrigger(_pDevice)
, m_pTexture(NULL)
, m_pBuffer(NULL)
{
	AddComponent();
}

CBoosterZone::~CBoosterZone()
{

}

CObj* CBoosterZone::Create(CDevice* _pDevice)
{
	CObj* pObj = new CBoosterZone(_pDevice);
	if (FAILED(pObj->Init()))
		::Safe_Delete(pObj);

	return pObj;
}

void CBoosterZone::AddComponent()
{
	// Texture ---------------
	m_pTexture = dynamic_cast<CTexture*>(
		CResourceMgr::GetInstance()->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC, CResourceMgr::RESOURCE_TYPE_TEXTURE, L"Texture_Other"));
	NULL_CHECK(m_pTexture);
	m_mapComponent.insert(make_pair(L"Texture", m_pTexture));


	// Model Buffer ---------------------
	m_pBuffer = dynamic_cast<CStaticModel*>(CResourceMgr::GetInstance()->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC, CResourceMgr::RESOURCE_TYPE_MODEL, L"Model_Booster"));
	NULL_CHECK(m_pBuffer);
	m_mapComponent.insert(make_pair(L"Buffer", m_pBuffer));
}

void CBoosterZone::SetWorldMatrix(const D3DXVECTOR3& _vPos /*= D3DXVECTOR3(0.f, 0.f, 0.f)*/,
									const D3DXVECTOR3& _vRadian /*= D3DXVECTOR3(0.f, 0.f, 0.f)*/)
{
	m_pPhysics->SetWorldMatrix(_vPos, _vRadian, D3DXVECTOR3(10.f, 10.f, 10.f));
	m_pObjCol->Update_WorldMatrix();

	D3DXVec3TransformNormal(&m_pPhysics->m_vDir, &g_vLook, &m_pPhysics->m_matWorld);
}

HRESULT CBoosterZone::Init()
{
	m_eObjState = CObj::OBJ_STATE_ALIVE;

	m_pObjCol->m_pOwnerPhysics = m_pPhysics;
	m_pObjCol->m_eObjColType = CObjCol::OBJCOL_TYPE_STATIC;

	m_pObjCol->SetScale(D3DXVECTOR3(3.f, 3.f, 5.f));


	return S_OK;
}

int CBoosterZone::Update()
{
	CObj::Update_Component();
	return m_eObjState;
}

void CBoosterZone::Render()
{
	m_pPhysics->Render();
	m_pTexture->Render();
	m_pBuffer->Render();


	if (m_bRenderObjCol_Other)
		m_pObjCol->Render();
}

void CBoosterZone::SendDataToServer()
{
	// 방향 보내주면 돼요

	CS_CollBooster tCollBooster;
	tCollBooster.type = CS_COLLBOOSTER;
	tCollBooster.size = sizeof(tCollBooster);
	tCollBooster.direction = m_pPhysics->m_vDir;

	CServer::GetInstance()->SendServerData(reinterpret_cast<char*>(&tCollBooster));
}

void CBoosterZone::Release()
{

}
