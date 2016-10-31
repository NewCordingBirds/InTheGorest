/*!
 * \file Floor.h
 * \date 2016/02/11 20:37
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

#ifndef Floor_h__
#define Floor_h__

#include "BackGround.h"
#include "PlanBuffer.h"


class CTexture;
class CFloor : public CBackGround
{
private :
	CTexture*			m_pTexture;
	

public:
	static CObj*	Create(CDevice* _pDevice, const wstring& _wstrTextureKey);

private:
	void	AddComponent(const wstring& _wstrTextureKey);

public:
	virtual HRESULT	Init();
	virtual int		Update();
	virtual void	Render();
	virtual void	Release();


public :
	void	SetTextureUV(const D3DXVECTOR2& _vUV)	{ ((CPlanBuffer*)m_pBuffer)->SetUV(_vUV); }


protected:
	explicit CFloor(CDevice* _pDevice);
public:
	virtual ~CFloor();
};

#endif // Floor_h__
