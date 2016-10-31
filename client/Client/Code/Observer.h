/*!
 * \file Observer.h
 * \date 2016/01/11 5:39
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

#ifndef Observer_h__
#define Observer_h__

#include "InfoSubject.h"


class CObserver
{
protected:
	CInfoSubject*	m_pInfoSubject;

public :
	virtual void Update(int _iMessage, UINT _nIdx = 0)	PURE;

protected:
	CObserver();
public :
	virtual ~CObserver();
};

#endif // Observer_h__
