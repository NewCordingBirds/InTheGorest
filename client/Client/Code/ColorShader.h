/*!
 * \file ColorShader.h
 * \date 2016/01/30 20:59
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

#ifndef ColorShader_h__
#define ColorShader_h__

#include "Shader.h"


class CColorShader : public CShader
{
public:
	static CResource*		Create(CDevice*);
	virtual CResource*		Clone();

public:
	virtual HRESULT			Init();
	virtual void			Update();
private:
	virtual void			Release();

	 
protected:
	explicit CColorShader(CDevice*);
public:
	virtual ~CColorShader();
};


#endif // ColorShader_h__
