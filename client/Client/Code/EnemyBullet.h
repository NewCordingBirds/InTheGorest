/*!
* \file Bullet.h
* \date 2016/04/15 23:01
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

#ifndef EnemyBullet_h__
#define EnemyBullet_h__

#include "Obj.h"
#include "ServerMgr.h"
#include "UserInfo.h"

class CStaticModel;
class CTexture;

class CObjCol;
class CEnemyBullet : public CObj
{
private:
	static const float MAX_DISTANCE;

	CTexture*		m_pTexture;
	CStaticModel*	m_pStaticModel;

	CObjCol*		m_pObjCol;
	D3DXVECTOR3		m_vStartPos;

	CServer*		m_pServerMgr;
	CUserInfo*		m_pUserInfo;

public:
	static CObj*	Create(CDevice*,
		const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir);


public:
	virtual HRESULT	Init();
	virtual int		Update();
	virtual void	Render();
	virtual void	Release();
	

private:
	void	Init_Other(const D3DXVECTOR3& _vPos,
		const D3DXVECTOR3& _vDir);
	void    UpdateServerData_Physics();

private:
	void	CheckDie();


private:
	explicit CEnemyBullet(CDevice*);
public:
	virtual ~CEnemyBullet();
};

#endif // Bullet_h__
