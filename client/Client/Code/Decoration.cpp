#include "stdafx.h"
#include "Decoration.h"


CDecoration::CDecoration(CDevice* _pDevice)
: CBackGround(_pDevice)
, m_pTexture(NULL)
{
}

CDecoration::~CDecoration()
{

}

CObj* CDecoration::Create(CDevice* _pDevice,
							const wstring& _wstrTextureKey,
							const wstring& _wstrModelKey)
{
	CObj* pObj = new CDecoration(_pDevice);
	if (FAILED(pObj->Init()))
		::Safe_Delete(pObj);

	((CDecoration*)pObj)->AddComponent(_wstrTextureKey, _wstrModelKey);

	return pObj;
}

void CDecoration::AddComponent(const wstring& _wstrTextureKey,
								const wstring& _wstrModelKey)
{
	// Texture ---------------
	m_pTexture = dynamic_cast<CTexture*>(
		CResourceMgr::GetInstance()->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC, CResourceMgr::RESOURCE_TYPE_TEXTURE, _wstrTextureKey));
	NULL_CHECK(m_pTexture);
	m_mapComponent.insert(make_pair(L"Texture", m_pTexture));


	// Model Buffer ---------------------
	m_pBuffer = dynamic_cast<CStaticModel*>(CResourceMgr::GetInstance()->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC, CResourceMgr::RESOURCE_TYPE_MODEL, _wstrModelKey));
	NULL_CHECK(m_pBuffer);
	m_mapComponent.insert(make_pair(L"Buffer", m_pBuffer));
}

HRESULT CDecoration::Init()
{
	m_eObjState = CObj::OBJ_STATE_ALIVE;
	m_pPhysics->m_eType = CPhysics::STATIC;

	return S_OK;
}

int CDecoration::Update()
{
	CObj::Update_Component();
	return m_eObjState;
}

void CDecoration::Render()
{
	m_pPhysics->Render();
	m_pTexture->Render();
	m_pBuffer->Render();
}

void CDecoration::Release()
{

}
