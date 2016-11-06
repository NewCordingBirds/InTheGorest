#include "stdafx.h"
#include "ProgressRate.h"

#include "ResourceMgr.h"
#include "Timer.h"
#include "Font.h"
#include "Renderer.h"
#include "ProgressMgr.h"

const float CProgressRate::SIZE_FRAME_Y_HALF	= 205.f;
const float CProgressRate::SIZE_RATE_Y			= 404.f;
const float CProgressRate::POS_X				= CLIENT_WINCX / 1.04f;


CProgressRate::CProgressRate(CDevice* _pDevice)
: CInMapUI(_pDevice)
, m_pFont_Lap(NULL)
, m_pFont_CurLap(NULL)
, m_pFont_TotalLap(NULL)
, m_pFont_FPS(NULL)

, m_nCurLap(1)
, m_nTotalLap(1)

, m_fCurMapLenght(0.0f)
{

}

CProgressRate::~CProgressRate()
{
	Release();
}

CObj* CProgressRate::Create(CDevice* _pDevice)
{
	CObj* pObj = new CProgressRate(_pDevice);
	if (FAILED(pObj->Init()))
		::Safe_Delete(pObj);

	return pObj;
}

HRESULT CProgressRate::Init()
{
	m_nTotalLap = 2;


	CResourceMgr*	pResourceMgr = CResourceMgr::GetInstance();

	// Texture ------------------
	m_tSprite_Frame.pTexture = dynamic_cast<CTexture*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC,
		CResourceMgr::RESOURCE_TYPE_TEXTURE,
		L"Texture_ProgressRate_Frame"));

	m_tSprite_Rate.pTexture = dynamic_cast<CTexture*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC,
		CResourceMgr::RESOURCE_TYPE_TEXTURE,
		L"Texture_ProgressRate_Rate"));


	// Buffer -----------------
	m_tSprite_Frame.pBuffer = dynamic_cast<CUIBuffer*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC,
		CResourceMgr::RESOURCE_TYPE_BUFFER,
		L"Buffer_UI"));

	m_tSprite_Rate.pBuffer = dynamic_cast<CUIBuffer*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC,
		CResourceMgr::RESOURCE_TYPE_BUFFER,
		L"Buffer_UI"));


	// Font ----------------
	m_pFont_Lap = dynamic_cast<CFont*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC,
		CResourceMgr::RESOURCE_TYPE_FONT,
		L"Font_HANSomaB"));

	m_pFont_Lap->m_eType = CFont::FONT_TYPE_OUTLINE;
	m_pFont_Lap->m_wstrText = L"LAP";
	m_pFont_Lap->m_fSize = 30.f;
	m_pFont_Lap->m_nColor = 0xFFFFFFFF;
	m_pFont_Lap->m_nFlag = FW1_CENTER | FW1_VCENTER | FW1_NOFLUSH | FW1_RESTORESTATE;
	m_pFont_Lap->m_vPos = D3DXVECTOR2(CLIENT_WINCX / 1.2f, CLIENT_WINCY / 13.8f);
	m_pFont_Lap->m_fOutlineSize = 1.f;
	m_pFont_Lap->m_nOutlineColor = 0xFF000000;


	m_pFont_CurLap = dynamic_cast<CFont*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC,
		CResourceMgr::RESOURCE_TYPE_FONT,
		L"Font_HANSomaB"));

	m_pFont_CurLap->m_eType = CFont::FONT_TYPE_OUTLINE;
	m_pFont_CurLap->m_wstrText = to_wstring(m_nCurLap);
	m_pFont_CurLap->m_fSize = 48.f;
	m_pFont_CurLap->m_nColor = 0xFF008AFF;
	m_pFont_CurLap->m_nFlag = FW1_CENTER | FW1_VCENTER | FW1_RESTORESTATE;
	m_pFont_CurLap->m_vPos = D3DXVECTOR2(CLIENT_WINCX / 1.12f, CLIENT_WINCY / 16.3f);
	m_pFont_CurLap->m_fOutlineSize = 1.f;
	m_pFont_CurLap->m_nOutlineColor = 0xFF000000;


	m_pFont_TotalLap = dynamic_cast<CFont*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC,
		CResourceMgr::RESOURCE_TYPE_FONT,
		L"Font_HANSomaB"));

	m_pFont_TotalLap->m_eType = CFont::FONT_TYPE_OUTLINE;
	m_pFont_TotalLap->m_wstrText = L"/" + to_wstring(m_nTotalLap);
	m_pFont_TotalLap->m_fSize = 30.f;
	m_pFont_TotalLap->m_nColor = 0xFFFFFFFF;
	m_pFont_TotalLap->m_nFlag = FW1_CENTER | FW1_VCENTER | FW1_NOFLUSH | FW1_RESTORESTATE;
	m_pFont_TotalLap->m_vPos = D3DXVECTOR2(CLIENT_WINCX / 1.09f, CLIENT_WINCY / 13.8f);
	m_pFont_TotalLap->m_fOutlineSize = 1.f;
	m_pFont_TotalLap->m_nOutlineColor = 0xFF000000;


	m_pFont_FPS = dynamic_cast<CFont*>(CResourceMgr::GetInstance()->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC,
		CResourceMgr::RESOURCE_TYPE_FONT,
		L"Font_HANSomaB"));

	m_pFont_FPS->m_wstrText = to_wstring(CRenderer::m_dwFPSCnt);
	m_pFont_FPS->m_fSize = 15.f;
	m_pFont_FPS->m_nColor = 0xFFFFFFFF;
	m_pFont_FPS->m_nFlag = FW1_RIGHT | FW1_VCENTER | FW1_RESTORESTATE;
	m_pFont_FPS->m_vPos = D3DXVECTOR2(CLIENT_WINCX / 1.09f, CLIENT_WINCY / 8.5f);


	// View ---------------
	m_tSprite_Frame.m_matView._11 = 19.f;
	m_tSprite_Frame.m_matView._22 = SIZE_FRAME_Y_HALF;
	m_tSprite_Frame.m_matView._33 = 1.f;
	m_tSprite_Frame.m_matView._41 = POS_X - (CLIENT_WINCX >> 1);
	m_tSprite_Frame.m_matView._42 = -(SIZE_FRAME_Y_HALF) + (CLIENT_WINCY >> 1);

	m_tSprite_Rate.m_matView._11 = 11.5f;
	m_tSprite_Rate.m_matView._22 = SIZE_RATE_Y/2.f;
	m_tSprite_Rate.m_matView._33 = 1.f;
	m_tSprite_Rate.m_matView._41 = POS_X - (CLIENT_WINCX >> 1);
	m_tSprite_Rate.m_matView._42 = -(SIZE_RATE_Y/2.f) + (CLIENT_WINCY >> 1);


	// Dir -------------
	D3DXVec3TransformNormal(&m_pPhysics->m_vDir, &g_vLook, &m_pPhysics->m_matWorld);

	return S_OK;
}


void CProgressRate::Render()
{
	if (m_bGameEnd)
		return;


	m_pPhysics->Render();

	Invalidate_View(m_tSprite_Rate.m_matView);
	m_tSprite_Rate.pTexture->Render();
	m_tSprite_Rate.pBuffer->Render();

	Invalidate_View(m_tSprite_Frame.m_matView);
	m_tSprite_Frame.pTexture->Render();
	m_tSprite_Frame.pBuffer->Render();


	// Font -------------
	m_pFont_Lap->Render();
	m_pFont_CurLap->Render();
	m_pFont_TotalLap->Render();
	m_pFont_FPS->Render();
}

void CProgressRate::Release()
{
	::Safe_Delete(m_tSprite_Frame.pTexture);
	::Safe_Delete(m_tSprite_Frame.pBuffer);
	::Safe_Delete(m_tSprite_Rate.pTexture);
	::Safe_Delete(m_tSprite_Rate.pBuffer);

	::Safe_Release(m_pFont_Lap);
	::Safe_Release(m_pFont_CurLap);
	::Safe_Release(m_pFont_TotalLap);
	::Safe_Release(m_pFont_FPS);
}