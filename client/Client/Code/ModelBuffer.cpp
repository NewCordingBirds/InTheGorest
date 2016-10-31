#include "stdafx.h"
#include "ModelBuffer.h"

#include "ResourceMgr.h"


CModelBuffer::CModelBuffer(CDevice* _pDevice)
: CBuffer(_pDevice)
, m_pShader(NULL)
{

}

CModelBuffer::~CModelBuffer()
{

}

void CModelBuffer::Render()
{
	m_pShader->Render();
	CBuffer::Render();
}

void CModelBuffer::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC tRasterizerDesc;
	ZeroMemory(&tRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	tRasterizerDesc.CullMode = D3D11_CULL_NONE;
	tRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	m_pDevice->GetDevice()->CreateRasterizerState(&tRasterizerDesc, &m_pRasterizerState);
}

void CModelBuffer::MakeVertexNormal(BYTE* _pVertices, WORD* _pIndices)
{
	if (!_pIndices)
		SetNormalVectorByBasic(_pVertices);
	else
		SetNormalVectorByAverage(_pVertices, _pIndices, (m_nIdxNum / 3), 3, false);
}

void CModelBuffer::SetNormalVectorByBasic(BYTE* _pVertices)
{
	D3DXVECTOR3		vNormal;
	VertexTexture*	pVertex = NULL;

	int nPrimitives = m_nVtxNum / 3;
	for (int i = 0; i < nPrimitives; ++i)
	{
		vNormal = GetTriAngleNormal(_pVertices, (i * 3 + 0), (i * 3 + 1), (i * 3 + 2));

		pVertex = (VertexTexture*)(_pVertices + ((i * 3 + 0) * m_nVtxStride));
		pVertex->vNormal = vNormal;
		pVertex = (VertexTexture*)(_pVertices + ((i * 3 + 1) * m_nVtxStride));
		pVertex->vNormal = vNormal;
		pVertex = (VertexTexture*)(_pVertices + ((i * 3 + 2) * m_nVtxStride));
		pVertex->vNormal = vNormal;
	}
}

void CModelBuffer::SetNormalVectorByAverage(BYTE* _pVertices, WORD* _pIndices, int _iPrimitives, int _iOffset, bool _bStrip)
{
	D3DXVECTOR3		vNormal(0.f, 0.f, 0.f);
	VertexTexture*	pVertex = NULL;
	USHORT nIndex_0, nIndex_1, nIndex_2;

	for (UINT i = 0; i < m_nVtxNum; ++i)
	{
		vNormal = D3DXVECTOR3(0.f, 0.f, 0.f);

		for (int j = 0; j < _iPrimitives; j++)
		{
			nIndex_0 = (_bStrip) ? (((j % 2) == 0) ? (j * _iOffset + 0) : (j * _iOffset + 1)) : (j * _iOffset + 0);
			if (_pIndices) nIndex_0 = _pIndices[nIndex_0];

			nIndex_1 = (_bStrip) ? (((j % 2) == 0) ? (j * _iOffset + 1) : (j * _iOffset + 0)) : (j * _iOffset + 1);
			if (_pIndices) nIndex_1 = _pIndices[nIndex_1];

			nIndex_2 = (_pIndices) ? _pIndices[j * _iOffset + 2] : (j * _iOffset + 2);

			if ((nIndex_0 == i) || (nIndex_1 == i) || (nIndex_2 == i))
				vNormal += GetTriAngleNormal(_pVertices, nIndex_0, nIndex_1, nIndex_2);
		}
		D3DXVec3Normalize(&vNormal, &vNormal);
		pVertex = (VertexTexture *)(_pVertices + (i * m_nVtxStride));
		pVertex->vNormal = vNormal;
	}
}

D3DXVECTOR3 CModelBuffer::GetTriAngleNormal(BYTE* _pVertices, USHORT _nIndex_0, USHORT _nIndex_1, USHORT _nIndex_2)
{
	D3DXVECTOR3 vNormal(0.f, 0.f, 0.f);
	D3DXVECTOR3 vP_0 = *((D3DXVECTOR3 *)(_pVertices + (m_nVtxStride * _nIndex_0)));
	D3DXVECTOR3 vP_1 = *((D3DXVECTOR3 *)(_pVertices + (m_nVtxStride * _nIndex_1)));
	D3DXVECTOR3 vP_2 = *((D3DXVECTOR3 *)(_pVertices + (m_nVtxStride * _nIndex_2)));

	D3DXVECTOR3 vEdge_0 = vP_1 - vP_0;
	D3DXVECTOR3 vEdge_1 = vP_2 - vP_0;

	D3DXVec3Cross(&vNormal, &vEdge_0, &vEdge_1);
	D3DXVec3Normalize(&vNormal, &vNormal);

	return vNormal;
}
