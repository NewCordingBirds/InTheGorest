/*!
 * \file DirectionSign.h
 * \date 2016/05/06 16:32
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

#ifndef DirectionSign_h__
#define DirectionSign_h__

#include "Decoration.h"


class CDirectionSign : public CDecoration
{
public:
	static CObj*	Create(CDevice* _pDevice);

public:
	virtual HRESULT	Init();
	virtual int		Update();


public:
	virtual void	SetWorldMatrix(const D3DXVECTOR3& _vPos = D3DXVECTOR3(0.f, 0.f, 0.f),
									const D3DXVECTOR3& _vRadian = D3DXVECTOR3(0.f, 0.f, 0.f));


protected:
	explicit CDirectionSign(CDevice* _pDevice);
public:
	virtual ~CDirectionSign();
};


#endif // DirectionSign_h__
