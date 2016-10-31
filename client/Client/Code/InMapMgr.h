/*!
* \file Player.h
* \date 2016/04/17 17:13
*
* \author 김명지
* Contact: bluevill04@gmail.com
*
* \brief
*
* TODO: long description
*
* \note
*/

#pragma once

#ifndef InMapMgr_h__
#define InMapMgr_h__

#include "Protocol.h"
#include "Player.h"


class CObjMgr;
class CRider;
class CEnemyObserver;
class CServer;

class CGrade;
class CNotify;
class CInMapMgr
{
	DECLARE_SINGLETON(CInMapMgr)

public :
	/*
	[Protocol Game State]

	#define STANDBY					0
	#define READY					1
	#define PLAYING					2
	#define END						3
	*/
	static enum InMapState
	{
		INMAP_STATE_LOADING		= STANDBY,
		INMAP_STATE_READY		= READY,
		INMAP_STATE_PLAYING		= PLAYING,
		INMAP_STATE_END			= _END,
		INMAP_STATE_RESULT
	};

	static enum InMapProcess
	{
		INMAP_PROCESS_IDLE,
		INMAP_PROCESS_WORK,
		INMAP_PROCESS_END
	};

private :
	InMapState		m_eInMapState;
	InMapProcess	m_eInMapProcess;

	WORD			m_nCurLap;

	SC_State		m_tState;
	SC_Item			m_tItem;

	CServer*		m_pServerMgr;
	CObjMgr*		m_pObjMgr;

	CRider*			m_pPlayer;
	CRider*			m_pEnemy[MAX_USER_NUM];

	vector<CObj*>	m_vecItem;


public :
	void		Init();
	void		Update();

	// (수정) SetInMapState 급해.. ㅎ
	void		SetInMapState(const InMapState& _eInMapState)
	{ m_eInMapState = _eInMapState; m_eInMapProcess = CInMapMgr::INMAP_PROCESS_IDLE; }
	InMapState	GetInMapState()									{ return m_eInMapState; }

	void		SetPlayer(CRider* _pPlayer)		{ m_pPlayer = _pPlayer; }

	void		PushBackItem(CObj* _pItem)		{ m_vecItem.push_back(_pItem); }
	void		ClearItem()						{ m_vecItem.clear(); }

	void		SetItemOnOff(unsigned int _nIdx, bool _bOn);
	

private :
	void		CheckInMapState();
	void		CheckCountDown();

	void		CheckItemState();
	void		CheckEnemyAniState();
	void		CheckBoosterEnd();
	void		CheckCreateEnemyBullet();
	void		SetRiderPlayingState(const CPlayingInfo::PlayingState&);


public :
	void		SetCurLap(WORD	_nLap);


public:
	void SetPlayerToReady();
	void CheckEnemyInitAndCreate();
private:
	void CreateEnemy(SC_InitPlayer _tInitPlayer);



private :
	CGrade*		m_pUI_Grade;
	CNotify*	m_pUI_Notify;


public :
	void		SetUI(CGrade* _pGrade, CNotify* _pNotify)
	{
		m_pUI_Grade = _pGrade;		m_pUI_Notify = _pNotify;
	}
	void		SetUI_Grade(CGrade* _pGrade)		{ m_pUI_Grade = _pGrade; }
	void		SetUI_Notify(CNotify* _pNotify)		{ m_pUI_Notify= _pNotify; }





private:
	CInMapMgr();
	~CInMapMgr();
};

#endif // InMapMgr_h__
