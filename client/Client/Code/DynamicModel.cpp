#include "stdafx.h"
#include "DynamicModel.h"

#include <assert.h>
#include "ResourceMgr.h"
#include "AniShader.h"
#include "AniBuffer.h"

#include "FbxParser.h"
#include "Timer.h"
#include <conio.h>



CDynamicModel::CDynamicModel(CDevice* _pDevice)
: CModelBuffer(_pDevice)
, m_wCurrenAniIdx(-1)
, m_fAniPlayTimer(0.f)
{
	
} 

CDynamicModel::~CDynamicModel()
{
	Release();
}

CResource* CDynamicModel::Create(CDevice* _pDevice,
								const char* _pPath,
								vector<string> _vecAniName)
{
	CDynamicModel* pComponent = new CDynamicModel(_pDevice);

	if (FAILED(pComponent->Init(_pPath, _vecAniName)))
		::Safe_Delete(pComponent);

	return pComponent;
}

CResource* CDynamicModel::Clone()
{
	++(*m_pRefCnt);
	return new CDynamicModel(*this);
}

HRESULT CDynamicModel::Init(const char* _pPath,
							vector<string> _vecAniName)
{
	FbxManager*	pFBXManager = FbxManager::Create();
	FbxIOSettings* pIOsettings = FbxIOSettings::Create(pFBXManager, IOSROOT);
	pFBXManager->SetIOSettings(pIOsettings);
	FbxScene* pFBXScene = FbxScene::Create(pFBXManager, "");

	FbxImporter* pImporter = FbxImporter::Create(pFBXManager, "");


	FAILED_CHECK_RETURN(
		Load_Model(_pPath, _vecAniName, pFBXManager, pIOsettings, pFBXScene, pImporter),
		E_FAIL);

	CModelBuffer::CreateRasterizerState();
	Init_Shader();
	

	pFBXScene->Destroy();
	pImporter->Destroy();
	pIOsettings->Destroy();
	pFBXManager->Destroy();

	Animend = false;
	Yamae = false;

	return S_OK;
}

