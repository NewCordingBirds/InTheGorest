#include "stdafx.h"
#include "TargetBox.h"

#include "ResourceMgr.h"
#include "UIBuffer.h"

#include "Timer.h"
#include "StaticCamera.h"

#include "ServerMgr.h"


const float CTargetBox::SIZE_FRAME_X		= 554.f;
const float CTargetBox::SIZE_FRAME_Y		= 335.f;
const float CTargetBox::SIZE_FRAME_X_HALF	= SIZE_FRAME_X / 2.f;
const float CTargetBox::SIZE_FRAME_Y_HALF	= SIZE_FRAME_Y / 2.f;

const D3DXVECTOR2	CTargetBox::CENTER_POS = D3DXVECTOR2((CLIENT_WINCX >> 1), POS_Y);

const float CTargetBox::SIZE_CICLE_BASIC_HALF			= 59.f;
const float CTargetBox::SIZE_CICLE_TARGETING_HALF		= 46.5f;

const float CTargetBox::POS_Y				= CLIENT_WINCY / 2.62f;
const float CTargetBox::ROTATE_SPEED_CIRCLE = 0.8f;


const float CTargetBox::DISTANCE_INDUCTION		= 300.f;
const float CTargetBox::SPEED_MOVE_TO_TARGET	= 300.f;


CTargetBox::CTargetBox(CDevice* _pDevice)
: CInMapUI(_pDevice)
, m_eState(STATE_BASIC)

, m_vTargetPos(-1.f, -1.f)
, m_vDirToTarget(-1.f, -1.f, -1.f)
, m_fCircleAngle(0.f)

, m_pServerMgr(CServer::GetInstance())
, m_pInfoSubject(CInfoSubject::GetInstance())
{

}
 
CTargetBox::~CTargetBox()
{
	Release();
}

CObj* CTargetBox::Create(CDevice* _pDevice)
{
	CObj* pObj = new CTargetBox(_pDevice);
	if (FAILED(pObj->Init()))
		::Safe_Delete(pObj);

	return pObj;
}

HRESULT CTargetBox::Init()
{
	// Texture ------------------
	CResourceMgr*	pResourceMgr = CResourceMgr::GetInstance();

	
	m_tSprite_Frame.pTexture = dynamic_cast<CTexture*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC,
		CResourceMgr::RESOURCE_TYPE_TEXTURE,
		L"Texture_TargetBox_Frame"));

	m_tSprite_Circle[STATE_BASIC].pTexture = dynamic_cast<CTexture*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC,
		CResourceMgr::RESOURCE_TYPE_TEXTURE,
		L"Texture_TargetBox_Circle_Basic"));
	m_tSprite_Circle[STATE_TARGETING].pTexture = dynamic_cast<CTexture*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC,
		CResourceMgr::RESOURCE_TYPE_TEXTURE,
		L"Texture_TargetBox_Circle_Targeting"));

	m_tSprite_Point[STATE_BASIC].pTexture = dynamic_cast<CTexture*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC,
		CResourceMgr::RESOURCE_TYPE_TEXTURE,
		L"Texture_TargetBox_Point_Basic"));
	m_tSprite_Point[STATE_TARGETING].pTexture = dynamic_cast<CTexture*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC,
		CResourceMgr::RESOURCE_TYPE_TEXTURE,
		L"Texture_TargetBox_Point_Targeting"));


	// Buffer -----------------
	m_tSprite_Frame.pBuffer = dynamic_cast<CUIBuffer*>(pResourceMgr->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC,
		CResourceMgr::RESOURCE_TYPE_BUFFER,
		L"Buffer_UI"));

	for (int i = 0; i < STATE_END; ++i)
	{
		m_tSprite_Circle[i].pBuffer = dynamic_cast<CUIBuffer*>(pResourceMgr->CloneResource(
			CResourceMgr::RESOURCE_ATTRI_STATIC,
			CResourceMgr::RESOURCE_TYPE_BUFFER,
			L"Buffer_UI"));
		m_tSprite_Point[i].pBuffer = dynamic_cast<CUIBuffer*>(pResourceMgr->CloneResource(
			CResourceMgr::RESOURCE_ATTRI_STATIC,
			CResourceMgr::RESOURCE_TYPE_BUFFER,
			L"Buffer_UI"));
	}

	// View ---------------
	m_tSprite_Frame.m_matView._11 = SIZE_FRAME_X_HALF;
	m_tSprite_Frame.m_matView._22 = SIZE_FRAME_Y_HALF;
	m_tSprite_Frame.m_matView._33 = 1.f;
	m_tSprite_Frame.m_matView._41 = CENTER_POS.x - (CLIENT_WINCX >> 1);
	m_tSprite_Frame.m_matView._42 = -CENTER_POS.y + (CLIENT_WINCY >> 1);

	m_tSprite_Circle[STATE_BASIC].m_matView._11 = SIZE_CICLE_BASIC_HALF;
	m_tSprite_Circle[STATE_BASIC].m_matView._22 = SIZE_CICLE_BASIC_HALF;
	m_tSprite_Circle[STATE_BASIC].m_matView._33 = 1.f;
	m_tSprite_Circle[STATE_BASIC].m_matView._41 = m_tSprite_Frame.m_matView._41;
	m_tSprite_Circle[STATE_BASIC].m_matView._42 = m_tSprite_Frame.m_matView._42;

	m_tSprite_Circle[STATE_TARGETING].m_matView._11 = SIZE_CICLE_TARGETING_HALF;
	m_tSprite_Circle[STATE_TARGETING].m_matView._22 = SIZE_CICLE_TARGETING_HALF;
	m_tSprite_Circle[STATE_TARGETING].m_matView._33 = 1.f;
	m_tSprite_Circle[STATE_TARGETING].m_matView._41 = m_tSprite_Frame.m_matView._41;
	m_tSprite_Circle[STATE_TARGETING].m_matView._42 = m_tSprite_Frame.m_matView._42;

	for (int i = 0; i < STATE_END; ++i)
	{
		m_tSprite_Point[i].m_matView._11 = 21.5f;
		m_tSprite_Point[i].m_matView._22 = 17.5f;
		m_tSprite_Point[i].m_matView._33 = 1.f;
		m_tSprite_Point[i].m_matView._41 = m_tSprite_Frame.m_matView._41;
		m_tSprite_Point[i].m_matView._42 = m_tSprite_Frame.m_matView._42;
	}

	// Dir -------------
	D3DXVec3TransformNormal(&m_pPhysics->m_vDir, &g_vLook, &m_pPhysics->m_matWorld);

	// Observer ---------
	m_pInfoSubject->AddData(CInfoSubject::DATA_TYPE_UI_STATIE, &m_eState);
	m_pInfoSubject->AddData(CInfoSubject::DATA_TYPE_UI_DIR_TO_TARGET, &m_vDirToTarget);


	return S_OK;
}

