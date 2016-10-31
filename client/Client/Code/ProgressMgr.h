#pragma once

#ifndef ProgressMgr_h__
#define ProgressMgr_h__

#include "stdafx.h"
#include "InMapMgr.h"
#include <conio.h>

class CProgressMgr
{
	DECLARE_SINGLETON(CProgressMgr)

public:

	vector<CObj*>	m_vecProgress;

	vector<D3DXVECTOR3>	m_vProgline;
	CPlayer		*m_pPlayer;
	vector<float>	m_vProglength;

	float		m_fFullLength;
	int			m_iProgressRareZoneNum;


	void		Init();
	void		Update();
	void		SetPlayer(CPlayer* _pPlayer)		{ m_pPlayer = _pPlayer; }

	void		PushBackProrate(CObj* _pProg)	
	{
		m_vecProgress.push_back(_pProg);
	}
	void		ClearProrate()						{ m_vecProgress.clear(); }

	float		ProgCalculate(CPlayer* _player);


private:
	CProgressMgr();
	~CProgressMgr();
};

#endif // InMapMgr_h__
