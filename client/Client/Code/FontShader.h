/*!
 * \file FontShader.h
 * \date 2016/03/28 20:35
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

#ifndef FontShader_h__
#define FontShader_h__

#include "Shader.h"


class CFontShader : public CShader
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
	explicit CFontShader(CDevice*);
public:
	virtual ~CFontShader();
};

#endif // FontShader_h__