int CTargetBox::Update()
{
	if (PLAYING != m_pServerMgr->GetState().gamestate)
		return m_eObjState;


	if (m_eState == STATE_BASIC)
		m_fCircleAngle += ROTATE_SPEED_CIRCLE * CTimer::GetInstance()->GetTime();

	Update_SizeByZoom();
	CheckEnemy();
	MoveToTarget();

	m_pInfoSubject->Notify(CInfoSubject::DATA_TYPE_UI_STATIE);

	return m_eObjState;
}

void CTargetBox::Render()
{
	if (m_bGameEnd)
		return;


	if (m_eState == STATE_BASIC)
		m_pPhysics->m_vRadian.z = m_fCircleAngle;

	m_pPhysics->Render();
	Invalidate_View(m_tSprite_Circle[m_eState].m_matView);
	m_tSprite_Circle[m_eState].pTexture->Render();
	m_tSprite_Circle[m_eState].pBuffer->Render();


	m_pPhysics->m_vRadian.z = 0.f;
	m_pPhysics->Render();

	Invalidate_View(m_tSprite_Point[m_eState].m_matView);
	m_tSprite_Point[m_eState].pTexture->Render();
	m_tSprite_Point[m_eState].pBuffer->Render();

	Invalidate_View(m_tSprite_Frame.m_matView);
	m_tSprite_Frame.pTexture->Render();
	m_tSprite_Frame.pBuffer->Render();
}

void CTargetBox::Update_SizeByZoom()
{
	float	fZoom = m_pCameraObserver->GetZoom();
	if (CStaticCamera::MIN_ZOOM_VALUE > fZoom)
		return;

	float fSize = CStaticCamera::MIN_ZOOM_VALUE / fZoom * 1.5f;
	if (1 <= fSize)
		return;

	m_tSprite_Frame.m_matView._11 = SIZE_FRAME_X_HALF * fSize;
	m_tSprite_Frame.m_matView._22 = SIZE_FRAME_Y_HALF * fSize;
}


