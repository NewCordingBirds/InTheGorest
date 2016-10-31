#include "stdafx.h"
#include "ObjCol.h"

#include "ResourceMgr.h"
#include "CubeBuffer.h"

#include "Physics.h"


CObjCol::CObjCol(CDevice* _pDevice)
: CCollision(_pDevice)
, m_eObjColType(OBJCOL_TYPE_DYNAMIC)

, m_pBuffer(NULL)

, m_pOwnerPhysics(NULL)

, m_vPlusPos(0.f, 0.f, 0.f)
, m_vScale(1.f, 1.f, 1.f)

, m_fRadius(2.f)
{
	D3DXMatrixIdentity(&m_matWorld);
}

CObjCol::~CObjCol()
{
	Release();
}

void CObjCol::SetPlusPos(const D3DXVECTOR3& _vPlusPos)
{
	m_vPlusPos = _vPlusPos;
	Update_WorldMatrix();
}

void CObjCol::SetScale(const D3DXVECTOR3& _vScale)
{
	m_vScale = _vScale;
	m_fRadius = MakeDiagonalLine(m_vScale);

	Update_WorldMatrix();
}

void CObjCol::Update_WorldMatrix()
{
	if (m_pOwnerPhysics == NULL)
		return;

	D3DXMATRIX	matScale, matRotate, matTrans;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationYawPitchRoll(&matRotate,
		m_pOwnerPhysics->m_vRadian.y, m_pOwnerPhysics->m_vRadian.x, m_pOwnerPhysics->m_vRadian.z);
	D3DXMatrixTranslation(
		&matTrans,
		m_pOwnerPhysics->m_vPos.x + m_vPlusPos.x,
		m_pOwnerPhysics->m_vPos.y + m_vPlusPos.y,
		m_pOwnerPhysics->m_vPos.z + m_vPlusPos.z);

	m_matWorld = matScale * matRotate * matTrans;
}

CComponent* CObjCol::Create(CDevice* _pDevice)
{
	CComponent* pComponent = new CObjCol(_pDevice);
	if ( FAILED((dynamic_cast<CObjCol*>(pComponent)->Init())) )
		::Safe_Delete(pComponent);

	return pComponent;
}

HRESULT CObjCol::Init()
{
	m_pBuffer = dynamic_cast<CBuffer*>(CResourceMgr::GetInstance()->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_DYNAMIC, CResourceMgr::RESOURCE_TYPE_BUFFER, L"Buffer_Cube"));
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);


	D3D11_BUFFER_DESC tBuffer;
	ZeroMemory(&tBuffer, sizeof(D3D11_BUFFER_DESC));
	tBuffer.Usage = D3D11_USAGE_DYNAMIC;
	tBuffer.ByteWidth = sizeof(D3DXMATRIX);
	tBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	FAILED_CHECK_RETURN(
		m_pDevice->GetDevice()->CreateBuffer(&tBuffer, NULL, &m_pWorldBuffer), S_FALSE);

	return S_OK;
}

void CObjCol::Update()
{
	if (m_eObjColType == OBJCOL_TYPE_DYNAMIC)
	{
		D3DXMATRIX	matScale, matRotate, matTrans;
		D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
		D3DXMatrixRotationYawPitchRoll(&matRotate,
			m_pOwnerPhysics->m_vRadian.y, m_pOwnerPhysics->m_vRadian.x, m_pOwnerPhysics->m_vRadian.z);
		D3DXMatrixTranslation(
			&matTrans,
			m_pOwnerPhysics->m_vPos.x + m_vPlusPos.x,
			m_pOwnerPhysics->m_vPos.y + m_vPlusPos.y,
			m_pOwnerPhysics->m_vPos.z + m_vPlusPos.z);

		m_matWorld = matScale * matRotate * matTrans;
	}

	m_pBuffer->Update();
}

void CObjCol::Render()
{
	if (m_pOwnerPhysics == NULL)
		return;

	

	D3D11_MAPPED_SUBRESOURCE tSubreResource;
	ID3D11DeviceContext* pDeviceContext = m_pDevice->GetDeviceContext();
	pDeviceContext->Map(m_pWorldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSubreResource);

	Buffer_World* pMatWorld = (Buffer_World*)(tSubreResource.pData);
	D3DXMatrixTranspose(&pMatWorld->m_matWorld, &m_matWorld);

	pDeviceContext->Unmap(m_pWorldBuffer, 0);

	pDeviceContext->VSSetConstantBuffers(VS_SLOT_WORLD_MATRIX, 1, &m_pWorldBuffer);
	m_pBuffer->Render();
}

void CObjCol::Release()
{
	::Safe_Delete(m_pBuffer);
	::Safe_Release(m_pWorldBuffer);
}
