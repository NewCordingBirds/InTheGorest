#include "stdafx.h"
#include "Floor.h"

#include "ResourceMgr.h"
#include "Texture.h"
#include "KeyMgr.h"


CFloor::CFloor(CDevice* _pDevice)
: CBackGround(_pDevice)
, m_pTexture(NULL)
{
}

CFloor::~CFloor()
{

}

CObj* CFloor::Create(CDevice* _pDevice, const wstring& _wstrTextureKey)
{
	CObj* pObj = new CFloor(_pDevice);

	((CFloor*)pObj)->AddComponent(_wstrTextureKey);
	if (FAILED(pObj->Init()))
		::Safe_Delete(pObj);
	

	return pObj;
}

void CFloor::AddComponent(const wstring& _wstrTextureKey)
{
	// Texture ---------------
	m_pTexture = dynamic_cast<CTexture*>(
		CResourceMgr::GetInstance()->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC, CResourceMgr::RESOURCE_TYPE_TEXTURE, _wstrTextureKey));
	NULL_CHECK(m_pTexture);
	m_mapComponent.insert(make_pair(L"Texture", m_pTexture));


	// Plan Buffer ---------------------
	m_pBuffer = dynamic_cast<CPlanBuffer*>(CResourceMgr::GetInstance()->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC, CResourceMgr::RESOURCE_TYPE_BUFFER, L"Buffer_Plan"));
	NULL_CHECK(m_pBuffer);
	m_mapComponent.insert(make_pair(L"Buffer", m_pBuffer));
}

HRESULT CFloor::Init()
{
	m_eObjState = CObj::OBJ_STATE_ALIVE;
	m_pPhysics->m_eType = CPhysics::STATIC;

	return S_OK;
}

int CFloor::Update()
{
	if (CKeyMgr::GetInstance()->CheckKeyboardDown(DIK_Z))
	{
		SetTextureUV(D3DXVECTOR2(2.f, 2.f));
	}

	CObj::Update_Component();
	return m_eObjState;
}

void CFloor::Render()
{
	m_pPhysics->Render();
	m_pTexture->Render();
	m_pBuffer->Render();
}

void CFloor::Release()
{

}
