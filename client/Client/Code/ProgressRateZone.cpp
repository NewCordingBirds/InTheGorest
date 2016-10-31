#include "stdafx.h"
#include "ProgressRateZone.h"

#include "Texture.h"
#include "StaticModel.h"


CProgressRateZone::CProgressRateZone(CDevice* _pDevice)
: CTrigger(_pDevice)
, m_iID(0)
{
}

CProgressRateZone::~CProgressRateZone()
{

}

CObj* CProgressRateZone::Create(CDevice* _pDevice)
{
	CObj* pObj = new CProgressRateZone(_pDevice);
	if (FAILED(pObj->Init()))
		::Safe_Delete(pObj);

	return pObj;
}

void CProgressRateZone::SetWorldMatrix(const D3DXVECTOR3& _vPos /*= D3DXVECTOR3(0.f, 0.f, 0.f)*/,
	const D3DXVECTOR3& _vRadian /*= D3DXVECTOR3(0.f, 0.f, 0.f)*/)
{
	m_pPhysics->SetWorldMatrix(_vPos, _vRadian);
	m_pObjCol->Update_WorldMatrix();
}

HRESULT CProgressRateZone::Init()
{
	m_eObjState = CObj::OBJ_STATE_ALIVE;

	m_pObjCol->m_pOwnerPhysics = m_pPhysics;
	m_pObjCol->m_eObjColType = CObjCol::OBJCOL_TYPE_STATIC;

	static int iID = 0;
	m_iID = iID++;

	return S_OK;
}

int CProgressRateZone::Update()
{
	CObj::Update_Component();
	return m_eObjState;
}

void CProgressRateZone::Render()
{
	if (m_bRenderObjCol_ProgressRateZone)
		m_pObjCol->Render();
}

void CProgressRateZone::Release()
{

}