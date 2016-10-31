#include "stdafx.h"
#include "Notify.h"

#include "ResourceMgr.h"
#include "Font.h"
#include "Timer.h"

#include "KeyMgr.h"

const float CNotify::POS_X = CLIENT_WINCX / 2.f;
const float CNotify::POS_Y = CLIENT_WINCY / 2.5f;

const float CNotify::TIME_UI_ANIMATING_OTHER		= 1.5f;
const float CNotify::TIME_UI_ANIMATING_COUNTDOWN	= 1.f;

const float CNotify::DISTANCE_UI_ANIMATING = 30.f;

const float CNotify::SPEED_UI_ANIMATING_START		= 100.f;
const float CNotify::SPEED_UI_ANIMATING_COUNTDOWN	= 200.f;
const float CNotify::SPEED_UI_ANIMATING_FINAL	= 250.f;
const float CNotify::SPEED_UI_ANIMATING_LAP		= 200.f;




CNotify::CNotify(CDevice* _pDevice)
: CInMapUI(_pDevice)
, m_pFont_LastCountDown(NULL)

, m_eCurAnimatingType(ANIMATION_TYPE_NONE)
, m_fAniTimer(0.f)
{

}

CNotify::~CNotify()
{
	Release();
}

CObj* CNotify::Create(CDevice* _pDevice)
{
	CObj* pObj = new CNotify(_pDevice);
	if (FAILED(pObj->Init()))
		::Safe_Delete(pObj);

	return pObj;
}

HRESULT CNotify::Init()
{
	CResourceMgr*	pResourceMgr = CResourceMgr::GetInstance();

	string		strTextrueKey = "Texture_Notify_CountDown_0";
	wstring		wtrTextrueKey = L"";

	for (int i = 0; i < MAX_START_COUNT_NUM; ++i)
	{
		strTextrueKey = "Texture_Notify_CountDown_0" + to_string(i + 1);
		wtrTextrueKey.assign(strTextrueKey.begin(), strTextrueKey.end());
		m_tSprite_StartCountDown[i].pTexture = dynamic_cast<CTexture*>(pResourceMgr->CloneResource(
			CResourceMgr::RESOURCE_ATTRI_DYNAMIC,
			CResourceMgr::RESOURCE_TYPE_TEXTURE,
			wtrTextrueKey));

		m_tSprite_StartCountDown[i].pBuffer = dynamic_cast<CUIBuffer*>(pResourceMgr->CloneResource(
			CResourceMgr::RESOURCE_ATTRI_STATIC,
			CResourceMgr::RESOURCE_TYPE_BUFFER,
			L"Buffer_UI"));

		m_tSprite_StartCountDown[i].m_matView._11 = 50.f;
		m_tSprite_StartCountDown[i].m_matView._22 = 60.f;
		m_tSprite_StartCountDown[i].m_matView._33 = 1.f;
		m_tSprite_StartCountDown[i].m_matView._41 = POS_X + DISTANCE_UI_ANIMATING - (CLIENT_WINCX >> 1);
		m_tSprite_StartCountDown[i].m_matView._42 = -POS_Y + (CLIENT_WINCY >> 1);
	}


	m_tSprite_Start.pTexture = dynamic_cast<CTexture*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC,
		CResourceMgr::RESOURCE_TYPE_TEXTURE,
		L"Texture_Notify_Start"));

	m_tSprite_Start.pBuffer = dynamic_cast<CUIBuffer*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC,
		CResourceMgr::RESOURCE_TYPE_BUFFER,
		L"Buffer_UI"));

	m_tSprite_Start.m_matView._11 = 275;
	m_tSprite_Start.m_matView._22 = 60.f;
	m_tSprite_Start.m_matView._33 = 1.f;
	m_tSprite_Start.m_matView._41 = POS_X - (CLIENT_WINCX >> 1);
	m_tSprite_Start.m_matView._42 = -POS_Y - DISTANCE_UI_ANIMATING + (CLIENT_WINCY >> 1);


	m_tSprite_Final.pTexture = dynamic_cast<CTexture*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC,
		CResourceMgr::RESOURCE_TYPE_TEXTURE,
		L"Texture_Notify_Final"));

	m_tSprite_Final.pBuffer = dynamic_cast<CUIBuffer*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC,
		CResourceMgr::RESOURCE_TYPE_BUFFER,
		L"Buffer_UI"));

	m_tSprite_Final.m_matView._11 = 175.f;
	m_tSprite_Final.m_matView._22 = 52.5;
	m_tSprite_Final.m_matView._33 = 1.f;
	m_tSprite_Final.m_matView._41 = POS_X - DISTANCE_UI_ANIMATING - (m_tSprite_Final.m_matView._11 * 1.5f) - (CLIENT_WINCX >> 1);
	m_tSprite_Final.m_matView._42 = -POS_Y + (CLIENT_WINCY >> 1);


	m_tSprite_Lap.pTexture = dynamic_cast<CTexture*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC,
		CResourceMgr::RESOURCE_TYPE_TEXTURE,
		L"Texture_Notify_Lap"));

	m_tSprite_Lap.pBuffer = dynamic_cast<CUIBuffer*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC,
		CResourceMgr::RESOURCE_TYPE_BUFFER,
		L"Buffer_UI"));

	m_tSprite_Lap.m_matView._11 = 155.f;
	m_tSprite_Lap.m_matView._22 = 65.f;
	m_tSprite_Lap.m_matView._33 = 1.f;
	m_tSprite_Lap.m_matView._41 = POS_X + DISTANCE_UI_ANIMATING + (m_tSprite_Lap.m_matView._11 * 1.5f) - (CLIENT_WINCX >> 1);
	m_tSprite_Lap.m_matView._42 = -POS_Y + (CLIENT_WINCY >> 1);


	// Font -------------------
	m_pFont_LastCountDown = dynamic_cast<CFont*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC,
		CResourceMgr::RESOURCE_TYPE_FONT,
		L"Font_HANSomaB"));

	m_pFont_LastCountDown->m_eType = CFont::FONT_TYPE_OUTLINE;
	m_pFont_LastCountDown->m_wstrText = L"10";
	m_pFont_LastCountDown->m_fSize = 120.f;
	m_pFont_LastCountDown->m_nColor = 0xFF008AFF;
	m_pFont_LastCountDown->m_nFlag = FW1_CENTER | FW1_VCENTER | FW1_RESTORESTATE;
	m_pFont_LastCountDown->m_vPos = D3DXVECTOR2(POS_X, POS_Y);
	m_pFont_LastCountDown->m_fOutlineSize = 7.f;
	m_pFont_LastCountDown->m_nOutlineColor = 0xFFFFFFFF;


	// Dir -------------
	D3DXVec3TransformNormal(&m_pPhysics->m_vDir, &g_vLook, &m_pPhysics->m_matWorld);

	return S_OK;
}

