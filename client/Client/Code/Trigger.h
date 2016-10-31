/*!
 * \file Trigger.h
 * \date 2016/02/12 21:55
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

#ifndef Trigger_h__
#define Trigger_h__

#include "BackGround.h"
#include "Obj.h"

#include "ResourceMgr.h"
#include "ObjCol.h"


class CObjCol;
class CTrigger abstract : public CObj
{
protected:
	CObjCol*			m_pObjCol;

private:
	void	AddComponent();

public :
	void	SetObjColPlusPos(const D3DXVECTOR3& _vPlusPos)	{ m_pObjCol->SetPlusPos(_vPlusPos); }
	void	SetObjColScale(const D3DXVECTOR3&	_vScale)		{ m_pObjCol->SetScale(_vScale); }


protected:
	explicit CTrigger(CDevice* _pDevice);
public:
	virtual ~CTrigger()		PURE;
};

#endif // Trigger_h__
