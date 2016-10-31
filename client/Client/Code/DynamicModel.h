/*!
 * \file DynamicModel.h
 * \date 2016/02/08 18:27
 *
 * \author �����
 * Contact: bluevill04@gmail.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#pragma once

#ifndef DynamicModel_h__
#define DynamicModel_h__

#include <xnamath.h>
#include "ModelBuffer.h"
#include "PlayingInfo.h"


struct VS_CB_BONE_MATRIX
{
	XMMATRIX m_XMmtxBone[BONE_MATRIX_NUM];
};

/*
//�ִϸ��̼� ���� ������ �Դϴ�.
float g_fAnimationPlayTime = 0.0f;				//�ִϸ��̼� ��� �ð�
XMMATRIX **g_pAnimationMatrix = nullptr;		//�ִϸ��̼� Matrix[�ð���][�ε�����]
XMMATRIX * g_pBaseBoneMatrix = nullptr;			//�ִϸ��̼� BaseMatrix[�ε�����]
XMMATRIX ** g_pResultMatrix = nullptr;			//�ִϸ��̼� ���� ��ȯ Matrix[�ð���][�ε�����]
long long g_llAnimationMaxTime = 0;				//�ִϸ��̼� �ִ����
unsigned int g_uiAnimationNodeIndexCount = 0;	//�ִϸ��̼� ����� ����
std::map<std::string, unsigned int> g_mIndexByName;

//�ִϸ��̼ǿ� ���õ� ��� ����
ID3D11Buffer *g_pd3dcbBoneMatrix = nullptr;
D3D11_MAPPED_SUBRESOURCE g_d3dMappedResource;
VS_CB_BONE_MATRIX *g_pcbBoneMatrix = nullptr;
*/

class CAniBuffer;
struct Animation 
{
	string			strAniName;
	CAniBuffer*		pAniBuffer;
	
	long long		llAniMaxTime;
	//float			fAniPlayTimer;
	float			fAniPlaySpeed;

	XMMATRIX**		ppAniMatrix;
	XMMATRIX*		pBaseBoneMatrix;
	XMMATRIX**		ppResultMatrix;
	
	unsigned int	nAniNodeIdxCnt;
	map<string, unsigned int>	mapIndexByName;


	ID3D11Buffer*				pBoneMatrixBuffer;
	D3D11_MAPPED_SUBRESOURCE	tMappedResource;
	VS_CB_BONE_MATRIX*			pBoneMatrix;

	Animation()
		: pAniBuffer(NULL)
		, llAniMaxTime(0)
		//, fAniPlayTimer(0.f)
		, fAniPlaySpeed(100.f)

		, ppAniMatrix(NULL)
		, pBaseBoneMatrix(NULL)
		, ppResultMatrix(NULL)

		, nAniNodeIdxCnt(0)
		, pBoneMatrixBuffer(NULL)
		, pBoneMatrix(NULL)
	{}
};


class CDynamicModel : public CModelBuffer
{
private :
	WORD					m_wCurrenAniIdx;
	vector<Animation*>		m_vecAni;
	int						m_iRepeatTime;

public:
	static CResource*		Create(CDevice* _pDevice,
									const char* _pPath,
									vector<string> _vecAniName);
	virtual CResource*		Clone();
	float					m_fAniPlayTimer;
	bool					Animend;
	bool					Yamae;

protected:
	virtual HRESULT		Load_Model(const char*,
		vector<string> _vecAniName,
		FbxManager*, FbxIOSettings*, FbxScene*, FbxImporter*);

	virtual void		Init_Shader();


public:
	virtual HRESULT		Init(const char* _pPath, vector<string> _vecAniName);
public:
	virtual void		Update();

private:
	void Release();
	void Release_Animation();


public :
	void		PlayAnimation(int _iIdx);
	void		BWPlayAnim(int _iIdx);



protected:
	CDynamicModel(CDevice*);
public:
	virtual ~CDynamicModel();
};


#endif // DynamicModel_h__
