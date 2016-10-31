/*!
 * \file Ammo.h
 * \date 2016/03/28 6:23
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

#ifndef Ammo_h__
#define Ammo_h__

#include "InMapUI.h"


class CFont;
class CAmmo : public CInMapUI
{
public:
	static const float	SIZE_FRAME_X_HALF;
	static const float	SIZE_FRAME_Y_HALF;

private:
	static const float	POS_Y;


private:
	Sprite		m_tSprite_Frame;

	CFont*		m_pFont_CurBullet;
	CFont*		m_pFont_TotalBullet;


public:
	static CObj*	Create(CDevice*);

public:
	virtual HRESULT	Init();
	virtual int		Update();
	virtual void	Render();
	virtual void	Release();


protected:
	explicit CAmmo(CDevice*);
public:
	virtual ~CAmmo();
};

#endif // Ammo_h__
