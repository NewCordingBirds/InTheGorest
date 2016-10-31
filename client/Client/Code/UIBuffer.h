/*!
* \file UIBuffer.h
* \date 2016/02/10 17:30
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

#ifndef UIBuffer_h__
#define UIBuffer_h__

#include "Buffer.h"


class CTextureShader;
class CUIBuffer : public CBuffer
{
private:
	CTextureShader*		m_pTextureShader;

public:
	static CResource*		Create(CDevice* _pDevice);
	virtual CResource*		Clone();

public:
	virtual HRESULT			Init();
	virtual void			Update();
	virtual void			Render();
private:
	virtual void			Release();


private:
	void			Init_Vtx();
	void			Init_Idx();
	HRESULT			Init_TextureShader();

	virtual void	CreateRasterizerState();


protected:
	explicit CUIBuffer(CDevice*);
public:
	virtual ~CUIBuffer();
};

#endif // UIBuffer_h__
