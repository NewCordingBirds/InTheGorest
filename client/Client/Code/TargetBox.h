/*!
 * \file TargetBox.h
 * \date 2016/03/26 22:52
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

#ifndef TargetBox_h__
#define TargetBox_h__

#include "InMapUI.h"


class CServer;
class CUIObserver;
class CTargetBox : public CInMapUI
{
public:
	static const float	SIZE_FRAME_X;
	static const float	SIZE_FRAME_Y;
	static const float	SIZE_FRAME_X_HALF;
	static const float	SIZE_FRAME_Y_HALF;

	static const D3DXVECTOR2	CENTER_POS;

private:
	static const float	SIZE_CICLE_BASIC_HALF;
	static const float	SIZE_CICLE_TARGETING_HALF;

	static const float	POS_Y;
	static const float	ROTATE_SPEED_CIRCLE;

public:
	static enum State	{ STATE_BASIC, STATE_TARGETING, STATE_END };


private :
	State		m_eState;

	Sprite		m_tSprite_Frame;
	Sprite		m_tSprite_Circle[STATE_END];
	Sprite		m_tSprite_Point[STATE_END];

private :
	static const float	DISTANCE_INDUCTION;
	static const float	SPEED_MOVE_TO_TARGET;

	D3DXVECTOR2		m_vTargetPos;
	D3DXVECTOR3		m_vDirToTarget;
	float			m_fCircleAngle;

	CServer*		m_pServerMgr;
	CInfoSubject*	m_pInfoSubject;


public:
	static CObj*	Create(CDevice*);

public:
	virtual HRESULT	Init();
	virtual int		Update();
	virtual void	Render();
	virtual void	Release();


private :
	void		Update_SizeByZoom();

	void		CheckEnemy();
	bool		CheckInTargetBox(const D3DXVECTOR3&);

	void		MoveToTarget();


protected:
	explicit CTargetBox(CDevice*);
public:
	virtual ~CTargetBox();
};


#endif // TargetBox_h__
