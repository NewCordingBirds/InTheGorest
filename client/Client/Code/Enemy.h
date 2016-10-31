/*!
* \file Enemy.h
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

#ifndef Enemy_h__
#define Enemy_h__

#include "Rider.h"
#include "DynamicModel.h"

//#include "EnemyObserver.h"


class CObjMgr;
class CEnemy : public CRider
{
private :
//	CEnemyObserver*		m_pEnemyObserver;
	CObjMgr*		m_pObjMgr;

private :
	void	UpdateServerData_Physics();

public:
	static CObj*	Create(CDevice*);

public:
	virtual HRESULT	Init();
	virtual int		Update();
	virtual void	Render();
	virtual void	Release();

public :
	void	CreateBullet(D3DXVECTOR3 _vDir);


private:
	explicit CEnemy(CDevice*);
public:
	virtual ~CEnemy();
};

#endif // Enemy_h__
