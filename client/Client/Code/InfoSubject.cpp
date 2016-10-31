#include "stdafx.h"
#include "InfoSubject.h"

IMPLEMENT_SINGLETON(CInfoSubject)


CInfoSubject::CInfoSubject()
{
}

CInfoSubject::~CInfoSubject()
{
	Release();
}

vector<void*>* CInfoSubject::GetDatalist(int _imessage)
{
	map<int, vector<void*>>::iterator	iter = m_mapData.find(_imessage);

	if (iter == m_mapData.end())
		return NULL;

	return &iter->second;
}

void CInfoSubject::AddData(int _iMessage, void* _pData)
{
	if (_pData)
	{
		map<int, vector<void*>>::iterator	iter = m_mapData.find(_iMessage);
		if (iter == m_mapData.end())
		{
			m_mapData[_iMessage] = vector<void*>();
		}
		m_mapData[_iMessage].push_back(_pData);
	}
}

void CInfoSubject::RemoveData(int _iMessage, void* _pData)
{
	map<int, vector<void*>>::iterator	iterMap = m_mapData.find(_iMessage);
	if (iterMap != m_mapData.end())
	{
		vector<void*>::iterator iterList = iterMap->second.begin();
		vector<void*>::iterator iterList_end = iterMap->second.end();

		for (; iterList != iterList_end; ++iterList)
		{
			if ((*iterList) == _pData)
			{
				iterMap->second.erase(iterList);
				return;
			}
		}
	}
}

void CInfoSubject::Release(void)
{
	map<int, vector<void*>>::iterator iter = m_mapData.begin();

	for (; iter != m_mapData.end(); ++iter)
		iter->second.clear();
	m_mapData.clear();
}


