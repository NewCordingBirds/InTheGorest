/*!
* \file Player.h
* \date 2016/01/24 11:29
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

#ifndef Rider_h__
#define Rider_h__

#include "Unit.h"

#include "UserInfo.h"
#include "PlayingInfo.h"

#include "Protocol.h"
#include "ServerMgr.h"


class CRider abstract : public CUnit
{
protected :
	CServer*				m_pServerMgr;

	CUserInfo*				m_pUserInfo;
	CPlayingInfo*			m_pPlayingInfo;

private:
	void	AddComponent();

// public :
// 	virtual void Render();

protected :
	void PlayAnimation();
public :
	virtual void SetAnimation(CPlayingInfo::AniState _eAniState);
	virtual void SetAnimation(BYTE _nAniState);

public :
	int		GetUserID()			{ return m_pUserInfo->m_iID; }


public :
	void	SetInitPlayer(const SC_InitPlayer& _tInitPlayer);
	void	SetPlayingState(const CPlayingInfo::PlayingState& _ePlayingState)		{ m_pPlayingInfo->m_ePlayingState = _ePlayingState; }


protected:
	explicit CRider(CDevice* _pDevice);
public:
	virtual ~CRider()	PURE;
};

#endif // Rider_h__
