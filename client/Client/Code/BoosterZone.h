/*!
 * \file BoosterZone.h
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

#ifndef BoosterZone_h__
#define BoosterZone_h__

#include "Trigger.h"

class CBoosterZone : public CTrigger
{
private :
	CTexture*			m_pTexture;
	CBuffer*			m_pBuffer;


public:
	static CObj*	Create(CDevice* _pDevice);

private:
	void	AddComponent();

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
	explicit CBoosterZone(CDevice* _pDevice);
public:
	virtual ~CBoosterZone();
};

#endif // BoosterZone_h__
