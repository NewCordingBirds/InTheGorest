#include "stdafx.h"
#include "Grade.h"

#include "ResourceMgr.h"
#include "Timer.h"
#include "Font.h"

const float CGrade::POS_X = CLIENT_WINCX / 11.6f;
const float CGrade::POS_Y = CLIENT_WINCY / 6.74f;


CGrade::CGrade(CDevice* _pDevice)
: CInMapUI(_pDevice)

, m_pFont_Time(NULL)

, m_pTimer(CTimer::GetInstance())
, m_fTimer(0.0f)

, m_nCurRiderNum(0)
{
	for (int i = 0; i < MAX_RIDER_NUM; ++i)
	{
		m_pFont_GradeNum[i] = NULL;
		m_pFont_ID[i] = NULL;
	}
}

CGrade::~CGrade()
{
	Release();
}

CObj* CGrade::Create(CDevice* _pDevice)
{
	CObj* pObj = new CGrade(_pDevice);
	if (FAILED(pObj->Init()))
		::Safe_Delete(pObj);

	return pObj;
}

HRESULT CGrade::Init()
{

	CResourceMgr*	pResourceMgr = CResourceMgr::GetInstance();

	string		strTextrueKey = "Texture_Grade_0";
	wstring		wtrTextrueKey = L"";

	for (int i = 0; i < MAX_RIDER_NUM; ++i)
	{
		strTextrueKey = "Texture_Grade_0" + to_string(i+1);
		wtrTextrueKey.assign(strTextrueKey.begin(), strTextrueKey.end());
		m_tSprite[i].pTexture = dynamic_cast<CTexture*>(pResourceMgr->CloneResource(
			CResourceMgr::RESOURCE_ATTRI_DYNAMIC,
			CResourceMgr::RESOURCE_TYPE_TEXTURE,
			wtrTextrueKey));
		
		m_tSprite[i].pBuffer = dynamic_cast<CUIBuffer*>(pResourceMgr->CloneResource(
			CResourceMgr::RESOURCE_ATTRI_STATIC,
			CResourceMgr::RESOURCE_TYPE_BUFFER,
			L"Buffer_UI"));

		m_tSprite[i].m_matView._11 = 65.f;
		m_tSprite[i].m_matView._22 = 55.f;
		m_tSprite[i].m_matView._33 = 1.f;
		m_tSprite[i].m_matView._41 = POS_X - (CLIENT_WINCX >> 1);
		m_tSprite[i].m_matView._42 = -POS_Y + (CLIENT_WINCY >> 1);
	}

	// Font -------------------
	m_pFont_Time = dynamic_cast<CFont*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC,
		CResourceMgr::RESOURCE_TYPE_FONT,
		L"Font_HANSomaB"));

	m_pFont_Time->m_eType = CFont::FONT_TYPE_OUTLINE;
	m_pFont_Time->m_wstrText = L"TIME  00 : 00.00";
	m_pFont_Time->m_fSize = 18.f;
	m_pFont_Time->m_nColor = 0xFFFFFFFF;
	m_pFont_Time->m_nFlag = FW1_LEFT | FW1_VCENTER | FW1_RESTORESTATE;
	m_pFont_Time->m_vPos = D3DXVECTOR2(CLIENT_WINCX / 22.4f, CLIENT_WINCY / 4.f);
	m_pFont_Time->m_fOutlineSize = 1.f;
	m_pFont_Time->m_nOutlineColor = 0xFF000000;


	AddPlayerGradeInfo(m_pServerMgr->GetInitPlayer().ID);
	

	// Dir -------------
	D3DXVec3TransformNormal(&m_pPhysics->m_vDir, &g_vLook, &m_pPhysics->m_matWorld);

	return S_OK;
}

int CGrade::Update()
{
	switch (m_pInMapMgr->GetInMapState())
	{
	case CInMapMgr::INMAP_STATE_LOADING:
		break;

	case CInMapMgr::INMAP_STATE_READY:
		break;

	case CInMapMgr::INMAP_STATE_PLAYING:
		Update_Time();
		break;

	case CInMapMgr::INMAP_STATE_RESULT:
		break;

	default:
		break;
	}

	return m_eObjState;
}

void CGrade::Render()
{
	if (m_bGameEnd)
		return;


	m_pPhysics->Render();
	Invalidate_View(m_tSprite[0].m_matView);
	m_tSprite[0].pTexture->Render();
	m_tSprite[0].pBuffer->Render();


	// Font -----------
	m_pFont_Time->Render();

	for (unsigned int i = 0; i < m_nCurRiderNum; ++i)
	{
		m_pFont_GradeNum[i]->Render();
		m_pFont_ID[i]->Render();
	}
}

