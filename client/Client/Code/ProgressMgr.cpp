#include "stdafx.h"
#include "ProgressMgr.h"

IMPLEMENT_SINGLETON(CProgressMgr)

CProgressMgr::CProgressMgr()
: m_iProgressRareZoneNum(0)
{
	
}

CProgressMgr::~CProgressMgr()
{
	ClearProrate();
}

void CProgressMgr::Init()
{
	D3DXVECTOR3 tempvec;
	float		tempfloat = 0;
	///---
	if (m_vecProgress.size() == 0)
		return;


	for (unsigned int i = 0; i < m_vecProgress.size() - 1; ++i)
	{
		tempvec = (*m_vecProgress[i + 1]->GetPos())
			- (*m_vecProgress[i]->GetPos());
		m_vProgline.push_back(tempvec);

	}
	tempvec = *(m_vecProgress[0]->GetPos())
		- *(m_vecProgress[m_vecProgress.size() - 1]->GetPos());
	m_vProgline.push_back(tempvec);

	// 2 바퀴니까 더 넣어준다..
	for (unsigned int i = 0; i < m_vecProgress.size(); ++i)
		m_vProgline.push_back(m_vProgline[i]);


	//m_vProglength.push_back(tempfloat);
	for (unsigned int i = 0; i < m_vProgline.size(); ++i)
	{
		tempfloat += D3DXVec3Length(&m_vProgline[i]);
		m_vProglength.push_back(tempfloat);
	}

	m_fFullLength = m_vProglength[m_vProglength.size() - 1];
}

void CProgressMgr::Update()
{
	m_pPlayer->m_fProgressRate = ProgCalculate(m_pPlayer);
}

float CProgressMgr::ProgCalculate(CPlayer* _player)
{
	D3DXVECTOR3 playervec;
	float	tempf;
	

	if (_player->m_iprogressnum >= 0)
	{
		unsigned int iIdx = 0;

		if (m_vecProgress.size() <= _player->m_iprogressnum)
			iIdx = _player->m_iprogressnum - m_vecProgress.size();
		else
			iIdx = _player->m_iprogressnum;

		playervec = CServer::GetInstance()->GetPlayerPos().move
			- *(m_vecProgress[iIdx]->GetPos());
		tempf = D3DXVec3Dot(&playervec, &m_vProgline[_player->m_iprogressnum])
			/ D3DXVec3Length(&(m_vProgline[_player->m_iprogressnum]));

		tempf += m_vProglength[_player->m_iprogressnum - 1];
	}
	
	return tempf;
}