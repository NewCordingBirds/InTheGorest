/*!
 * \file Subject.h
 * \date 2016/01/11 5:41
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

#ifndef Subject_h__
#define Subject_h__

class CObserver;
class CSubject
{
protected:
	list<CObserver*>		m_ListObserver;


public :
	void Subscribe(CObserver* _pObserver);
	void Unsubscribe(CObserver* _pObserver);
	void Notify(int _iMessage, UINT _nIdx = 0);


private :
	void Release();


protected:
	CSubject();
	virtual ~CSubject();
};

#endif // Subject_h__
