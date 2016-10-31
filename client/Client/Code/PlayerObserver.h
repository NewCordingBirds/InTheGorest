/*!
 * \file PlayerObserver.h
 * \date 2016/03/24 20:14
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

#ifndef PlayerObserver_h__
#define PlayerObserver_h__

#include "Observer.h"
#include "Protocol.h"


class CPlayerObserver : public CObserver
{
private:
	//SC_InitPlayer	tInitPlayer;
	//SC_PlayerPos	tPlayerPos;
	CS_Key			m_tKey;
	CS_Rotate		m_tRotate;
	CS_Shoot		m_tShoot;

	WORD			m_nBulletNum;

	WORD			m_nHp;
	WORD			m_nRecoterNum;

	float			m_fCurMapLength;
	

public:
	static CPlayerObserver* Create();
	virtual void Update(int _iMessage, UINT _nIdx = 0);

	const CS_Key		GetKey()		{ return m_tKey;		}
	const CS_Rotate		GetRotate()		{ return m_tRotate;		}
	const CS_Shoot		GetShoot()		{ return m_tShoot;		}

	const WORD			GetHp()			{ return m_nHp; }
	const WORD			GetRecoster()	{ return m_nRecoterNum; }
	const WORD			GetBulletNum()	{ return m_nBulletNum; }

	const float			GetCurMapLength()	{ return m_fCurMapLength; }

private:
	explicit CPlayerObserver();
public:
	~CPlayerObserver();
};

#endif // PlayerObserver_h__
