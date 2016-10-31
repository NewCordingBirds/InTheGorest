#include "stdafx.h"
#include "RoomInfo.h"

CRoomInfo::CRoomInfo()
: m_dwRoomNum(0)
, m_wstrRoomName(L"")

, m_eRoomState(ROOM_STATE_NONE)
, m_eSelectMapType(MAP_TYPE_END)

, m_wMaxUserNum(0)
, m_wCurUserNum(0)
{

}

CRoomInfo::~CRoomInfo()
{
	Release();
}


CComponent* CRoomInfo::Create()
{
	return new CRoomInfo;
}

void CRoomInfo::Update()
{

}

void CRoomInfo::Release()
{
}