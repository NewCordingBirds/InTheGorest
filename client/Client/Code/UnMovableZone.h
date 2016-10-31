/*!
 * \file UnMovableZone.h
 * \date 2016/03/13 1:47
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

#ifndef UnMovableZone_h__
#define UnMovableZone_h__

#include "Trigger.h"

class CUnMovableZone : public CTrigger
{
public:
	static CObj*	Create(CDevice* _pDevice);

public:
	void	SetWorldMatrix(const D3DXVECTOR3& _vPos = D3DXVECTOR3(0.f, 0.f, 0.f),
		const D3DXVECTOR3& _vRadian = D3DXVECTOR3(0.f, 0.f, 0.f));

public:
	virtual HRESULT	Init();
	virtual int		Update();
	virtual void	Render();
	virtual void	Release();


public :
	void			SendDataToServer();


protected:
	explicit CUnMovableZone(CDevice* _pDevice);
public:
	virtual ~CUnMovableZone();
};

#endif // UnMovableZone_h__
