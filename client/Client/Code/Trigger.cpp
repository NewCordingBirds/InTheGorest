#include "stdafx.h"
#include "Trigger.h"

#include "ResourceMgr.h"
#include "ObjCol.h"


CTrigger::CTrigger(CDevice* _pDevice)
: CObj(_pDevice)
, m_pObjCol(NULL)
{
	AddComponent();
}

CTrigger::~CTrigger()
{

}

void CTrigger::AddComponent()
{
	// ObjCol ---------------
	m_pObjCol = dynamic_cast<CObjCol*>(CObjCol::Create(m_pDevice));
	NULL_CHECK(m_pObjCol);
	m_mapComponent.insert(make_pair(L"ObjCol", m_pObjCol));
}