HRESULT CDynamicModel::Load_Model(const char* _pPath,
								vector<string> _vecAniName,
								FbxManager* _pFBXManager,
								FbxIOSettings* _pIOsettings,
								FbxScene* _pFBXScene,
								FbxImporter* _pImporter)
{
	int		iFileFormat = EOF;
	string	strFullPath;
	string	strExtenstiom = ".FBX";

	FbxAxisSystem CurrAxisSystem;
	FbxAxisSystem DestAxisSystem = FbxAxisSystem::eMayaYUp;
	FbxGeometryConverter lGeomConverter(_pFBXManager);

	FbxNode*		pFbxRootNode;
	Animation*		pAni;


	for (unsigned int i = 0; i < _vecAniName.size(); ++i)
	{
		// http://www.slideshare.net/quxn6/c4316-alignment-error
		pAni = new Animation();
		pAni->strAniName = _vecAniName[i];

		strFullPath.clear();
		strFullPath = _pPath;
		strFullPath += _vecAniName[i];
		strFullPath += strExtenstiom;

		_pFBXManager->GetIOPluginRegistry()->DetectReaderFileFormat(strFullPath.c_str(), iFileFormat);

		FALSE_CHECK_MSG(
			_pImporter->Initialize(strFullPath.c_str(), iFileFormat),
			L"No Find Fbx Path");

		_pImporter->Import(_pFBXScene);

		CurrAxisSystem = _pFBXScene->GetGlobalSettings().GetAxisSystem();
		if (CurrAxisSystem != DestAxisSystem) DestAxisSystem.ConvertScene(_pFBXScene);

		lGeomConverter.Triangulate(_pFBXScene, true);

		pFbxRootNode = _pFBXScene->GetRootNode();
		NULL_CHECK_RETURN(pFbxRootNode, E_FAIL);

		//pAni->pBaseBoneMatrix = new XMMATRIX[BONE_MATRIX_NUM];
		pAni->pBaseBoneMatrix = (XMMATRIX*)_aligned_malloc(
			sizeof(XMMATRIX) * BONE_MATRIX_NUM, 16);
		//new (pAni->pBaseBoneMatrix)XMMATRIX();

		for (int i = 0; i < BONE_MATRIX_NUM; ++i)
			pAni->pBaseBoneMatrix[i] = XMMatrixIdentity();

		pAni->pAniBuffer = new CAniBuffer;
		pAni->pAniBuffer->SetFbxBoneIndex(&(pAni->mapIndexByName), pFbxRootNode);

		
		CFbxParser::ParsingVertex(pFbxRootNode, pAni);


		// Animation -----------------------
		auto AnimStack = _pFBXScene->GetSrcObject<FbxAnimStack>();
		NULL_CHECK_RETURN(AnimStack, E_FAIL);

		pAni->llAniMaxTime = AnimStack->GetLocalTimeSpan().GetDuration().GetMilliSeconds();
		pAni->nAniNodeIdxCnt = pAni->mapIndexByName.size();

		pAni->ppAniMatrix = new XMMATRIX*[unsigned int(pAni->llAniMaxTime / 10)];
		pAni->ppResultMatrix = new XMMATRIX*[unsigned int(pAni->llAniMaxTime / 10)];

		for (long long i = 0; i < pAni->llAniMaxTime / 10; ++i)
		{
			//pAni->ppAniMatrix[i] = new XMMATRIX[pAni->nAniNodeIdxCnt];
			//pAni->ppResultMatrix[i] = new XMMATRIX[pAni->nAniNodeIdxCnt];

			pAni->ppAniMatrix[i] = (XMMATRIX*)_aligned_malloc(
				sizeof(XMMATRIX) * pAni->nAniNodeIdxCnt, 16);
			pAni->ppResultMatrix[i] = (XMMATRIX*)_aligned_malloc(
				sizeof(XMMATRIX) * pAni->nAniNodeIdxCnt, 16);
		}
		
		//Animation 상수 버퍼 생성
		{
			//최초 상수버퍼를 생성한다.
			D3D11_BUFFER_DESC BD;
			::ZeroMemory(&BD, sizeof(D3D11_BUFFER_DESC));
			BD.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
			BD.ByteWidth = sizeof(VS_CB_BONE_MATRIX);
			BD.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			BD.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

			m_pDevice->GetDevice()->CreateBuffer(&BD, NULL, &pAni->pBoneMatrixBuffer);

			m_pDevice->GetDeviceContext()->Map(
				pAni->pBoneMatrixBuffer,
				NULL,
				D3D11_MAP::D3D11_MAP_WRITE_DISCARD,
				NULL,
				&pAni->tMappedResource);

			pAni->pBoneMatrix = (VS_CB_BONE_MATRIX *)pAni->tMappedResource.pData;

			for (int i = 0; i < BONE_MATRIX_NUM; i++)
				pAni->pBoneMatrix->m_XMmtxBone[i] = XMMatrixIdentity();

			m_pDevice->GetDeviceContext()->Unmap(
				pAni->pBoneMatrixBuffer,
				NULL);
		}

		CFbxParser::ParsingAnimation(pFbxRootNode, pAni, AnimStack);


		//XMMATRIX ResultMtx;
		for (long long i = 0; i < pAni->llAniMaxTime / 10; ++i)
		{
			for (unsigned int j = 0; j < pAni->nAniNodeIdxCnt; ++j)
			{
				XMMATRIX ResultMtx(pAni->pBaseBoneMatrix[j] * (XMMATRIX)(pAni->ppAniMatrix[i][j]));
				pAni->ppResultMatrix[i][j] = ResultMtx;
			}
		}

		m_vecAni.push_back(pAni);
	}

	return S_OK;
}


void CDynamicModel::Init_Shader()
{
	// Ani Shader ---------
	m_pShader = dynamic_cast<CAniShader*>(
		CResourceMgr::GetInstance()->CloneResource(
		CResourceMgr::RESOURCE_ATTRI_STATIC, CResourceMgr::RESOURCE_TYPE_SHADER, L"Shader_Ani"));
	NULL_CHECK(m_pShader);
}

void CDynamicModel::Update()
{

}

