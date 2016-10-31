#include "stdafx.h"
#include "StaticCamera.h"

#include "KeyMgr.h"
#include "Timer.h"

const float CStaticCamera::MAX_ZOOM_VALUE		= 5.f;
const float CStaticCamera::MIN_ZOOM_VALUE		= 2.f;


CStaticCamera::CStaticCamera(CDevice* _pDevice)
: CCamera(_pDevice)

, m_pKeyMgr(CKeyMgr::GetInstance())
, m_pTimer(CTimer::GetInstance())

, m_pTargetPhysics(NULL)

, m_eWheelEvent(WHEEL_EVENT_NONE)
, m_fTargetZoom(0.f)

, m_fZoom(0.f)
{

}

CStaticCamera::~CStaticCamera()
{
	Release();
}

CObj* CStaticCamera::Create(CDevice* _pDevice)
{
	CStaticCamera* pCamera = new CStaticCamera(_pDevice);
	if (FAILED(pCamera->Init()))
		::Safe_Delete(pCamera);

	return pCamera;
}

HRESULT CStaticCamera::Init()
{
	m_eObjState = CObj::OBJ_STATE_ALIVE;

	m_fFovy = (float)D3DXToRadian(90.f);
	m_fAspect = float(CLIENT_WINCX) / CLIENT_WINCY;
	m_fNear = 1.f;
	m_fFar = 2400.f;

	m_fZoom = MAX_ZOOM_VALUE;
	m_fTargetZoom = MIN_ZOOM_VALUE;
	//m_fTargetZoom = m_fZoom;


	CCamera::Invalidate_View();
	CCamera::Invalidate_Proj();
	CCamera::Invalidate_Eye();


	// Observer ------------
	CCamera::InitInfoSubject();
	m_pInfoSubject->AddData(CInfoSubject::DATA_TYPE_CAMERA_ZOOM, &m_fZoom);

	m_pInfoSubject->Notify(CInfoSubject::DATA_TYPE_CAMERA_PROJECTION);
	m_pInfoSubject->Notify(CInfoSubject::DATA_TYPE_CAMERA_ZOOM);

	return S_OK;
}

int CStaticCamera::Update()
{
	CheckKey();
	MoveToTarget();
	AnimatingWheel();
	CCamera::Invalidate_View();

	m_pInfoSubject->Notify(CInfoSubject::DATA_TYPE_CAMERA_VIEW);
	m_pInfoSubject->Notify(CInfoSubject::DATA_TYPE_CAMERA_PROJECTION);
	m_pInfoSubject->Notify(CInfoSubject::DATA_TYPE_CAMERA_EYE);


	return m_eObjState;
}

void CStaticCamera::Render()
{
	CCamera::Invalidate_Eye();
}

void CStaticCamera::Release()
{

}

void CStaticCamera::CheckKey()
{
	int		iDistance = 0;
	float	fTime = m_pTimer->GetTime();

	if (iDistance = m_pKeyMgr->GetDIMouseMove(CKeyMgr::MOUSE_MOVE_Z))
	{
		m_fTargetZoom -= iDistance * fTime;

		if (m_fTargetZoom > MAX_ZOOM_VALUE)
			m_fTargetZoom = MAX_ZOOM_VALUE;
		else if (m_fTargetZoom < MIN_ZOOM_VALUE)
			m_fTargetZoom = MIN_ZOOM_VALUE;
	}
}

void CStaticCamera::MoveToTarget()
{
	if (m_pTargetPhysics == NULL)
		return;

	//const static float	DISTANCE_VALUE_EYE = 6.f;
	//const static float	DISTANCE_VALUE_EYE_Y = 12.f;
	//const static float	DISTANCE_VALUE_AT_Y = 9.f;


	//m_vEye = m_pTargetPhysics->m_vPos + (-(m_pTargetPhysics->m_vDir) * DISTANCE_VALUE_EYE * m_fZoom);
	//m_vEye.y += DISTANCE_VALUE_EYE_Y;

	//m_vAt = m_pTargetPhysics->m_vPos;
	//m_vAt.y += DISTANCE_VALUE_AT_Y;


	const static float	DISTANCE_VALUE_EYE = 10.f;
	const static float	DISTANCE_VALUE_AT_Y = 12.f;

	m_vEye = -(m_pTargetPhysics->m_vDir) * DISTANCE_VALUE_EYE * m_fZoom;

	D3DXVECTOR3		vTargetRight;
	memcpy(&vTargetRight, &(m_pTargetPhysics->m_matWorld.m[0][0]), sizeof(D3DXVECTOR3));

	D3DXMATRIX		matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &vTargetRight, D3DXToRadian(30.f));
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);

	m_vAt = m_pTargetPhysics->m_vPos;
	m_vAt.y += DISTANCE_VALUE_AT_Y;
	m_vEye += m_pTargetPhysics->m_vPos;
}

void CStaticCamera::AnimatingWheel()
{
	//const float SPEED_WHEEL = 10.f;
	const float SPEED_WHEEL = 8.f;
	
	if (m_fZoom == m_fTargetZoom)		m_eWheelEvent = WHEEL_EVENT_NONE;
	else if (m_fZoom < m_fTargetZoom)	m_eWheelEvent = WHEEL_EVENT_UP;
	else if (m_fZoom > m_fTargetZoom)	m_eWheelEvent = WHEEL_EVENT_DOWN;


	switch (m_eWheelEvent)
	{
	case WHEEL_EVENT_NONE:
		break;

	case WHEEL_EVENT_UP:
		if (m_fZoom < MAX_ZOOM_VALUE)
			m_fZoom += SPEED_WHEEL * m_pTimer->GetTime();
		if (m_fZoom > m_fTargetZoom)
		{
			m_eWheelEvent = WHEEL_EVENT_NONE;
			m_fZoom = m_fTargetZoom;
		}
		m_pInfoSubject->Notify(CInfoSubject::DATA_TYPE_CAMERA_ZOOM);
		break;

	case WHEEL_EVENT_DOWN:
		if (m_fZoom > MIN_ZOOM_VALUE)
			m_fZoom -= SPEED_WHEEL * m_pTimer->GetTime();
		if (m_fZoom < m_fTargetZoom)
		{
			m_eWheelEvent = WHEEL_EVENT_NONE;
			m_fZoom = m_fTargetZoom;
		}
		m_pInfoSubject->Notify(CInfoSubject::DATA_TYPE_CAMERA_ZOOM);
		break;
	}
}



