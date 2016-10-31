#include "stdafx.h"
#include "UIBuffer.h"

#include "Texture.h"
#include "TextureShader.h"

#include "ResourceMgr.h"


CUIBuffer::CUIBuffer(CDevice* _pDevice)
: CBuffer(_pDevice)
, m_pTextureShader(NULL)
{

}

CUIBuffer::~CUIBuffer()
{
	Release();
}

CResource* CUIBuffer::Create(CDevice* _pDevice)
{
	CUIBuffer* pComponent = new CUIBuffer(_pDevice);
	if (FAILED(pComponent->Init()))
		::Safe_Delete(pComponent);


	return pComponent;
}

CResource* CUIBuffer::Clone()
{
	++(*m_pRefCnt);
	return new CUIBuffer(*this);
}

HRESULT CUIBuffer::Init()
{
	m_nVtxNum = 4;
	m_nVtxStride = sizeof(VertexTexture);
	m_nVtxOffset = 0;

	m_nIdxNum = 6;
	m_nIdxStart = 0;
	m_nIdxPlus = 0;

	Init_Vtx();
	Init_Idx();
	Init_TextureShader();
	CreateRasterizerState();

	return S_OK;
}

void CUIBuffer::Update()
{

}

void CUIBuffer::Render()
{
	m_pTextureShader->Render();
	CBuffer::Render();
}

void CUIBuffer::Release()
{
	if ((*m_pRefCnt) == 0)
	{
		::Safe_Release(m_pVtxBuffer);
		::Safe_Release(m_pIdxBuffer);
		::Safe_Delete(m_pTextureShader);
		CResource::Release_RefCnt();
	}
	else --(*m_pRefCnt);
}

void CUIBuffer::Init_Vtx()
{
	VertexTexture pVertex[] =
	{
		{ D3DXVECTOR3(-1.f, 1.f, -1.f)
		, D3DXVECTOR3(0.f, 1.f, 0.f)
		, D3DXVECTOR2(0.f, 0.f) },

		{ D3DXVECTOR3(1.f, 1.f, -1.f)
		, D3DXVECTOR3(0.f, 1.f, 0.f)
		, D3DXVECTOR2(1.f, 0.f) },

		{ D3DXVECTOR3(1.f, -1.f, -1.f)
		, D3DXVECTOR3(0.f, 1.f, 0.f)
		, D3DXVECTOR2(1.f, 1.f) },

		{ D3DXVECTOR3(-1.f, -1.f, -1.f)
		, D3DXVECTOR3(0.f, 1.f, 0.f)
		, D3DXVECTOR2(0.f, 1.f) },
	};


	D3D11_BUFFER_DESC tBufferDesc;
	ZeroMemory(&tBufferDesc, sizeof(D3D11_BUFFER_DESC));
	tBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	tBufferDesc.ByteWidth = m_nVtxStride * m_nVtxNum;
	tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA tData;
	ZeroMemory(&tData, sizeof(D3D11_SUBRESOURCE_DATA));
	tData.pSysMem = pVertex;
	FAILED_CHECK_RETURN(
		m_pDevice->GetDevice()->CreateBuffer(&tBufferDesc, &tData, &m_pVtxBuffer), );
}

void CUIBuffer::Init_Idx()
{
	Index16 pIdx[] =
	{
		{ Index16(0, 1, 2) },
		{ Index16(0, 2, 3) }
	};


	D3D11_BUFFER_DESC tBufferDesc;
	ZeroMemory(&tBufferDesc, sizeof(D3D11_BUFFER_DESC));
	tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	tBufferDesc.ByteWidth = sizeof(DWORD)* m_nIdxNum;
	tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	tBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA tData;
	ZeroMemory(&tData, sizeof(D3D11_SUBRESOURCE_DATA));
	tData.pSysMem = pIdx;
	FAILED_CHECK_RETURN(
		m_pDevice->GetDevice()->CreateBuffer(&tBufferDesc, &tData, &m_pIdxBuffer), );
}

HRESULT CUIBuffer::Init_TextureShader()
{
	m_pTextureShader = dynamic_cast<CTextureShader*>(CResourceMgr::GetInstance()->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC, CResourceMgr::RESOURCE_TYPE_SHADER, L"Shader_UI"));
	NULL_CHECK_RETURN(m_pTextureShader, E_FAIL);

	return S_OK;
}

void CUIBuffer::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC tRasterizerDesc;
	ZeroMemory(&tRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	tRasterizerDesc.CullMode = D3D11_CULL_NONE;
	tRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	m_pDevice->GetDevice()->CreateRasterizerState(&tRasterizerDesc, &m_pRasterizerState);
}