#include "stdafx.h"
#include "Observer.h"


CObserver::CObserver()
: m_pInfoSubject(CInfoSubject::GetInstance())
{
}

CObserver::~CObserver()
{
}