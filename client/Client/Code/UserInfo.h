/*!
 * \file UserInfo.h
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

#ifndef UserInfo_h__
#define UserInfo_h__

#include "Component.h"

class CUserInfo : public CComponent
{
public :
	wstring		m_wstrIP;
	int			m_iID;

	DWORD			m_dwRoomNum;
	bool			m_bReady;

public:
	static CComponent*		Create();
public :
	virtual void			Update();
private:
	virtual void			Release();

public:
	explicit CUserInfo();
	virtual ~CUserInfo();
};


#endif // UserInfo_h__
