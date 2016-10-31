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

#ifndef Player_h__
#define Player_h__

#include "Rider.h"
#include "DynamicModel.h"

#include "InfoSubject.h"
#include "PlayerObserver.h"
#include "ServerMgr.h"
#include "Sound.h"

enum AvoidState{
	keydowntrig,
	keyuptrig,
	avoidtrig,
	earlyup,
};

enum TempTrig{
	noneTrigger,
	boosterTrigger,
	colTrigger,
};
	
class CKeyMgr;
class CTimer;
class CUIObserver;
class CObjMgr;
class CPlayer : public CRider
{
private :
	CKeyMgr*	m_pKeyMgr;
	CTimer*		m_pTimer;
	CObjMgr*	m_pObjMgr;
	CServer*	m_pServer;

	CInfoSubject*		m_pInfoSubject;
	CUIObserver*		m_pUIObserver;
	

	int		m_AvoidState;

	int		m_TempPhysicsState;

	bool	m_bAvoidTrig;

public :
	static CObj*	Create(CDevice*);
	SoundEngine		sound;

public:
	virtual HRESULT	Init();
	virtual int		Update();
	virtual void	Render();
	virtual void	Release();

	float		m_fProgressRate;
	int			m_iprogressnum;

private :
	void CheckCol();

	void CheckTestKey();

	void CheckKey();
	void CheckKey_Move(float _fTime);
	void CheckKey_Rotate(float _fTime);

public :
	virtual void SetAnimation(CPlayingInfo::AniState _eAniState);


private :
	static enum BulletState
	{
		BULLET_STATE_NONE,
		BULLET_STATE_RELOAD,
	};

	BulletState		m_eBulletState;

	void CheckKey_Bullet(float _fTime);
	void CheckKey_UseRecoster();
	void CheckKey_Avoid(float _fTime);

public :
	void CreateBullet();
private :
	void ReloadingBullet();


private :
	void FixMouse();


private :
	CS_Key		m_tKey;
	CS_Rotate	m_tRotate;
	CS_Shoot	m_tShoot;
	void		UpdateServerData_Physics();
	void		RotateToServer();


private :
	void		UpdateCurMapLength();


private :
	explicit CPlayer(CDevice*);
public :
	virtual ~CPlayer();
private:
	float posstarttime;
	float posendtime;

	float rotatestarttime;
	float rotateendtime;
};

#endif // Player_h__
