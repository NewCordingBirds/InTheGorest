/*!
 * \file Collision.h
 * \date 2016/01/24 9:38
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

#ifndef Collision_h__
#define Collision_h__

#include "Component.h"


class CCollision : public CComponent
{
protected:
	CDevice*				m_pDevice;


public:
	virtual void			Update()	PURE;
private:
	virtual void			Release()	PURE;


protected:
	CCollision(CDevice*);
public:
	virtual ~CCollision();
};

#endif // Collision_h__