int CNotify::Update()
{
	AnimationgUI();
	CInMapUI::CheckGameEnd();

	return m_eObjState;
}

void CNotify::Render()
{
	if (m_bGameEnd)
		return;

	m_pPhysics->Render();
	RenderUI();
}

void CNotify::SetCurAnimatingType(const AnimationType& _eAnimationType)
{
	InitUIPos();
	m_eCurAnimatingType = _eAnimationType;
	m_fAniTimer = 0.f;
}

void CNotify::SetCurAnimatingType(const int& _iCountDownNum)
{
	InitUIPos();

	switch (_iCountDownNum)
	{
	case 3: m_eCurAnimatingType = ANIMATION_TYPE_CNT_3; break;
	case 2: m_eCurAnimatingType = ANIMATION_TYPE_CNT_2; break;
	case 1: m_eCurAnimatingType = ANIMATION_TYPE_CNT_1; break;
	default:
		break;
	}

	m_fAniTimer = 0.f;
}

void CNotify::InitUIPos()
{
	switch (m_eCurAnimatingType)
	{
	case CNotify::ANIMATION_TYPE_NONE:
		break;

	case CNotify::ANIMATION_TYPE_CNT_3:
		m_tSprite_StartCountDown[2].m_matView._41 = POS_X + DISTANCE_UI_ANIMATING - (CLIENT_WINCX >> 1);
		break;

	case CNotify::ANIMATION_TYPE_CNT_2:
		m_tSprite_StartCountDown[1].m_matView._41 = POS_X + DISTANCE_UI_ANIMATING - (CLIENT_WINCX >> 1);
		break;

	case CNotify::ANIMATION_TYPE_CNT_1:
		m_tSprite_StartCountDown[0].m_matView._41 = POS_X + DISTANCE_UI_ANIMATING - (CLIENT_WINCX >> 1);
		break;

	case CNotify::ANIMATION_TYPE_START:
		m_tSprite_Start.m_matView._42 = -POS_Y - DISTANCE_UI_ANIMATING + (CLIENT_WINCY >> 1);
		break;


	case CNotify::ANIMATION_TYPE_FINALLAP:

		m_tSprite_Final.m_matView._41 = POS_X - (m_tSprite_Final.m_matView._11 * 1.5f) - (CLIENT_WINCX >> 1);
		m_tSprite_Lap.m_matView._41 = POS_X  + (m_tSprite_Lap.m_matView._11 * 1.5f) - (CLIENT_WINCX >> 1);
		break;

	default:
		break;
	}
}


