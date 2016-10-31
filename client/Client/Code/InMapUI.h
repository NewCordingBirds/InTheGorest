/*!
 * \file InMapUI.h
 * \date 2016/03/27 21:45
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

#ifndef InMapUI_h__
#define InMapUI_h__

#include "UI.h"
#include "PlayerObserver.h"
#include "ServerMgr.h"
#include "InMapMgr.h"

class CInMapUI abstract : public CUI
{
protected:
	CServer*			m_pServerMgr;
	CInMapMgr*			m_pInMapMgr;
	CPlayerObserver*	m_pPlayerObserver;


private:
	void	Init_PlayerObserver();
	void	Release_PlayerObserver();

public:
	virtual HRESULT	Init()			PURE;
	virtual int		Update()		PURE;
	virtual void	Render()		PURE;
	virtual void	Release()		PURE;


protected:
	static bool	m_bGameEnd;
	void	CheckGameEnd();

protected:
	CInMapUI(CDevice*);
public:
	virtual ~CInMapUI()		PURE;
};

#endif // InMapUI_h__
