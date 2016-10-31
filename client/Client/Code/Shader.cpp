#include "stdafx.h"
#include "Shader.h"

CShader::CShader(CDevice* _pDevice)
: CResource(_pDevice)
, m_pVertexShader(NULL)
, m_pVertexLayout(NULL)

, m_pPixelShader(NULL)
{

}

CShader::~CShader()
{
	Release();
}

void CShader::Render()
{
	ID3D11DeviceContext* pDeviceContext = m_pDevice->GetDeviceContext();

	if (m_pVertexLayout)
		pDeviceContext->IASetInputLayout(m_pVertexLayout);
	if (m_pVertexShader)
		pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	if (m_pPixelShader)
		pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
}

void CShader::Release()
{
	if ((*m_pRefCnt) == 0)
	{
		::Safe_Release(m_pVertexShader);
		::Safe_Release(m_pVertexLayout);
		::Safe_Release(m_pPixelShader);
		CResource::Release_RefCnt();
	}
	else --(*m_pRefCnt);
}
