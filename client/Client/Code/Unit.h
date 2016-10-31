/*!
 * \file Unit.h
 * \date 2016/01/24 11:12
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

#ifndef Unit_h__
#define Unit_h__


#include "Obj.h"

#include "ObjCol.h"

#include "StaticModel.h"
#include "DynamicModel.h"
#include "Texture.h"


class CUnit abstract : public CObj
{
protected :
	CObjCol*			m_pObjCol;

protected:
	CTexture*			m_pTexture;
	CDynamicModel*		m_pDynamicModel;


private :
	void	AddComponent();


protected:
	explicit CUnit(CDevice* _pDevice);
public :
	virtual ~CUnit()	PURE;
};

#endif // Unit_h__