void CNotify::AnimationgUI()
{
	switch (m_eCurAnimatingType)
	{
	case CNotify::ANIMATION_TYPE_NONE:
		break;

	case CNotify::ANIMATION_TYPE_CNT_3:
	{
										  float fTime = CTimer::GetInstance()->GetTime();
										  m_fAniTimer += fTime;

										  if (TIME_UI_ANIMATING_COUNTDOWN <= m_fAniTimer)
										  {
											  m_fAniTimer = 0.f;
											  InitUIPos();
											  m_eCurAnimatingType = CNotify::ANIMATION_TYPE_NONE;
										  }
										  else
										  {
											  m_tSprite_StartCountDown[2].m_matView._41 -= SPEED_UI_ANIMATING_COUNTDOWN * fTime;
											
											  if (m_tSprite_StartCountDown[2].m_matView._41 <= POS_X - (CLIENT_WINCX >> 1))
												  m_tSprite_StartCountDown[2].m_matView._41 = POS_X - (CLIENT_WINCX >> 1);
										  }
	}
		break;

	case CNotify::ANIMATION_TYPE_CNT_2:
	{
										  float fTime = CTimer::GetInstance()->GetTime();
										  m_fAniTimer += fTime;

										  if (TIME_UI_ANIMATING_COUNTDOWN <= m_fAniTimer)
										  {
											  m_fAniTimer = 0.f;
											  InitUIPos();
											  m_eCurAnimatingType = CNotify::ANIMATION_TYPE_NONE;
										  }
										  else
										  {
											  m_tSprite_StartCountDown[1].m_matView._41 -= SPEED_UI_ANIMATING_COUNTDOWN * fTime;

											  if (m_tSprite_StartCountDown[1].m_matView._41 <= POS_X - (CLIENT_WINCX >> 1))
												  m_tSprite_StartCountDown[1].m_matView._41 = POS_X - (CLIENT_WINCX >> 1);
										  }
	}
		break;

	case CNotify::ANIMATION_TYPE_CNT_1:
	{
										  float fTime = CTimer::GetInstance()->GetTime();
										  m_fAniTimer += fTime;

										  if (TIME_UI_ANIMATING_COUNTDOWN <= m_fAniTimer)
										  {
											  m_fAniTimer = 0.f;
											  InitUIPos();
											  m_eCurAnimatingType = CNotify::ANIMATION_TYPE_NONE;
										  }
										  else
										  {
											  m_tSprite_StartCountDown[0].m_matView._41 -= SPEED_UI_ANIMATING_COUNTDOWN * fTime;

											  if (m_tSprite_StartCountDown[0].m_matView._41 <= POS_X - (CLIENT_WINCX >> 1))
												  m_tSprite_StartCountDown[0].m_matView._41 = POS_X - (CLIENT_WINCX >> 1);
										  }
	}
		break;

	case CNotify::ANIMATION_TYPE_START:
	{
										  float fTime = CTimer::GetInstance()->GetTime();
										  m_fAniTimer += fTime;

										  if (TIME_UI_ANIMATING_OTHER <= m_fAniTimer)
										  {
											  m_fAniTimer = 0.f;
											  InitUIPos();
											  m_eCurAnimatingType = CNotify::ANIMATION_TYPE_NONE;
										  }
										  else
										  {
											  m_tSprite_Start.m_matView._42 += SPEED_UI_ANIMATING_START * fTime;

											  if (m_tSprite_Start.m_matView._42 >= -POS_Y + (CLIENT_WINCY >> 1))
												  m_tSprite_Start.m_matView._42 = -POS_Y + (CLIENT_WINCY >> 1);
										  }
	}
		break;
	

	case CNotify::ANIMATION_TYPE_FINALLAP:
	{
											 float fTime = CTimer::GetInstance()->GetTime();
											 m_fAniTimer += fTime;

											 if (TIME_UI_ANIMATING_OTHER <= m_fAniTimer)
											 {
												 m_fAniTimer = 0.f;
												 InitUIPos();
												 m_eCurAnimatingType = CNotify::ANIMATION_TYPE_NONE;
											 }
											 else
											 {
												 m_tSprite_Final.m_matView._41 += SPEED_UI_ANIMATING_FINAL * fTime;
												 m_tSprite_Lap.m_matView._41 -= SPEED_UI_ANIMATING_LAP * fTime;


												 if (m_tSprite_Final.m_matView._41 >= POS_X - (m_tSprite_Final.m_matView._11) - (CLIENT_WINCX >> 1))
													 m_tSprite_Final.m_matView._41 = POS_X - (m_tSprite_Final.m_matView._11) - (CLIENT_WINCX >> 1);

												 if (m_tSprite_Lap.m_matView._41 <= POS_X + (m_tSprite_Lap.m_matView._11 * 1.2f) - (CLIENT_WINCX >> 1))
													 m_tSprite_Lap.m_matView._41 = POS_X + (m_tSprite_Lap.m_matView._11 * 1.2f) - (CLIENT_WINCX >> 1);
											 }
	}
		break;

	default:
		break;
	}
}

