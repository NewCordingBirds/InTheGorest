/*!
* \file EnemyObserver.h
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

#ifndef EnemyObserver_h__
#define EnemyObserver_h__

#include "Observer.h"
#include "Protocol.h"


class CEnemyObserver : public CObserver
{
private:
	//SC_InitPlayer	tInitPlayer[ROOM_MAXUSER];
	//SC_PlayerPos	tPlayerPos[ROOM_MAXUSER];

public:
	static CEnemyObserver* Create();
	virtual void Update(int _iMessage, UINT _nIdx = 0);

	//const SC_InitPlayer	GetInitPlayer(WORD _wIdx)	{ return tInitPlayer[_wIdx]; }
	//const SC_PlayerPos	GetPlayerPos(WORD _wIdx)	{ return tPlayerPos[_wIdx]; }

private:
	explicit CEnemyObserver();
public:
	~CEnemyObserver();
};

#endif // EnemyObserver_h__
