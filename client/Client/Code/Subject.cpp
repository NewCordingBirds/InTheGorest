#include "stdafx.h"
#include "Subject.h"

#include "Observer.h"

CSubject::CSubject(void)
{

}

CSubject::~CSubject(void)
{
	Release();
}

void CSubject::Subscribe(CObserver* _pObserver)
{
	NULL_CHECK(_pObserver);
	m_ListObserver.push_back(_pObserver);
}

void CSubject::Unsubscribe(CObserver* _pObserver)
{
	list<CObserver*>::iterator	iter = m_ListObserver.begin();
	list<CObserver*>::iterator	iter_end = m_ListObserver.end();

	for (; iter != iter_end; ++iter)
	{
		if ((*iter) == _pObserver)
		{
			m_ListObserver.erase(iter);
			return;
		}
	}
}

void CSubject::Notify(int _iMessage,
						UINT _nIdx /*= 0*/)
{
	list<CObserver*>::iterator	iter = m_ListObserver.begin();
	list<CObserver*>::iterator	iter_end = m_ListObserver.end();

	for (; iter != iter_end; ++iter)
		(*iter)->Update(_iMessage, _nIdx);
}

void CSubject::Release(void)
{
	m_ListObserver.clear();
}

