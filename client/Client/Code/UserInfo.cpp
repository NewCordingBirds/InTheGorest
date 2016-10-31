#include "stdafx.h"
#include "UserInfo.h"

CUserInfo::CUserInfo()
: m_wstrIP(L"")
, m_iID(-1)

, m_dwRoomNum(0)
, m_bReady(false)
{

}

CUserInfo::~CUserInfo()
{
	Release();
}

CComponent* CUserInfo::Create()
{
	return new CUserInfo;
}

void CUserInfo::Update()
{

}

void CUserInfo::Release()
{
}