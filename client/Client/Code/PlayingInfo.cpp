#include "stdafx.h"
#include "PlayingInfo.h"

CPlayingInfo::CPlayingInfo()
: m_ePlayingState(PLAYING_STATE_NONE)
, m_eAniState(ANI_STATE_END)

, m_wGradeNum(0)
, m_wLabNum(0)
, m_fProgressRate(0.f)

, m_iHp(MAX_HP)
, m_iBulletNum(MAX_BULLET_NUM)
, m_iRecoter(0)
{

}

CPlayingInfo::~CPlayingInfo()
{
	Release();
}

CComponent* CPlayingInfo::Create()
{
	return new CPlayingInfo;
}

void CPlayingInfo::Update()
{

}

void CPlayingInfo::Release()
{
}

void CPlayingInfo::PlusHp(int _iValue)
{
	m_iHp += _iValue;

	if (MAX_HP < m_iHp) m_iHp = MAX_HP;
	else if (0 > m_iHp) m_iHp = 0;
}

bool CPlayingInfo::PlusRecoster(int _iValue)
{
	m_iRecoter += _iValue;

	if (MAX_RECOSTER < m_iRecoter)
	{
		m_iRecoter = MAX_RECOSTER;
		return true;
	}
	else if (0 > m_iRecoter)
	{
		m_iRecoter -= _iValue;
		return false;
	}

	return true;
}
