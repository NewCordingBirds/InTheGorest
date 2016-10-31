/*!
 * \file UI.h
 * \date 2016/03/26 16:34
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

#ifndef UI_h__
#define UI_h__

#include "Obj.h"

#include "Texture.h"
#include "UIBuffer.h"
#include "CameraObserver.h"

#include "Font.h"


class CDevice;
class CUI abstract : public CObj
{
protected:
	ID3D11Buffer*		m_pViewBuffer;
	ID3D11Buffer*		m_pProjBuffer;

protected:
	CCameraObserver*		m_pCameraObserver;
	ID3D11DeviceContext*	m_pDeviceContext;


private :
	void	Init_ViewBuffer();
	void	Init_ProjBuffer();
	void	Init_CameraObserver();

	void	Release_CameraObserver();

protected:
	void	Invalidate_View(D3DXMATRIX _matView);


public:
	virtual HRESULT	Init()			PURE;
	virtual int		Update()		PURE;
	virtual void	Render()		PURE;
	virtual void	Release()		PURE;

protected:
	explicit CUI(CDevice*);
public :
	virtual ~CUI()		PURE;
};

struct Sprite
{
	D3DXVECTOR2		vPos;
	D3DXVECTOR2		fHalfSize;

	CTexture*		pTexture;
	CUIBuffer*		pBuffer;

	D3DXMATRIX		m_matView;

	Sprite()
		: vPos(0.f, 0.f), fHalfSize(0.f, 0.f)
		, pTexture(NULL), pBuffer(NULL)
	{	D3DXMatrixIdentity(&m_matView); }
};

#endif // UI_h__
