/*!
 * \file Status.h
 * \date 2016/03/28 6:16
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

#ifndef Status_h__
#define Status_h__

#include "InMapUI.h"


class CStatus : public CInMapUI
{
public:
	static const float	SIZE_HP_X;
	static const float	SIZE_RECOSTER_X;


private:
	Sprite		m_tSprite_Frame;
	Sprite		m_tSprite_Hp;
	Sprite		m_tSprite_Recoster;

	CFont*		m_pFont_CurHp;
	CFont*		m_pFont_TotalHp;

	CFont*		m_pFont_CurRecoster;
	CFont*		m_pFont_TotalRecoster;


public:
	static CObj*	Create(CDevice*);

public:
	virtual HRESULT	Init();
	virtual int		Update();
	virtual void	Render();
	virtual void	Release();


protected:
	explicit CStatus(CDevice*);
public:
	virtual ~CStatus();
};

#endif // Status_h__
