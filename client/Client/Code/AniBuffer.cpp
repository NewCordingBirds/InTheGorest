#include "stdafx.h"
#include "AniBuffer.h"

CDevice* CAniBuffer::m_pDevice = CDevice::GetInstance();


CAniBuffer::CAniBuffer()
: m_nOffset(0)
, m_nStride(sizeof(VertexAni))
, m_nStartIndex(0)
, m_nBaseVertex(0)
, m_nVertices(0)
{
	this->m_pVertexBuffer = nullptr;
}

CAniBuffer::~CAniBuffer()
{
	for (auto iter : this->m_vecChildBuffer)
		delete iter;

	if (this->m_pVertexBuffer != nullptr)
	{
		this->m_pVertexBuffer->Release();
		this->m_pVertexBuffer = nullptr;
	}
}

VertexAni* CAniBuffer::GetVertex(unsigned int _nIndex)
{
	if (this->m_nVertices <= _nIndex)
	{
		MSG_BOX(L"WARNNING! Out Of Range");
		return nullptr;
	}
	else
		return &m_pVertex[_nIndex];
}

void CAniBuffer::SetVertexSize(unsigned int _iSize)
{
	this->m_nVertices = _iSize;
	this->m_pVertex = new VertexAni[_iSize];
}

CAniBuffer* CAniBuffer::GetChild(unsigned int _nindex)
{
	return m_vecChildBuffer[_nindex];
}

void CAniBuffer::AddChild(CAniBuffer* _pChildBuffer)
{
	this->m_vecChildBuffer.push_back(_pChildBuffer);
}

void CAniBuffer::CreateBuffer()
{
	if (this->m_nVertices != 0)
	{
		unsigned int ByteWidth
			= ((unsigned int)((this->m_nStride * this->m_nVertices) / 16) * 16) + 16;

		D3D11_BUFFER_DESC BD;
		ZeroMemory(&BD, sizeof(D3D11_BUFFER_DESC));
		BD.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		BD.ByteWidth = ByteWidth;
		BD.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		BD.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA SD;
		ZeroMemory(&SD, sizeof(D3D11_SUBRESOURCE_DATA));
		SD.pSysMem = this->m_pVertex;

		if (FAILED(m_pDevice->GetDevice()->CreateBuffer(&BD, &SD, &this->m_pVertexBuffer)))
		{
			MSG_BOX(L"Vertex Buffer Create Failed!");
			getchar();
			exit(0);
		}
	}
}

void CAniBuffer::SetBuffer()
{
	if (this->m_pVertexBuffer)
	{
		m_pDevice->GetDeviceContext()->IASetVertexBuffers(
			0,
			1,
			&this->m_pVertexBuffer,
			&this->m_nStride,
			&this->m_nOffset);
	}
}

void CAniBuffer::Render()
{
	if (this->m_nVertices != 0)
	{
		this->SetBuffer();
		m_pDevice->GetDeviceContext()->Draw(this->m_nVertices, this->m_nStartIndex);
	}

	for (auto iter : this->m_vecChildBuffer)
		iter->Render();
}

void CAniBuffer::SetFbxBoneIndex(map<std::string, unsigned int>* _pIndexByName,
								FbxNode* _pNode)
{
	(*_pIndexByName)[_pNode->GetName()] = _pIndexByName->size();

	for (int i = 0; i < _pNode->GetChildCount(); ++i)
		this->SetFbxBoneIndex(_pIndexByName, _pNode->GetChild(i));
}
