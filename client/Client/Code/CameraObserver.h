/*!
 * \file CameraObserver.h
 * \date 2016/03/24 1:28
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

#ifndef CameraObserver_h__
#define CameraObserver_h__

#include "Observer.h"


class CCameraObserver : public CObserver
{
private:
	D3DXMATRIX		m_matView;
	D3DXMATRIX		m_matProj;

	D3DXVECTOR3		m_vEye;
	float			m_fZoom;


public :
	static CCameraObserver* Create();

public :
	virtual void Update(int _iMessage, UINT _nIdx = 0);


public :
	const D3DMATRIX&	GetView()		{ return m_matView; }
	const D3DMATRIX&	GetProj()		{ return m_matProj; }
	const D3DXVECTOR3&	GetEye()		{ return m_vEye; }
	const float&		GetZoom()		{ return m_fZoom; }


private :
	CCameraObserver();
public :
	virtual ~CCameraObserver();
};

#endif // CameraObserver_h__
