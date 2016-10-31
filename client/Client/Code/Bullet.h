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

#ifndef Bullet_h__
#define Bullet_h__

#include "Obj.h"
#include "ServerMgr.h"


class CStaticModel;
class CTexture;

class CObjCol;
class CBullet : public CObj
{
private :
	static const float MAX_DISTANCE;

private :
	int				m_iID;

	CTexture*		m_pTexture;
	CStaticModel*	m_pStaticModel;
	CServer*		m_pServerMgr;
	
	CObjCol*		m_pObjCol;
	D3DXVECTOR3		m_vStartPos;
	CInfoSubject*	m_pInfoSubject;


public:
	static CObj*	Create(CDevice*,
		const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir);


public:
	virtual HRESULT	Init();
	virtual int		Update();
	virtual void	Render();
	virtual void	Release();


private :
	void	Init_Other(const D3DXVECTOR3& _vPos,
				const D3DXVECTOR3& _vDir);

public :
	void	SendDataToServer(D3DXVECTOR3 _vDir);

	
private :
	void	CheckDie();
	void	Move();


private:
	explicit CBullet(CDevice*);
public :
	virtual ~CBullet();
};

#endif // Bullet_h__
