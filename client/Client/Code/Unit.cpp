#include "stdafx.h"
#include "Unit.h"

#include "Component.h"


CUnit::CUnit(CDevice* _pDevice)
: CObj(_pDevice)
, m_pObjCol(NULL)

, m_pTexture(NULL)
, m_pDynamicModel(NULL)
{
	AddComponent();
}

CUnit::~CUnit()
{
}

void CUnit::AddComponent()
{
	// ObjCol ---------------
	m_pObjCol = dynamic_cast<CObjCol*>(CObjCol::Create(m_pDevice));
	NULL_CHECK(m_pObjCol);
	m_mapComponent.insert(make_pair(L"ObjCol", m_pObjCol));
}

