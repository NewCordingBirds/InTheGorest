/*!
* \file UIObserver.h
* \date 2016/03/24 20:14
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

#ifndef UIObserver_h__
#define UIObserver_h__

#include "Observer.h"


class CUIObserver : public CObserver
{
private:
	int				m_iTargetingState;
	D3DXVECTOR3		m_vDirToTarget;


public:
	static CUIObserver* Create();
	virtual void Update(int _iMessage, UINT _nIdx = 0);

	const int			GetTargetingState()	{ return m_iTargetingState; }
	const D3DXVECTOR3	GetDirToTarget()	{ return m_vDirToTarget; }

private:
	explicit CUIObserver();
public:
	~CUIObserver();
};

#endif // UIObserver_h__
