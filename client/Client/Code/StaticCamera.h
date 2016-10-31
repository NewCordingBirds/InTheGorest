/*!
 * \file StaticCamera.h
 * \date 2016/02/19 3:53
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

#ifndef StaticCamera_h__
#define StaticCamera_h__

#include "Camera.h"


class CKeyMgr;
class CTimer;
class CPhysics;
class CStaticCamera : public CCamera
{
private:
	static enum WheelEvent
	{
		WHEEL_EVENT_NONE,

		WHEEL_EVENT_UP,
		WHEEL_EVENT_DOWN
	};

public :
	const static float	MAX_ZOOM_VALUE;
	const static float	MIN_ZOOM_VALUE;

private:
	CPhysics*		m_pTargetPhysics;


private :
	WheelEvent			m_eWheelEvent;
	float				m_fTargetZoom;
	float				m_fZoom;

	CKeyMgr*		m_pKeyMgr;
	CTimer*			m_pTimer;


public:
	static CObj*	Create(CDevice*);


public :
	void SetTargetPhysics(CPhysics* _pTargetPhysics)	{ m_pTargetPhysics = _pTargetPhysics; }


public:
	virtual HRESULT	Init();
	virtual int		Update();
	virtual void	Render();
	virtual void	Release();


private :
	void	CheckKey();
	void	MoveToTarget();
	void	AnimatingWheel();


private:
	explicit CStaticCamera(CDevice*);
public:
	virtual ~CStaticCamera();
};

#endif // StaticCamera_h__
