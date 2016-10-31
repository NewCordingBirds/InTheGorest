/*!
* \file RoomInfo.h
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

#ifndef RoomInfo_h__
#define RoomInfo_h__

#include "Component.h"
#include "Enum.h"

class CRoomInfo : public CComponent
{
public :
	const static WORD	MAX_USER_NUM_IN_ROOM	= 8;

public :
	static enum RoomState
	{
		ROOM_STATE_NONE,

		ROOM_STATE_IDLE,
		ROOM_STATE_COUNTING,
		ROOM_STATE_ALLSET,
		ROOM_STATE_PLAYING
	};

public:
	DWORD			m_dwRoomNum;
	wstring			m_wstrRoomName;

	RoomState		m_eRoomState;
	MapType			m_eSelectMapType;

	WORD			m_wMaxUserNum;
	WORD			m_wCurUserNum;


public:
	static CComponent*		Create();
public :
	virtual void			Update();
private:
	virtual void			Release();

public:
	explicit CRoomInfo();
	virtual ~CRoomInfo();
};


#endif // RoomInfo_h__
