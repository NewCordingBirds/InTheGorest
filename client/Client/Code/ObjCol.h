/*!
 * \file ObjCol.h
 * \date 2016/01/24 9:37
 *
 * \author ±è¸íÁö
 * Contact: bluevill04@gmail.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#pragma once

#ifndef ObjCol_h__
#define ObjCol_h__

#include "Collision.h"
#include "Physics.h"

class CBuffer;
class CObjCol : public CCollision
{
public :
	static enum ObjColType { OBJCOL_TYPE_STATIC, OBJCOL_TYPE_DYNAMIC };

public:
	ObjColType		m_eObjColType;

private :
	CBuffer*		m_pBuffer;
	ID3D11Buffer*	m_pWorldBuffer;

public:
	CPhysics*			m_pOwnerPhysics;

	D3DXVECTOR3			m_vPlusPos;
	D3DXVECTOR3			m_vScale;

	float				m_fRadius;
	D3DXMATRIX			m_matWorld;


public:
	static CComponent*		Create(CDevice*);
public:
	HRESULT					Init();
	virtual void			Update();
	void					Render();
private:
	virtual void			Release();


public:
	const D3DXVECTOR3&	GetPos()			const	{ return (m_pOwnerPhysics->m_vPos) + m_vPlusPos; }
	const D3DXVECTOR3&	GetScale()			const	{ return m_vScale; }
	D3DXVECTOR3&		GetDir()			const	{ return m_pOwnerPhysics->m_vDir; }
	const float&		GetRadius()			const	{ return m_fRadius; }
	const D3DXMATRIX&	GetWorldMatrix()	const	{ return m_matWorld; }

	void SetPlusPos(const D3DXVECTOR3& _vPlusPos);
	void SetScale(const D3DXVECTOR3& _vScale);

public :
	void Update_WorldMatrix();


private :
	CObjCol(CDevice*);
public :
	~CObjCol();
};

#endif // ObjCol_h__
