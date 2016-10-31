#include "stdafx.h"
#include "Item.h"

#include "Physics.h"
#include "Texture.h"
#include "StaticModel.h"
#include "ColMgr.h"

#include "ServerMgr.h"


CItem::CItem(CDevice* _pDevice, ItemType _eItemType)
: CTrigger(_pDevice)
, m_iID(-1)

, m_eItemType(_eItemType)
, m_iGageValue(0)
, m_pTexture(NULL)
, m_pBuffer(NULL)
{
	AddComponent(_eItemType);
}

CItem::~CItem()
{

}

CObj* CItem::Create(CDevice* _pDevice, ItemType _eItemType)
{
	CObj* pObj = new CItem(_pDevice, _eItemType);
	if (FAILED(pObj->Init()))
		::Safe_Delete(pObj);

	return pObj;
}

void CItem::AddComponent(ItemType _eItemType)
{
	// Texture ---------------
	wstring wsrtTextureKey;

	switch (_eItemType)
	{
	case CItem::ITEM_TYPE_FULL :	wsrtTextureKey = L"Texture_Item_Full"; break;
	case CItem::ITEM_TYPE_THIRD:	wsrtTextureKey = L"Texture_Item_Third"; break;
	}

	m_pTexture = dynamic_cast<CTexture*>(
		CResourceMgr::GetInstance()->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC, CResourceMgr::RESOURCE_TYPE_TEXTURE, wsrtTextureKey));
	NULL_CHECK(m_pTexture);
	m_mapComponent.insert(make_pair(L"Texture", m_pTexture));


	// Model Buffer ---------------------
	m_pBuffer = dynamic_cast<CStaticModel*>(CResourceMgr::GetInstance()->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC, CResourceMgr::RESOURCE_TYPE_MODEL, L"Model_Item"));
	NULL_CHECK(m_pBuffer);
	m_mapComponent.insert(make_pair(L"Buffer", m_pBuffer));
}

void CItem::SetWorldMatrix(const D3DXVECTOR3& _vPos /*= D3DXVECTOR3(0.f, 0.f, 0.f)*/)
{
	m_pPhysics->m_vPos = _vPos;
	m_pObjCol->Update_WorldMatrix();
}

HRESULT CItem::Init()
{
	static int iID = 0;
	m_iID = iID++;

	m_eObjState = CObj::OBJ_STATE_ALIVE;

	m_pObjCol->m_pOwnerPhysics = m_pPhysics;
	m_pObjCol->m_eObjColType = CObjCol::OBJCOL_TYPE_STATIC;

	m_pObjCol->SetScale(D3DXVECTOR3(5.f, 5.f, 5.f));

	switch (m_eItemType)
	{
	case CItem::ITEM_TYPE_FULL :	m_iGageValue = 90; break;
	case CItem::ITEM_TYPE_THIRD:	m_iGageValue = 30; break;
	}

	m_pPhysics->m_vScale = D3DXVECTOR3(10.f, 10.f, 10.f);

	return S_OK;
}

int CItem::Update()
{
	switch (m_eObjState)
	{
	case CObj::OBJ_STATE_NONE:
		break;

	case CObj::OBJ_STATE_ALIVE:
		CObj::Update_Component();

	case CObj::OBJ_STATE_IDLE:
		Animating();
		break;

	case CObj::OBJ_STATE_DIE:
		break;
	}
	
	return m_eObjState;
}

void CItem::Render()
{
	if (CObj::OBJ_STATE_ALIVE != m_eObjState)
		return;

	m_pPhysics->Render();
	m_pTexture->Render();
	m_pBuffer->Render();

	if (m_bRenderObjCol_Other)
		m_pObjCol->Render();
}

void CItem::Animating()
{
	m_pPhysics->m_vRadian.y += 2.f * CTimer::GetInstance()->GetTime();
}

void CItem::SendDataToServer()
{
	CS_ItemGet tITemGet;
	tITemGet.type = CS_ITEMGET;
	tITemGet.size = sizeof(CS_ItemGet);
	tITemGet.itemnum = m_iID;

	CServer::GetInstance()->SendServerData(reinterpret_cast<char*>(&tITemGet));
}

int CItem::GetRecosterGaugePlusValue()
{
	switch (m_eItemType)
	{
	case CItem::ITEM_TYPE_FULL : return RECOSTER_GAUGE_FULL;
	case CItem::ITEM_TYPE_THIRD: return RECOSTER_GAUGE_THIRD;
	}

	return 0;
}

void CItem::Release()
{

}