void CTargetBox::CheckEnemy()
{
	SC_PlayerPos	tPlayerPos = m_pServerMgr->GetPlayerPos();
	CS_Rotate		tPlayerRotate = m_pPlayerObserver->GetRotate();
	SC_PlayerPos	tEnemyPos[MAX_USER_NUM];

	D3DXVECTOR3		vEnemyPosInScreen(0.f, 0.f, 0.f);
	float			fRadian = 0.f;
	float			fMinDistanceFromCenter = -1;
	float			fCurDistanceFromCenter = 0.f;

	D3DXVECTOR2		vResultTargetPos(-1.f, -1.f);


	for (int i = 0; i < MAX_USER_NUM; ++i)
	{
		tEnemyPos[i] = m_pServerMgr->GetEnemyPos(i);

		if (-1 >= tEnemyPos[i].ID)
			continue;


		m_vDirToTarget = tEnemyPos[i].move - tPlayerPos.move;
	//m_vDirToTarget = D3DXVECTOR3(4.9f, 60.5f, 320.f) - tPlayerPos.move;

	if (DISTANCE_INDUCTION < D3DXVec3Length(&m_vDirToTarget))
	{
		m_vTargetPos = vResultTargetPos;
		continue;
		//return;
	}


		D3DXVec3Normalize(&m_vDirToTarget, &m_vDirToTarget);
		D3DXVec3Normalize(&tPlayerRotate.dir, &tPlayerRotate.dir);

		fRadian = D3DXVec3Dot(&m_vDirToTarget, &tPlayerRotate.dir);

		if (0 > D3DXToDegree(fRadian))
		{
			m_vTargetPos = vResultTargetPos;
			//continue;
			return;
		}


		vEnemyPosInScreen = ::Change3DCoordTo2DCoord(
			tEnemyPos[i].move,
			//D3DXVECTOR3(4.9f, 60.5f, 320.f),
			m_pCameraObserver->GetView(),
			m_pCameraObserver->GetProj());


		if (CheckInTargetBox(vEnemyPosInScreen))
		{
			fCurDistanceFromCenter = D3DXVec3Length(
				&(vEnemyPosInScreen - D3DXVECTOR3((CLIENT_WINCX >> 1), POS_Y, 0.f)));

			if (-1 == fMinDistanceFromCenter)
			{
				fMinDistanceFromCenter = fCurDistanceFromCenter;
				vResultTargetPos.x = vEnemyPosInScreen.x;
				vResultTargetPos.y = vEnemyPosInScreen.y;
			}
			else
			{
				if (fMinDistanceFromCenter > fCurDistanceFromCenter)
				{
					fMinDistanceFromCenter = fCurDistanceFromCenter;
					vResultTargetPos.x = vEnemyPosInScreen.x;
					vResultTargetPos.y = vEnemyPosInScreen.y;
				}
			}
		}
	}


	m_vTargetPos = vResultTargetPos;
}

bool CTargetBox::CheckInTargetBox(const D3DXVECTOR3& _vPos)
{
	static D3DXVECTOR2 vFramePos((CLIENT_WINCX >> 1), POS_Y);

	if (_vPos.x >= vFramePos.x - m_tSprite_Frame.m_matView._11
		&& _vPos.x <= vFramePos.x + m_tSprite_Frame.m_matView._11
		&& _vPos.y >= vFramePos.y - m_tSprite_Frame.m_matView._22
		&& _vPos.y <= vFramePos.y + m_tSprite_Frame.m_matView._22)
	{
		return true;
	}

	return false;
}