void CDynamicModel::PlayAnimation(int _iIdx)
{
	if (_iIdx < 0 || (unsigned)_iIdx > m_vecAni.size())
		return ;


	m_pShader->Render();
	m_pDevice->GetDeviceContext()->RSSetState(m_pRasterizerState);

	//m_vecAni[_iIdx]->fAniPlayTimer
	m_fAniPlayTimer
		+= m_vecAni[_iIdx]->fAniPlaySpeed * CTimer::GetInstance()->GetTime();

	//if (m_vecAni[_iIdx]->fAniPlayTimer > m_vecAni[_iIdx]->llAniMaxTime / 10)
	//	m_vecAni[_iIdx]->fAniPlayTimer = 0;

	//애니메이션 스테이트 체크해서 어디서부터 반복할지 넣어준다
	switch (_iIdx)
	{
	case CPlayingInfo::ANI_STATE_BOOSTER:
	case CPlayingInfo::ANI_STATE_DEAD:
	case CPlayingInfo::ANI_STATE_BREAK:
		if (m_fAniPlayTimer > m_vecAni[_iIdx]->llAniMaxTime / 10){
			m_fAniPlayTimer = (m_vecAni[_iIdx]->llAniMaxTime / 10) - 1.f;
			Animend = true;
		}
		break;
	default:
		if (m_fAniPlayTimer > m_vecAni[_iIdx]->llAniMaxTime / 10){
			m_fAniPlayTimer = 0;
			Animend = true;
		}
		break;
	}


	for (unsigned int i = 0; i < m_vecAni[_iIdx]->nAniNodeIdxCnt; ++i)
	{
		m_vecAni[_iIdx]->pBoneMatrix->m_XMmtxBone[i]
			//= m_vecAni[_iIdx]->ppResultMatrix[int(m_vecAni[_iIdx]->fAniPlayTimer)][i];
			= m_vecAni[_iIdx]->ppResultMatrix[int(m_fAniPlayTimer)][i];
	}
	

	if (m_vecAni[_iIdx]->pBoneMatrixBuffer != NULL)
	{
		m_pDevice->GetDeviceContext()->VSSetConstantBuffers(
			VS_SLOT_BONE_MATRIX, 1, &m_vecAni[_iIdx]->pBoneMatrixBuffer);
	}

	m_vecAni[_iIdx]->pAniBuffer->Render();
	m_wCurrenAniIdx = _iIdx;
}

void CDynamicModel::BWPlayAnim(int _iIdx)
{
	//애니메이션 스테이트 체크해서 어디서부터 반복할지 넣어준다
	switch (_iIdx)
	{
	case CPlayingInfo::ANI_STATE_BOOSTER:
		m_iRepeatTime = 120;
		break;
	case CPlayingInfo::ANI_STATE_DEAD:
		m_iRepeatTime = 100;
		break;
	case CPlayingInfo::ANI_STATE_BREAK:
		m_iRepeatTime = 50;
		break;
	default:
		m_iRepeatTime = 0;
		break;
	}

	if (_iIdx < 0 || (unsigned)_iIdx > m_vecAni.size())
		return ;


	m_pShader->Render();
	m_pDevice->GetDeviceContext()->RSSetState(m_pRasterizerState);


	

	//반대로 돌린다아ㅏ
	m_fAniPlayTimer
		-= m_vecAni[_iIdx]->fAniPlaySpeed * CTimer::GetInstance()->GetTime();

	//AllocConsole();
	//_cprintf("%lf\n", m_fAniPlayTimer);

	if (m_fAniPlayTimer <= m_iRepeatTime){
		Animend = false;
		//return true;
	}

	//-----------------------------------------------------

	for (unsigned int i = 0; i < m_vecAni[_iIdx]->nAniNodeIdxCnt; ++i)
	{
		m_vecAni[_iIdx]->pBoneMatrix->m_XMmtxBone[i]
			= m_vecAni[_iIdx]->ppResultMatrix[int(m_fAniPlayTimer)][i];
	}


	if (m_vecAni[_iIdx]->pBoneMatrixBuffer != NULL)
	{
		m_pDevice->GetDeviceContext()->VSSetConstantBuffers(
			VS_SLOT_BONE_MATRIX, 1, &m_vecAni[_iIdx]->pBoneMatrixBuffer);
	}

	m_vecAni[_iIdx]->pAniBuffer->Render();
	m_wCurrenAniIdx = _iIdx;
	//return false;
}


void CDynamicModel::Release_Animation()
{
	//for (unsigned int i = 0; i < m_vecAni.size(); ++i)
	//{
	//	for (int j = 0; j < BONE_MATRIX_NUM; ++j)
	//	{
	//		_aligned_free((void*)&(m_vecAni[i]->pBaseBoneMatrix[j]));

	//		for (int k = 0; k < m_vecAni[i]->llAniMaxTime / 10; ++k)
	//		{
	//			_aligned_free((void*)&(m_vecAni[i]->ppAniMatrix[j][k]));
	//			_aligned_free((void*)&(m_vecAni[i]->ppResultMatrix[j][k]));
	//		}
	//	}

	//	::Safe_Delete(m_vecAni[i]->pAniBuffer);

	//	m_vecAni[i]->pBoneMatrixBuffer->Release();
	//	::Safe_Delete(m_vecAni[i]->pBoneMatrix);

	//	delete m_vecAni[i];
	//	m_vecAni[i] = NULL;
	//}
}

void CDynamicModel::Release()
{
	if ((*m_pRefCnt) == 0)
	{
		Release_Animation();
		::Safe_Release(m_pVtxBuffer);
		::Safe_Release(m_pIdxBuffer);
		::Safe_Delete(m_pShader);
		CResource::Release_RefCnt();
	}
	else --(*m_pRefCnt);
}
