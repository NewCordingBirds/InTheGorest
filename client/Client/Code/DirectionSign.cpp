#include "stdafx.h"
#include "DirectionSign.h"


CDirectionSign::CDirectionSign(CDevice* _pDevice)
: CDecoration(_pDevice)
{

}

CDirectionSign::~CDirectionSign()
{

}

CObj* CDirectionSign::Create(CDevice* _pDevice)
{
	CObj* pDeco = new CDirectionSign(_pDevice);
	if (FAILED(pDeco->Init()))
		::Safe_Delete(pDeco);

	return pDeco;

}

HRESULT CDirectionSign::Init()
{

	m_eObjState = CObj::OBJ_STATE_ALIVE;
	m_pPhysics->m_eType = CPhysics::STATIC;


	// Texture ---------------
	m_pTexture = dynamic_cast<CTexture*>(
		CResourceMgr::GetInstance()->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC, CResourceMgr::RESOURCE_TYPE_TEXTURE, L"Texture_Other"));

	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture", m_pTexture));


	// Model Buffer ---------------------
	m_pBuffer = dynamic_cast<CStaticModel*>(CResourceMgr::GetInstance()->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC, CResourceMgr::RESOURCE_TYPE_MODEL, L"Model_DirectionSign"));
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer", m_pBuffer));


	return S_OK;
}

void CDirectionSign::SetWorldMatrix(const D3DXVECTOR3& _vPos /*= D3DXVECTOR3(0.f, 0.f, 0.f)*/,
									const D3DXVECTOR3& _vRadian /*= D3DXVECTOR3(0.f, 0.f, 0.f)*/)
{
	m_pPhysics->SetWorldMatrix(_vPos, _vRadian, D3DXVECTOR3(10.f, 10.f, 10.f));
}

int CDirectionSign::Update()
{
	CObj::Update_Component();
	return m_eObjState;
}