void CGrade::AddPlayerGradeInfo(UINT _nID)
{
	if (m_nCurRiderNum > MAX_USER_NUM)
		return;


	m_pFont_GradeNum[_nID] = dynamic_cast<CFont*>(CResourceMgr::GetInstance()->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC,
		CResourceMgr::RESOURCE_TYPE_FONT,
		L"Font_HANSomaB"));

	m_pFont_GradeNum[_nID]->m_eType = CFont::FONT_TYPE_OUTLINE;
	m_pFont_GradeNum[_nID]->m_wstrText = to_wstring(_nID + 1) + L".";
	m_pFont_GradeNum[_nID]->m_fSize = 15.f;
	m_pFont_GradeNum[_nID]->m_nColor = 0xFF00B4FF;
	m_pFont_GradeNum[_nID]->m_nFlag = FW1_CENTER | FW1_VCENTER | FW1_NOFLUSH | FW1_RESTORESTATE;
	m_pFont_GradeNum[_nID]->m_vPos = D3DXVECTOR2(
		CLIENT_WINCX / 18.f,
		CLIENT_WINCY / 3.3f + (_nID * m_pFont_GradeNum[_nID]->m_fSize * 1.5f));
	m_pFont_GradeNum[_nID]->m_fOutlineSize = 1.f;
	m_pFont_GradeNum[_nID]->m_nOutlineColor = 0xFF000000;


	m_pFont_ID[_nID] = dynamic_cast<CFont*>(CResourceMgr::GetInstance()->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC,
		CResourceMgr::RESOURCE_TYPE_FONT,
		L"Font_HANSomaB"));

	m_pFont_ID[_nID]->m_eType = CFont::FONT_TYPE_OUTLINE;
	m_pFont_ID[_nID]->m_wstrText = L"Player_0" + to_wstring(_nID);
	m_pFont_ID[_nID]->m_fSize = 15.f;
	m_pFont_ID[_nID]->m_nColor = 0xFFFFFFFF;
	m_pFont_ID[_nID]->m_nFlag = FW1_LEFT | FW1_VCENTER | FW1_RESTORESTATE;
	m_pFont_ID[_nID]->m_vPos = D3DXVECTOR2(
		CLIENT_WINCX / 14.5f,
		CLIENT_WINCY / 3.3f + (_nID * m_pFont_GradeNum[_nID]->m_fSize * 1.5f));
	m_pFont_ID[_nID]->m_fOutlineSize = 1.f;
	m_pFont_ID[_nID]->m_nOutlineColor = 0xFF000000;


	++m_nCurRiderNum;
}

void CGrade::Update_Time()
{
	static int		iMinit = 0;
	static float	fSecond = 0;

	float fCurrentTotalTime = m_pServerMgr->GetTime().time;
	fSecond += fCurrentTotalTime - m_fTimer;
	m_fTimer = fCurrentTotalTime;


	if (60.0f <= fSecond)
	{
		++iMinit;
		fSecond = 0.0f;
	}

	// L"TIME  00 : 00.00";

	m_pFont_Time->m_wstrText.clear();

	if (10 <= iMinit)
		m_pFont_Time->m_wstrText = L"TIME  " + to_wstring(iMinit);
	else
		m_pFont_Time->m_wstrText = L"TIME  0" + to_wstring(iMinit);


	float fForSecondPoint = (int)(fSecond * 100.0f);
	fForSecondPoint /= 100.0f;

	if (10.0f <= fSecond)
		m_pFont_Time->m_wstrText += L" : " + to_wstring(fForSecondPoint);
	else
		m_pFont_Time->m_wstrText += L" : 0" + to_wstring(fForSecondPoint);


	m_pFont_Time->m_wstrText.erase(
		m_pFont_Time->m_wstrText.begin() + 16,
		m_pFont_Time->m_wstrText.end());
}

void CGrade::Release()
{
	::Safe_Release(m_pFont_Time);

	for (unsigned int i = 0; i < m_nCurRiderNum; ++i)
	{
		::Safe_Delete(m_tSprite[i].pTexture);
		::Safe_Delete(m_tSprite[i].pBuffer);

		::Safe_Release(m_pFont_GradeNum[i]);
		::Safe_Release(m_pFont_ID[i]);
	}
}
