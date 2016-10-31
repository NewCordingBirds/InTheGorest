#include "stdafx.h"
#include "Status.h"

#include "ResourceMgr.h"
#include "Timer.h"

const float CStatus::SIZE_HP_X = 292.f;
const float CStatus::SIZE_RECOSTER_X = 311.f;


CStatus::CStatus(CDevice* _pDevice)
: CInMapUI(_pDevice)
, m_pFont_CurHp(NULL)
, m_pFont_TotalHp(NULL)

, m_pFont_CurRecoster(NULL)
, m_pFont_TotalRecoster(NULL)
{

}

CStatus::~CStatus()
{
	Release();
}

CObj* CStatus::Create(CDevice* _pDevice)
{
	CObj* pObj = new CStatus(_pDevice);
	if (FAILED(pObj->Init()))
		::Safe_Delete(pObj);

	return pObj;
}

HRESULT CStatus::Init()
{
	
	CResourceMgr*	pResourceMgr = CResourceMgr::GetInstance();

	// Texture ------------------
	m_tSprite_Frame.pTexture = dynamic_cast<CTexture*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC,
		CResourceMgr::RESOURCE_TYPE_TEXTURE,
		L"Texture_Status_Frame"));

	m_tSprite_Hp.pTexture = dynamic_cast<CTexture*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC,
		CResourceMgr::RESOURCE_TYPE_TEXTURE,
		L"Texture_Status_Hp"));

	m_tSprite_Recoster.pTexture = dynamic_cast<CTexture*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC,
		CResourceMgr::RESOURCE_TYPE_TEXTURE,
		L"Texture_Status_Recoster"));


	// Buffer -----------------
	m_tSprite_Frame.pBuffer = dynamic_cast<CUIBuffer*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC,
		CResourceMgr::RESOURCE_TYPE_BUFFER,
		L"Buffer_UI"));

	m_tSprite_Hp.pBuffer = dynamic_cast<CUIBuffer*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC,
		CResourceMgr::RESOURCE_TYPE_BUFFER,
		L"Buffer_UI"));

	m_tSprite_Recoster.pBuffer = dynamic_cast<CUIBuffer*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC,
		CResourceMgr::RESOURCE_TYPE_BUFFER,
		L"Buffer_UI"));


	// Font ------------------
	m_pFont_CurHp = dynamic_cast<CFont*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC,
		CResourceMgr::RESOURCE_TYPE_FONT,
		L"Font_HANSomaB"));

	m_pFont_CurHp->m_eType = CFont::FONT_TYPE_OUTLINE;
	m_pFont_CurHp->m_wstrText = to_wstring(MAX_HP);
	m_pFont_CurHp->m_fSize = 20.f;
	m_pFont_CurHp->m_nColor = 0xFFFFFFFF;
	m_pFont_CurHp->m_nFlag = FW1_RIGHT | FW1_VCENTER | FW1_RESTORESTATE ;
	m_pFont_CurHp->m_vPos = D3DXVECTOR2(CLIENT_WINCX / 5.1f, CLIENT_WINCY / 1.345f);
	m_pFont_CurHp->m_fOutlineSize = 1.f;
	m_pFont_CurHp->m_nOutlineColor = 0xFF000000;


	m_pFont_TotalHp = dynamic_cast<CFont*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC,
		CResourceMgr::RESOURCE_TYPE_FONT,
		L"Font_HANSomaB"));

	m_pFont_TotalHp->m_eType = CFont::FONT_TYPE_OUTLINE;
	m_pFont_TotalHp->m_wstrText = L"/" + to_wstring(MAX_HP);
	m_pFont_TotalHp->m_fSize = 20.f;
	m_pFont_TotalHp->m_nColor = 0xFFFFFFFF;
	m_pFont_TotalHp->m_nFlag = FW1_CENTER | FW1_VCENTER | FW1_NOFLUSH | FW1_RESTORESTATE;
	m_pFont_TotalHp->m_vPos = D3DXVECTOR2(CLIENT_WINCX / 4.55f, CLIENT_WINCY / 1.345f);
	m_pFont_TotalHp->m_fOutlineSize = 1.f;
	m_pFont_TotalHp->m_nOutlineColor = 0xFF000000;

	m_pFont_CurRecoster = dynamic_cast<CFont*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC,
		CResourceMgr::RESOURCE_TYPE_FONT,
		L"Font_HANSomaB"));

	m_pFont_CurRecoster->m_eType = CFont::FONT_TYPE_OUTLINE;
	m_pFont_CurRecoster->m_wstrText = to_wstring(0);
	m_pFont_CurRecoster->m_fSize = 20.f;
	m_pFont_CurRecoster->m_nColor = 0xFFFFFFFF;
	m_pFont_CurRecoster->m_nFlag = FW1_RIGHT | FW1_VCENTER | FW1_RESTORESTATE;
	m_pFont_CurRecoster->m_vPos = D3DXVECTOR2(CLIENT_WINCX / 5.1f, CLIENT_WINCY / 1.235f);
	m_pFont_CurRecoster->m_fOutlineSize = 1.f;
	m_pFont_CurRecoster->m_nOutlineColor = 0xFF000000;


	m_pFont_TotalRecoster = dynamic_cast<CFont*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC,
		CResourceMgr::RESOURCE_TYPE_FONT,
		L"Font_HANSomaB"));

	m_pFont_TotalRecoster->m_eType = CFont::FONT_TYPE_OUTLINE;
	m_pFont_TotalRecoster->m_wstrText = L"/" + to_wstring(MAX_RECOSTER);
	m_pFont_TotalRecoster->m_fSize = 20.f;
	m_pFont_TotalRecoster->m_nColor = 0xFFFFFFFF;
	m_pFont_TotalRecoster->m_nFlag = FW1_CENTER | FW1_VCENTER | FW1_NOFLUSH | FW1_RESTORESTATE;
	m_pFont_TotalRecoster->m_vPos = D3DXVECTOR2(CLIENT_WINCX / 4.55f, CLIENT_WINCY / 1.235f);
	m_pFont_TotalRecoster->m_fOutlineSize = 1.f;
	m_pFont_TotalRecoster->m_nOutlineColor = 0xFF000000;


	// View ---------------
	m_tSprite_Frame.m_matView._11 = 200;
	m_tSprite_Frame.m_matView._22 = 90;
	m_tSprite_Frame.m_matView._33 = 1.f;
	m_tSprite_Frame.m_matView._41 = m_tSprite_Frame.m_matView._11 - (CLIENT_WINCX >> 1);
	m_tSprite_Frame.m_matView._42 = -(CLIENT_WINCY / 1.3f) + (CLIENT_WINCY >> 1);

	m_tSprite_Hp.m_matView._11 = SIZE_HP_X / 2.f;
	m_tSprite_Hp.m_matView._22 = 9.5f;
	m_tSprite_Hp.m_matView._33 = 1.f;
	m_tSprite_Hp.m_matView._41 = m_tSprite_Hp.m_matView._11 + 20.f - (CLIENT_WINCX >> 1);
	m_tSprite_Hp.m_matView._42 = -(CLIENT_WINCY / 1.29f) + (CLIENT_WINCY >> 1);

	m_tSprite_Recoster.m_matView._11 = SIZE_RECOSTER_X / 2.f;
	m_tSprite_Recoster.m_matView._22 = 9.f;
	m_tSprite_Recoster.m_matView._33 = 1.f;
	m_tSprite_Recoster.m_matView._41 = m_tSprite_Recoster.m_matView._11 - (CLIENT_WINCX >> 1);
	m_tSprite_Recoster.m_matView._42 = -(CLIENT_WINCY / 1.19f) + (CLIENT_WINCY >> 1);


	// Dir -------------
	D3DXVec3TransformNormal(&m_pPhysics->m_vDir, &g_vLook, &m_pPhysics->m_matWorld);

	return S_OK;
}

