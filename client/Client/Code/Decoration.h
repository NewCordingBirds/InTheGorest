/*!
 * \file Decoration.h
 * \date 2016/02/12 22:12
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

#ifndef Decoration_h__
#define Decoration_h__

#include "BackGround.h"

#include "ResourceMgr.h"
#include "Texture.h"
#include "StaticModel.h"


class CDecoration : public CBackGround
{
protected :
	CTexture*			m_pTexture;


public:
	static CObj*	Create(CDevice* _pDevice,
							const wstring& _wstrTextureKey,
							const wstring& _wstrModelKey);

protected :
	virtual void	AddComponent(const wstring& _wstrTextureKey,
								const wstring& _wstrModelKey);

public:
	virtual HRESULT	Init();
	virtual int		Update();
	virtual void	Render();
	virtual void	Release();


protected:
	explicit CDecoration(CDevice* _pDevice);
public:
	virtual ~CDecoration();
};

#endif // Decoration_h__
