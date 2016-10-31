/*!
 * \file FbxBuffer.h
 * \date 2016/01/31 12:58
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

#ifndef ModelBuffer_h__
#define ModelBuffer_h__

#include "Buffer.h"
#include "Shader.h"


class CModelBuffer abstract : public CBuffer
{
protected :
	CShader*		m_pShader;


protected:
	virtual void		Init_Shader()	PURE;


public :
	virtual CResource*	Clone()		PURE;
public:
	virtual void		Render();

protected:
	virtual void	CreateRasterizerState();

protected:
	void MakeVertexNormal(BYTE* _pVertices, WORD* _pIndices);
	void SetNormalVectorByBasic(BYTE* _pVertices);
	void SetNormalVectorByAverage(BYTE* _pVertices, WORD* _pIndices, int _iPrimitives, int _iOffset, bool _bStrip);
	D3DXVECTOR3 GetTriAngleNormal(BYTE* _pVertices, USHORT _nIndex_0, USHORT _nIndex_1, USHORT _nIndex_2);

protected :
	CModelBuffer(CDevice*);
public :
	virtual ~CModelBuffer();
};

#endif // ModelBuffer_h__