int CStatus::Update()
{
	WORD nHp = m_pPlayerObserver->GetHp();
	WORD nRecoster = m_pPlayerObserver->GetRecoster();

	m_pFont_CurHp->m_wstrText = to_wstring(nHp);
	m_pFont_CurRecoster->m_wstrText = to_wstring(nRecoster);

	m_tSprite_Hp.m_matView._11 = (nHp * SIZE_HP_X / MAX_HP) / 2.f;
	m_tSprite_Hp.m_matView._41 = m_tSprite_Hp.m_matView._11 + 20.f - (CLIENT_WINCX >> 1);
	m_tSprite_Recoster.m_matView._11 = (nRecoster * SIZE_RECOSTER_X / MAX_RECOSTER) / 2.f;
	m_tSprite_Recoster.m_matView._41 = m_tSprite_Recoster.m_matView._11 - (CLIENT_WINCX >> 1);
	
	return m_eObjState;
}

void CStatus::Render()
{
	if (m_bGameEnd)
		return;


	m_pPhysics->Render();

	Invalidate_View(m_tSprite_Hp.m_matView);
	m_tSprite_Hp.pTexture->Render();
	m_tSprite_Hp.pBuffer->Render();

	Invalidate_View(m_tSprite_Recoster.m_matView);
	m_tSprite_Recoster.pTexture->Render();
	m_tSprite_Recoster.pBuffer->Render();

	Invalidate_View(m_tSprite_Frame.m_matView);
	m_tSprite_Frame.pTexture->Render();
	m_tSprite_Frame.pBuffer->Render();


	// Font -------------
	m_pFont_CurHp->Render();
	m_pFont_TotalHp->Render();

	m_pFont_CurRecoster->Render();
	m_pFont_TotalRecoster->Render();
}

void CStatus::Release()
{
	::Safe_Delete(m_tSprite_Frame.pTexture);
	::Safe_Delete(m_tSprite_Frame.pBuffer);

	::Safe_Delete(m_tSprite_Hp.pTexture);
	::Safe_Delete(m_tSprite_Hp.pBuffer);

	::Safe_Delete(m_tSprite_Recoster.pTexture);
	::Safe_Delete(m_tSprite_Recoster.pBuffer);

	::Safe_Release(m_pFont_CurHp);
	::Safe_Release(m_pFont_TotalHp);
	::Safe_Release(m_pFont_CurRecoster);
	::Safe_Release(m_pFont_TotalRecoster);
}