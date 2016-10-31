/*!
 * \file CubeColor.h
 * \date 2016/01/24 3:23
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

#ifndef CubeBuffer_h__
#define CubeBuffer_h__

#include "Buffer.h"

class CColorShader;
class CCubeBuffer : public CBuffer
{
private :
	CColorShader*		m_pColorShader;

public :
	static CResource*		Create(CDevice* _pDevice);
	virtual CResource*		Clone();

public:
	virtual HRESULT			Init();
	virtual void			Update();
	virtual void			Render();
private:
	virtual void			Release();


private :
	void			Init_Vtx();
	void			Init_Idx();
	void			Init_ColorShader();

	virtual void	CreateRasterizerState();


protected:
	explicit CCubeBuffer(CDevice*);
public:
	virtual ~CCubeBuffer();
};

#endif // CubeBuffer_h__
