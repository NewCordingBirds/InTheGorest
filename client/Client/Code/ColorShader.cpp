#include "stdafx.h"
#include "ColorShader.h"


CColorShader::CColorShader(CDevice* _pDevice)
: CShader(_pDevice)
{

}

CColorShader::~CColorShader()
{
	Release();
}

CResource* CColorShader::Create(CDevice* _pDevice)
{
	CColorShader* pComponent = new CColorShader(_pDevice);
	if (FAILED(pComponent->Init()))
		::Safe_Delete(pComponent);

	return pComponent;
}

CResource* CColorShader::Clone()
{
	++(*m_pRefCnt);
	return new CColorShader(*this);
}

HRESULT CColorShader::Init()
{
	D3D11_INPUT_ELEMENT_DESC tInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT iElementNum = ARRAYSIZE(tInputLayout);


	DWORD swFlag =  D3DCOMPILE_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )
	swFlag |= D3D10_SHADER_DEBUG;
#endif

	ID3DBlob* pBlob = NULL, *pErrorBlob = NULL;
	ID3D11Device* pDevice = m_pDevice->GetDevice();

	if (SUCCEEDED(D3DX11CompileFromFile(L"../bin/Data/Fx/VertexColor.fx", NULL, NULL,
		"VS", "vs_4_0", swFlag, 0, NULL, &pBlob, &pErrorBlob, NULL)))
	{
		pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pVertexShader);
		pDevice->CreateInputLayout(
			tInputLayout, iElementNum, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_pVertexLayout);
	}
	else
	{
		if (pErrorBlob) { FAILED_CHECK_MSG(E_FAIL, L"셰이더 컴파일 실패"); }
		else { FAILED_CHECK_MSG(E_FAIL, L"셰이더 파일이 존재하지 않습니다.");  }
		return E_FAIL;
	}

	pBlob = pErrorBlob = NULL;
	if (SUCCEEDED(D3DX11CompileFromFile(L"../bin/Data/Fx/VertexColor.fx", NULL, NULL,
		"PS", "ps_4_0", swFlag, 0, NULL, &pBlob, &pErrorBlob, NULL)))
	{
		pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pPixelShader);
	}
	else
	{
		if (pErrorBlob) { FAILED_CHECK_MSG(E_FAIL, L"셰이더 컴파일 실패"); }
		else { FAILED_CHECK_MSG(E_FAIL, L"셰이더 파일이 존재하지 않습니다."); }
		return E_FAIL;
	}
	::Safe_Release(pBlob);
	::Safe_Release(pErrorBlob);

	return S_OK;
}


void CColorShader::Update()
{

}

void CColorShader::Release()
{

}