void CNotify::RenderUI()
{
	switch (m_eCurAnimatingType)
	{
	case CNotify::ANIMATION_TYPE_NONE:
		break;

	case CNotify::ANIMATION_TYPE_CNT_3:
		Invalidate_View(m_tSprite_StartCountDown[2].m_matView);
		m_tSprite_StartCountDown[2].pTexture->Render();
		m_tSprite_StartCountDown[2].pBuffer->Render();
		break;

	case CNotify::ANIMATION_TYPE_CNT_2:
		Invalidate_View(m_tSprite_StartCountDown[1].m_matView);
		m_tSprite_StartCountDown[1].pTexture->Render();
		m_tSprite_StartCountDown[1].pBuffer->Render();
		break;

	case CNotify::ANIMATION_TYPE_CNT_1:
		Invalidate_View(m_tSprite_StartCountDown[0].m_matView);
		m_tSprite_StartCountDown[0].pTexture->Render();
		m_tSprite_StartCountDown[0].pBuffer->Render();
		break;

	case CNotify::ANIMATION_TYPE_START:
		Invalidate_View(m_tSprite_Start.m_matView);
		m_tSprite_Start.pTexture->Render();
		m_tSprite_Start.pBuffer->Render();
		break;

	case CNotify::ANIMATION_TYPE_FINALLAP:
		Invalidate_View(m_tSprite_Final.m_matView);
		m_tSprite_Final.pTexture->Render();
		m_tSprite_Final.pBuffer->Render();

		Invalidate_View(m_tSprite_Lap.m_matView);
		m_tSprite_Lap.pTexture->Render();
		m_tSprite_Lap.pBuffer->Render();
		break;

	default:
		break;
	}
}

void CNotify::Release()
{
	for (int i = 0; i < MAX_START_COUNT_NUM; ++i)
	{
		::Safe_Delete(m_tSprite_StartCountDown[i].pTexture);
		::Safe_Delete(m_tSprite_StartCountDown[i].pBuffer);
	}

	::Safe_Delete(m_tSprite_Start.pTexture);
	::Safe_Delete(m_tSprite_Start.pBuffer);

	::Safe_Delete(m_tSprite_Final.pTexture);
	::Safe_Delete(m_tSprite_Final.pBuffer);

	::Safe_Delete(m_tSprite_Lap.pTexture);
	::Safe_Delete(m_tSprite_Lap.pBuffer);

	::Safe_Release(m_pFont_LastCountDown);
} 
