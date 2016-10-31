/*!
 * \file ProgressRate.h
 * \date 2016/03/28 5:50
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

#ifndef ProgressRate_h__
#define ProgressRate_h__

#include "InMapUI.h"


class CFont;
class CProgressRate : public CInMapUI
{
public:
	static const float	SIZE_FRAME_Y_HALF;
	static const float	SIZE_RATE_Y;

private:
	static const float	POS_X;


private:
	Sprite		m_tSprite_Frame;
	Sprite		m_tSprite_Rate;

	CFont*		m_pFont_Lap;
	CFont*		m_pFont_CurLap;
	CFont*		m_pFont_TotalLap;
	CFont*		m_pFont_FPS;

	UINT		m_nCurLap;
	UINT		m_nTotalLap;

	float		m_fCurMapLenght;


public:
	static CObj*	Create(CDevice*);

public:
	virtual HRESULT	Init();
	virtual int		Update();
	virtual void	Render();
	virtual void	Release();


protected:
	explicit CProgressRate(CDevice*);
public:
	virtual ~CProgressRate();
};

#endif // ProgressRate_h__
