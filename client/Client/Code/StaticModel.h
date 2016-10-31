/*!
 * \file StaticMesh.h
 * \date 2016/02/08 18:13
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

#ifndef StaticModel_h__
#define StaticModel_h__

#include "ModelBuffer.h"


class CStaticModel : public CModelBuffer
{
public:
	static CResource*		Create(CDevice* _pDevice, const char* _pPath);
	virtual CResource*		Clone();


private:
	virtual HRESULT		Load_Model(const char*,
		FbxManager*, FbxIOSettings*, FbxScene*, FbxImporter*);
	virtual void		Init_Shader();


public:
	virtual HRESULT		Init(const char* _pPath);
public:
	virtual void		Update();

private :
	void Release();


protected:
	CStaticModel(CDevice*);
public:
	virtual ~CStaticModel();
};

#endif // StaticModel_h__
