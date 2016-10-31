#include "stdafx.h"
#include "UI.h"


CUI::CUI(CDevice* _pDevice)
: CObj(_pDevice)
, m_pDeviceContext(_pDevice->GetDeviceContext())
{
	Init_ViewBuffer();
	Init_ProjBuffer();
	Init_CameraObserver();
}

CUI::~CUI()
{
	Release_CameraObserver();
	::Safe_Release(m_pViewBuffer);
	::Safe_Release(m_pProjBuffer);
}

void CUI::Init_CameraObserver()
{
	m_pCameraObserver = CCameraObserver::Create();
	CInfoSubject::GetInstance()->Subscribe(m_pCameraObserver);
}

void CUI::Init_ViewBuffer()
{
	D3D11_BUFFER_DESC tBuffer;
	ZeroMemory(&tBuffer, sizeof(D3D11_BUFFER_DESC));

	tBuffer.Usage = D3D11_USAGE_DYNAMIC;
	tBuffer.ByteWidth = sizeof(Buffer_View);
	tBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	FAILED_CHECK_RETURN(
		m_pDevice->GetDevice()->CreateBuffer(&tBuffer, NULL, &m_pViewBuffer), );
}

void CUI::Init_ProjBuffer()
{
	D3D11_BUFFER_DESC tBuffer;
	ZeroMemory(&tBuffer, sizeof(D3D11_BUFFER_DESC));

	tBuffer.Usage = D3D11_USAGE_DYNAMIC;
	tBuffer.ByteWidth = sizeof(Buffer_Proj);
	tBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	FAILED_CHECK_RETURN(
		m_pDevice->GetDevice()->CreateBuffer(&tBuffer, NULL, &m_pProjBuffer), );
}

void CUI::Release_CameraObserver()
{
	CInfoSubject::GetInstance()->Unsubscribe(m_pCameraObserver);
	::Safe_Delete(m_pCameraObserver);
}

void CUI::Invalidate_View(D3DXMATRIX _matView)
{
	D3D11_MAPPED_SUBRESOURCE tSubreResource;
	m_pDeviceContext->Map(m_pViewBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSubreResource);

	Buffer_View* pMatView = (Buffer_View*)tSubreResource.pData;
	D3DXMatrixTranspose(&pMatView->m_matView, &_matView);

	m_pDeviceContext->Unmap(m_pViewBuffer, 0);
	m_pDeviceContext->VSSetConstantBuffers(VS_SLOT_VIEW_MATRIX, 1, &m_pViewBuffer);
}