/*!
* \file PlayingInfo.h
* \date 2016/01/21 4:56
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

#ifndef PlayingInfo_h__
#define PlayingInfo_h__

#include "Component.h"
#include "Enum.h"

class CPlayingInfo : public CComponent
{
public :
	static enum PlayingState
	{
		PLAYING_STATE_NONE	= -1,

		PLAYING_STATE_STANDBY	= STANDBY,
		PLAYING_STATE_READY		= READY,
		PLAYING_STATE_PLAYING	= PLAYING,
		PLAYING_STATE_GOAL		= _END
	};

	/*
	[Protocol Game State]

	#define STANDBY					0
	#define READY					1
	#define PLAYING					2
	#define END						3
	*/

	static enum AniState
	{
		ANI_STATE_RUN,
		ANI_STATE_FALL,
		ANI_STATE_DEAD,
		ANI_STATE_DAMAGE,
		ANI_STATE_BOOSTER,
		ANI_STATE_BREAK,

		ANI_STATE_END
	};


public:
	PlayingState	m_ePlayingState;
	AniState		m_eAniState;

	WORD			m_wGradeNum;
	WORD			m_wLabNum;
	float			m_fProgressRate;

	int			m_iBulletNum;

	int			m_iHp;
	int			m_iRecoter;


public:
	static CComponent*		Create();
public :
	virtual void			Update();
private:
	virtual void			Release();


public :
	void	PlusHp(int _iValue);
	bool	PlusRecoster(int _iValue);

public:
	explicit CPlayingInfo();
	virtual ~CPlayingInfo();
};


#endif // PlayingInfo_h__
