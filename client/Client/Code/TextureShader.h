/*!
 * \file TextureShader.h
 * \date 2016/02/08 22:19
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

#ifndef TextureShader_h__
#define TextureShader_h__

#include "Shader.h"


class CTextureShader : public CShader
{
public:
	static CResource*		Create(CDevice*, bool);
	virtual CResource*		Clone();

public:
	virtual HRESULT			Init();
	virtual void			Update();
private:
	virtual void			Release();


private :
	static bool		m_bUI;


protected:
	explicit CTextureShader(CDevice*);
public:
	virtual ~CTextureShader();
};

#endif // TextureShader_h__
