/*!
 * \file AniShader.h
 * \date 2016/02/21 19:33
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

#ifndef AniShader_h__
#define AniShader_h__

#include "Shader.h"


class CAniShader : public CShader
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
	explicit CAniShader(CDevice*);
public:
	virtual ~CAniShader();
};

#endif // AniShader_h__
