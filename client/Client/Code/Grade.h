/*!
 * \file Grade.h
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

#ifndef Grade_h__
#define Grade_h__

#include "InMapUI.h"


class CFont;
class CTimer;
class CGrade : public CInMapUI
{
private:
	static const float	POS_X, POS_Y;
	static const WORD	MAX_RIDER_NUM	= 8;
	

private:
	Sprite		m_tSprite[MAX_RIDER_NUM];
	CFont*		m_pFont_Time;

	CTimer*		m_pTimer;
	float		m_fTimer;
	
	UINT		m_nCurRiderNum;
	CFont*		m_pFont_GradeNum[MAX_RIDER_NUM];
	CFont*		m_pFont_ID[MAX_RIDER_NUM];


public:
	static CObj*	Create(CDevice*);

public:
	virtual HRESULT	Init();
	virtual int		Update();
	virtual void	Render();
	virtual void	Release();


public :
	void	AddPlayerGradeInfo(UINT _nID);


private :
	void	Update_Time();


protected:
	explicit CGrade(CDevice*);
public:
	virtual ~CGrade();
};

#endif // Grade_h__