void CTargetBox::MoveToTarget()
{
	static const float			RANGE_CHECK_POS_TO_CENTER	= 1.f;

	// 타겟이 없을 경우 센터로 이동
	if (-1.f >= m_vTargetPos.x)
	{
		m_eState = STATE_BASIC;
		
		if (m_tSprite_Circle[STATE_BASIC].m_matView._41 > m_tSprite_Frame.m_matView._41 - RANGE_CHECK_POS_TO_CENTER
			&& m_tSprite_Circle[STATE_BASIC].m_matView._41 < m_tSprite_Frame.m_matView._41 + RANGE_CHECK_POS_TO_CENTER
			&& m_tSprite_Circle[STATE_BASIC].m_matView._42 > m_tSprite_Frame.m_matView._42 - RANGE_CHECK_POS_TO_CENTER
			&& m_tSprite_Circle[STATE_BASIC].m_matView._42 < m_tSprite_Frame.m_matView._42 + RANGE_CHECK_POS_TO_CENTER)
		{
			m_tSprite_Circle[STATE_BASIC].m_matView._41 = m_tSprite_Frame.m_matView._41;
			m_tSprite_Circle[STATE_BASIC].m_matView._42 = m_tSprite_Frame.m_matView._42;
			m_tSprite_Point[STATE_BASIC].m_matView._41 = m_tSprite_Frame.m_matView._41;
			m_tSprite_Point[STATE_BASIC].m_matView._42 = m_tSprite_Frame.m_matView._42;

			return;
		}

		D3DXVECTOR2 vPos(
			m_tSprite_Circle[STATE_BASIC].m_matView._41 + (CLIENT_WINCX >> 1),
			-(m_tSprite_Circle[STATE_BASIC].m_matView._42 - (CLIENT_WINCY >> 1)));

		D3DXVECTOR2 vDir = CENTER_POS - vPos;
		D3DXVec2Normalize(&vDir, &vDir);


		float fTime = CTimer::GetInstance()->GetTime();

		m_tSprite_Circle[STATE_BASIC].m_matView._41 += vDir.x * SPEED_MOVE_TO_TARGET * fTime;
		m_tSprite_Circle[STATE_BASIC].m_matView._42 -= vDir.y * SPEED_MOVE_TO_TARGET * fTime;
		m_tSprite_Point[STATE_BASIC].m_matView._41 += vDir.x * SPEED_MOVE_TO_TARGET * fTime;
		m_tSprite_Point[STATE_BASIC].m_matView._42 -= vDir.y * SPEED_MOVE_TO_TARGET * fTime;

		return;
	}


	m_eState = STATE_BASIC;

	D3DXVECTOR2 vPos(
		m_tSprite_Circle[STATE_BASIC].m_matView._41 + (CLIENT_WINCX >> 1),
		-(m_tSprite_Circle[STATE_BASIC].m_matView._42 - (CLIENT_WINCY >> 1)));

	D3DXVECTOR2 vDir = m_vTargetPos - vPos;
	D3DXVec2Normalize(&vDir, &vDir);


	float fTime = CTimer::GetInstance()->GetTime();

	m_tSprite_Circle[STATE_BASIC].m_matView._41 += vDir.x * SPEED_MOVE_TO_TARGET * fTime;
	m_tSprite_Circle[STATE_BASIC].m_matView._42 -= vDir.y * SPEED_MOVE_TO_TARGET * fTime;
	m_tSprite_Point[STATE_BASIC].m_matView._41 += vDir.x * SPEED_MOVE_TO_TARGET * fTime;
	m_tSprite_Point[STATE_BASIC].m_matView._42 -= vDir.y * SPEED_MOVE_TO_TARGET * fTime;


	if (vPos.x > m_vTargetPos.x - RANGE_CHECK_POS_TO_CENTER
		&& vPos.x < m_vTargetPos.x + RANGE_CHECK_POS_TO_CENTER
		&& vPos.y > m_vTargetPos.y - RANGE_CHECK_POS_TO_CENTER
		&& vPos.y < m_vTargetPos.y + RANGE_CHECK_POS_TO_CENTER)
	{
		m_tSprite_Circle[STATE_BASIC].m_matView._41 = m_vTargetPos.x - (CLIENT_WINCX >> 1);
		m_tSprite_Circle[STATE_BASIC].m_matView._42 = -m_vTargetPos.y + (CLIENT_WINCY >> 1);
		m_tSprite_Point[STATE_BASIC].m_matView._41 = m_vTargetPos.x - (CLIENT_WINCX >> 1);
		m_tSprite_Point[STATE_BASIC].m_matView._42 = -m_vTargetPos.y + (CLIENT_WINCY >> 1);

		m_tSprite_Circle[STATE_TARGETING].m_matView._41 = m_vTargetPos.x - (CLIENT_WINCX >> 1);
		m_tSprite_Circle[STATE_TARGETING].m_matView._42 = -m_vTargetPos.y + (CLIENT_WINCY >> 1);
		m_tSprite_Point[STATE_TARGETING].m_matView._41 = m_vTargetPos.x - (CLIENT_WINCX >> 1);
		m_tSprite_Point[STATE_TARGETING].m_matView._42 = -m_vTargetPos.y + (CLIENT_WINCY >> 1);

		m_eState = STATE_TARGETING;
		m_pInfoSubject->Notify(CInfoSubject::DATA_TYPE_UI_DIR_TO_TARGET);

		return;
	}
}

void CTargetBox::Release()
{
	::Safe_Delete(m_tSprite_Frame.pTexture);
	::Safe_Delete(m_tSprite_Frame.pBuffer);

	for (int i = 0; i < STATE_END; ++i)
	{
		::Safe_Delete(m_tSprite_Circle[i].pTexture);
		::Safe_Delete(m_tSprite_Circle[i].pBuffer);
		::Safe_Delete(m_tSprite_Point[i].pTexture);
		::Safe_Delete(m_tSprite_Point[i].pBuffer);
	}
}
