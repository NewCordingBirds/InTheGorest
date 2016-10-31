#include "stdafx.h"
#include "AniShader.h"

CAniShader::CAniShader(CDevice* _pDevice)
: CShader(_pDevice)
{

}

CAniShader::~CAniShader()
{
	Release();
}

CResource* CAniShader::Create(CDevice* _pDevice)
{
	CAniShader* pComponent = new CAniShader(_pDevice);
	if (FAILED(pComponent->Init()))
		::Safe_Delete(pComponent);

	return pComponent;
}

CResource* CAniShader::Clone()
{
	++(*m_pRefCnt);
	return new CAniShader(*this);
}

HRESULT CAniShader::Init()
{
	D3D11_INPUT_ELEMENT_DESC tInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONES", 1, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iElementNum = ARRAYSIZE(tInputLayout);


	DWORD swFlag = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )
	swFlag |= D3D10_SHADER_DEBUG;
#endif

	ID3DBlob* pBlob = NULL, *pErrorBlob = NULL;
	ID3D11Device* pDevice = m_pDevice->GetDevice();

	if (SUCCEEDED(D3DX11CompileFromFile(L"../bin/Data/Fx/VertexAni.fx", NULL, NULL,
		"VS", "vs_4_0", swFlag, 0, NULL, &pBlob, &pErrorBlob, NULL)))
	{
		pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pVertexShader);
		pDevice->CreateInputLayout(
			tInputLayout, iElementNum, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_pVertexLayout);
	}
	else
	{
		if (pErrorBlob) { FAILED_CHECK_MSG(E_FAIL, L"셰이더 컴파일 실패"); }
		else { FAILED_CHECK_MSG(E_FAIL, L"셰이더 파일이 존재하지 않습니다."); }
		return E_FAIL;
	}

	pBlob = pErrorBlob = NULL;
	if (SUCCEEDED(D3DX11CompileFromFile(L"../bin/Data/Fx/VertexAni.fx", NULL, NULL,
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


void CAniShader::Update()
{

}

void CAniShader::Release()
{

}