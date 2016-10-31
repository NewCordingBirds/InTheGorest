#include "stdafx.h"
#include "SkyBoxBuffer.h"

#include "ResourceMgr.h"
#include "SkyBoxShader.h"


CSkyBoxBuffer::CSkyBoxBuffer(CDevice* _pDevice)
: CBuffer(_pDevice)
, m_pSkyBoxShader(NULL)
{

}

CSkyBoxBuffer::~CSkyBoxBuffer()
{
	Release();
}

CResource* CSkyBoxBuffer::Create(CDevice* _pDevice)
{
	CSkyBoxBuffer* pComponent = new CSkyBoxBuffer(_pDevice);
	if (FAILED(pComponent->Init()))
		::Safe_Delete(pComponent);

	return pComponent;
}

CResource* CSkyBoxBuffer::Clone()
{
	++(*m_pRefCnt);
	return new CSkyBoxBuffer(*this);
}

HRESULT CSkyBoxBuffer::Init()
{
	m_nVtxNum = 8;
	m_nVtxStride = sizeof(VertexSkyBox);
	m_nVtxOffset = 0;
	m_nVtxStart = 1;

	m_nIdxNum = 36;
	m_nIdxStart = 0;
	m_nIdxPlus = 0;

	Init_Vtx();
	Init_Idx();
	Init_SkyBoxShader();
	CreateRasterizerState();

	return S_OK;
}

void CSkyBoxBuffer::Init_Vtx()
{
	VertexSkyBox pVertex[] =
	{
		{ D3DXVECTOR3(-1.f, 1.f, -1.f)
		, D3DXVECTOR2(0.f, 0.f) },

		{ D3DXVECTOR3(1.f, 1.f, -1.f)
		, D3DXVECTOR2(1.f, 0.f) },

		{ D3DXVECTOR3(1.f, -1.f, -1.f)
		, D3DXVECTOR2(1.f, 1.f) },

		{ D3DXVECTOR3(-1.f, -1.f, -1.f)
		, D3DXVECTOR2(0.f, 1.f) },

		{ D3DXVECTOR3(-1.f, 1.f, 1.f)
		, D3DXVECTOR2(0.f, 0.f) },

		{ D3DXVECTOR3(1.f, 1.f, 1.f)
		, D3DXVECTOR2(1.f, 0.f) },

		{ D3DXVECTOR3(1.f, -1.f, 1.f)
		, D3DXVECTOR2(1.f, 1.f) },

		{ D3DXVECTOR3(-1.f, -1.f, 1.f)
		, D3DXVECTOR2(0.f, 1.f) }
	};


	D3D11_BUFFER_DESC tBufferDesc;
	ZeroMemory(&tBufferDesc, sizeof(D3D11_BUFFER_DESC));
	tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	tBufferDesc.ByteWidth = m_nVtxStride * m_nVtxNum;
	tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tBufferDesc.CPUAccessFlags = 0;
	tBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA tData;
	ZeroMemory(&tData, sizeof(D3D11_SUBRESOURCE_DATA));
	tData.pSysMem = pVertex;
	FAILED_CHECK_RETURN(
		m_pDevice->GetDevice()->CreateBuffer(&tBufferDesc, &tData, &m_pVtxBuffer), );
}

void CSkyBoxBuffer::Init_Idx()
{
	Index16 pIdx[] =
	{
		// Front
		{ Index16(0, 1, 2) },
		{ Index16(0, 2, 3) },

		// Right
		{ Index16(1, 5, 6) },
		{ Index16(1, 6, 2) },

		// Bottom
		{ Index16(3, 2, 6) },
		{ Index16(3, 6, 7) },

		// Left
		{ Index16(4, 0, 3) },
		{ Index16(4, 3, 7) },

		// Top
		{ Index16(4, 5, 1) },
		{ Index16(4, 1, 0) },

		// Back
		{ Index16(5, 4, 7) },
		{ Index16(5, 7, 6) }
	};


	D3D11_BUFFER_DESC tBufferDesc;
	ZeroMemory(&tBufferDesc, sizeof(D3D11_BUFFER_DESC));
	tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	tBufferDesc.ByteWidth = sizeof(WORD) * m_nIdxNum;
	tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	tBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA tData;
	ZeroMemory(&tData, sizeof(D3D11_SUBRESOURCE_DATA));
	tData.pSysMem = pIdx;
	FAILED_CHECK_RETURN(
		m_pDevice->GetDevice()->CreateBuffer(&tBufferDesc, &tData, &m_pIdxBuffer), );
}

void CSkyBoxBuffer::Init_SkyBoxShader()
{
	m_pSkyBoxShader = dynamic_cast<CSkyBoxShader*>(CResourceMgr::GetInstance()->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC, CResourceMgr::RESOURCE_TYPE_SHADER, L"Shader_SkyBox"));
	NULL_CHECK(m_pSkyBoxShader);
}

void CSkyBoxBuffer::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC tRasterizerDest;
	ZeroMemory(&tRasterizerDest, sizeof(D3D11_RASTERIZER_DESC));
	tRasterizerDest.CullMode = D3D11_CULL_FRONT;
	tRasterizerDest.FillMode = D3D11_FILL_SOLID;
	tRasterizerDest.DepthClipEnable = true;
	m_pDevice->GetDevice()->CreateRasterizerState(&tRasterizerDest, &m_pRasterizerState);
}

void CSkyBoxBuffer::Update()
{

}

void CSkyBoxBuffer::Render()
{
	m_pSkyBoxShader->Render();
	CBuffer::Render();
}

void CSkyBoxBuffer::Release()
{
	if ((*m_pRefCnt) == 0)
	{
		::Safe_Release(m_pVtxBuffer);
		::Safe_Release(m_pIdxBuffer);
		::Safe_Delete(m_pSkyBoxShader);
		CResource::Release_RefCnt();
	}
	else --(*m_pRefCnt);
}
