#include "stdafx.h"
#include "Ammo.h"

#include "PlayingInfo.h"
#include "ResourceMgr.h"
#include "Timer.h"
#include "Font.h"

const float CAmmo::SIZE_FRAME_X_HALF = 151.f;
const float CAmmo::SIZE_FRAME_Y_HALF = 46.5f;

const float CAmmo::POS_Y = CLIENT_WINCY / 1.27f;


CAmmo::CAmmo(CDevice* _pDevice)
: CInMapUI(_pDevice)
, m_pFont_CurBullet(NULL)
, m_pFont_TotalBullet(NULL)
{

}

CAmmo::~CAmmo()
{
	Release();
}

CObj* CAmmo::Create(CDevice* _pDevice)
{
	CObj* pObj = new CAmmo(_pDevice);
	if (FAILED(pObj->Init()))
		::Safe_Delete(pObj);

	return pObj;
}

HRESULT CAmmo::Init()
{

	CResourceMgr*	pResourceMgr = CResourceMgr::GetInstance();

	// Texture ------------------
	m_tSprite_Frame.pTexture = dynamic_cast<CTexture*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC,
		CResourceMgr::RESOURCE_TYPE_TEXTURE,
		L"Texture_Ammo"));


	// Buffer -----------------
	m_tSprite_Frame.pBuffer = dynamic_cast<CUIBuffer*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC,
		CResourceMgr::RESOURCE_TYPE_BUFFER,
		L"Buffer_UI"));


	// Font ------------------
	m_pFont_CurBullet = dynamic_cast<CFont*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC,
		CResourceMgr::RESOURCE_TYPE_FONT,
		L"Font_HANSomaB"));

	m_pFont_CurBullet->m_eType = CFont::FONT_TYPE_OUTLINE;
	m_pFont_CurBullet->m_wstrText = to_wstring(MAX_BULLET_NUM);
	m_pFont_CurBullet->m_fSize = 48.f;
	m_pFont_CurBullet->m_nColor = 0xFFFFFFFF;
	m_pFont_CurBullet->m_nFlag = FW1_RIGHT | FW1_VCENTER | FW1_RESTORESTATE;
	m_pFont_CurBullet->m_vPos = D3DXVECTOR2(CLIENT_WINCX / 1.09f, CLIENT_WINCY / 1.28f);
	m_pFont_CurBullet->m_fOutlineSize = 1.f;
	m_pFont_CurBullet->m_nOutlineColor = 0xFF000000;


	m_pFont_TotalBullet = dynamic_cast<CFont*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC,
		CResourceMgr::RESOURCE_TYPE_FONT,
		L"Font_HANSomaB"));

	m_pFont_TotalBullet->m_eType = CFont::FONT_TYPE_OUTLINE;
	m_pFont_TotalBullet->m_wstrText = L"/" + to_wstring(MAX_BULLET_NUM);
	m_pFont_TotalBullet->m_fSize = 36.f;
	m_pFont_TotalBullet->m_nColor = 0xFFFFFFFF;
	m_pFont_TotalBullet->m_nFlag = FW1_CENTER | FW1_VCENTER | FW1_NOFLUSH | FW1_RESTORESTATE;
	m_pFont_TotalBullet->m_vPos = D3DXVECTOR2(CLIENT_WINCX / 1.05f, CLIENT_WINCY / 1.26f);
	m_pFont_TotalBullet->m_fOutlineSize = 1.f;
	m_pFont_TotalBullet->m_nOutlineColor = 0xFF000000;


	// View ---------------
	m_tSprite_Frame.m_matView._11 = SIZE_FRAME_X_HALF;
	m_tSprite_Frame.m_matView._22 = SIZE_FRAME_Y_HALF;
	m_tSprite_Frame.m_matView._33 = 1.f;
	m_tSprite_Frame.m_matView._41 = CLIENT_WINCX - SIZE_FRAME_X_HALF - (CLIENT_WINCX >> 1);
	m_tSprite_Frame.m_matView._42 = -POS_Y + (CLIENT_WINCY >> 1);


	// Dir -------------
	D3DXVec3TransformNormal(&m_pPhysics->m_vDir, &g_vLook, &m_pPhysics->m_matWorld);

	return S_OK;
}

int CAmmo::Update()
{
	m_pFont_CurBullet->m_wstrText = to_wstring(m_pPlayerObserver->GetBulletNum());
	return m_eObjState;
}

void CAmmo::Render()
{
	if (m_bGameEnd)
		return;


	m_pPhysics->Render();
	Invalidate_View(m_tSprite_Frame.m_matView);
	m_tSprite_Frame.pTexture->Render();
	m_tSprite_Frame.pBuffer->Render();


	// Font -------------
	m_pFont_CurBullet->Render();
	m_pFont_TotalBullet->Render();
}

void CAmmo::Release()
{
	::Safe_Delete(m_tSprite_Frame.pTexture);
	::Safe_Delete(m_tSprite_Frame.pBuffer);

	::Safe_Release(m_pFont_CurBullet);
	::Safe_Release(m_pFont_TotalBullet);
}