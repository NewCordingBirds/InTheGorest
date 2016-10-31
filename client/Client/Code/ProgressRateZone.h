/*!
 * \file ProgressRateZone.h
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

#ifndef ProgressRateZone_h__
#define ProgressRateZone_h__

#include "Trigger.h"
#include "ProgressMgr.h"
#include "Obj.h"

class CProgressRateZone : public CTrigger
{
private :
	int		m_iID;

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
	int		GetID()		{ return m_iID;  }


protected:
	explicit CProgressRateZone(CDevice* _pDevice);
public:
	virtual ~CProgressRateZone();
};

#endif // ProgressRateZone_h__
