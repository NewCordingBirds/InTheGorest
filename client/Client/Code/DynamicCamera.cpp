#include "stdafx.h"
#include "DynamicCamera.h"

#include "KeyMgr.h"
#include "Timer.h"

extern HWND		g_hWnd;


CDynamicCamera::CDynamicCamera(CDevice* _pDevice)
: CCamera(_pDevice)
, m_pKeyMgr(CKeyMgr::GetInstance())
, m_pTimer(CTimer::GetInstance())

, m_fSpeed(0.f)
{

}

CDynamicCamera::~CDynamicCamera()
{
	Release();
}

CObj* CDynamicCamera::Create(CDevice* _pDevice)
{
	CDynamicCamera* pCamera = new CDynamicCamera(_pDevice);
	if (FAILED(pCamera->Init()))
		::Safe_Delete(pCamera);

	return pCamera;
}

HRESULT CDynamicCamera::Init()
{
	m_eObjState = CObj::OBJ_STATE_ALIVE;
	m_fSpeed = 100.f;

	m_fFovy = (float)D3DXToRadian(90.f);
	m_fAspect = float(CLIENT_WINCX) / CLIENT_WINCY;
	m_fNear = 1.f;
	m_fFar = 2400.f;

	CCamera::InitInfoSubject();

	CCamera::Invalidate_View();
	CCamera::Invalidate_Proj();


	return S_OK;
}

int CDynamicCamera::Update()
{
	CheckKey();
	//FixMouse();
	CCamera::Invalidate_View();

	return m_eObjState;
}

void CDynamicCamera::Render()
{

}

void CDynamicCamera::Release()
{

}

void CDynamicCamera::CheckKey()
{
	float fTime = m_pTimer->GetTime();

	CheckKey_Move(fTime);
	CheckKey_Rotate(fTime);
}

void CDynamicCamera::CheckKey_Move(float _fTime)
{
	float fTime = m_pTimer->GetTime();

	if (m_pKeyMgr->CheckKeyboardPress(DIK_A))
	{
		D3DXVECTOR3		vRight;
		D3DXMATRIX		matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		m_vEye -= vRight * m_fSpeed * _fTime;
		m_vAt -= vRight * m_fSpeed * _fTime;
	}

	if (m_pKeyMgr->CheckKeyboardPress(DIK_D))
	{
		D3DXVECTOR3		vRight;
		D3DXMATRIX		matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		m_vEye += vRight * m_fSpeed * _fTime;
		m_vAt += vRight * m_fSpeed * _fTime;
	}

	if (m_pKeyMgr->CheckKeyboardPress(DIK_W))
	{
		D3DXVECTOR3		vLook;
		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		m_vEye += vLook * m_fSpeed * _fTime;
		m_vAt += vLook * m_fSpeed * _fTime;
	}

	if (m_pKeyMgr->CheckKeyboardPress(DIK_S))
	{
		D3DXVECTOR3		vLook;
		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		m_vEye -= vLook * m_fSpeed * _fTime;
		m_vAt -= vLook * m_fSpeed * _fTime;
	}
}

void CDynamicCamera::CheckKey_Rotate(float _fTime)
{
	int		iDistance = 0;

	if (iDistance = m_pKeyMgr->GetDIMouseMove(CKeyMgr::MOUSE_MOVE_X))
	{
		D3DXMATRIX		matAxis;
		D3DXMatrixRotationY(&matAxis, (float)D3DXToRadian(iDistance / 10.f));

		D3DXVECTOR3		vDir;
		vDir = m_vAt - m_vEye;
		D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

		m_vAt = m_vEye + vDir;
	}

	if (iDistance = m_pKeyMgr->GetDIMouseMove(CKeyMgr::MOUSE_MOVE_Y))
	{
		D3DXVECTOR3		vRight;
		D3DXMATRIX		matViewInverse;

		D3DXMatrixInverse(&matViewInverse, NULL, &m_matView);
		memcpy(&vRight, &matViewInverse.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		D3DXMATRIX		matAxis;
		D3DXMatrixRotationAxis(&matAxis, &vRight, (float)D3DXToRadian(iDistance / 10.f));

		D3DXVECTOR3		vDir;
		vDir = m_vAt - m_vEye;
		D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

		m_vAt = m_vEye + vDir;
	}
}

void CDynamicCamera::FixMouse()
{
	POINT	ptMouse = { ::CLIENT_WINCX >> 1, ::CLIENT_WINCY >> 1 };
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}


