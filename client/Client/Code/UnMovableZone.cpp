#include "stdafx.h"
#include "UnMovableZone.h"

#include "Texture.h"
#include "StaticModel.h"

#include "ServerMgr.h"


CUnMovableZone::CUnMovableZone(CDevice* _pDevice)
: CTrigger(_pDevice)
{
}

CUnMovableZone::~CUnMovableZone()
{

}

CObj* CUnMovableZone::Create(CDevice* _pDevice)
{
	CObj* pObj = new CUnMovableZone(_pDevice);
	if (FAILED(pObj->Init()))
		::Safe_Delete(pObj);

	return pObj;
}

void CUnMovableZone::SetWorldMatrix(const D3DXVECTOR3& _vPos /*= D3DXVECTOR3(0.f, 0.f, 0.f)*/,
	const D3DXVECTOR3& _vRadian /*= D3DXVECTOR3(0.f, 0.f, 0.f)*/)
{
	m_pPhysics->SetWorldMatrix(_vPos, _vRadian);
	m_pObjCol->Update_WorldMatrix();
}

HRESULT CUnMovableZone::Init()
{
	m_eObjState = CObj::OBJ_STATE_ALIVE;

	m_pObjCol->m_pOwnerPhysics = m_pPhysics;
	m_pObjCol->m_eObjColType = CObjCol::OBJCOL_TYPE_STATIC;


	return S_OK;
}

int CUnMovableZone::Update()
{
	CObj::Update_Component();
	return m_eObjState;
}

void CUnMovableZone::Render()
{
	if (m_bRenderObjCol_UnMovableZone)
		m_pObjCol->Render();
}

void CUnMovableZone::SendDataToServer()
{
	//CS_ItemGet tITemGet;
	//tITemGet.type = CS_ITEMGET;
	//tITemGet.size = sizeof(CS_ItemGet);
	//tITemGet.itemnum = m_iID;

	//CServer::GetInstance()->SendServerData(reinterpret_cast<char*>(&tITemGet));
}

void CUnMovableZone::Release()
{

}
