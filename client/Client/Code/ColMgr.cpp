#include "stdafx.h"
#include "ColMgr.h"

#include "Trigger.h"
#include "Unit.h"

IMPLEMENT_SINGLETON(CColMgr)


CColMgr::CColMgr()
:m_pMapObjList_GameLogic(NULL)
{
}

CColMgr::~CColMgr()
{
	Release();
}

void CColMgr::SetMapObjList(CLayer::LayerType _eLayerType,
							map<wstring, ListObj>* _pMapObjList)
{
	map<wstring, ListObj>::iterator iter;

	switch (_eLayerType)
	{
	case CLayer::LAYERTYPE_GAMELOGIC:
		m_pMapObjList_GameLogic = _pMapObjList;


		iter = (*m_pMapObjList_GameLogic).find(L"ProgressRateZone");
		if (iter != (*m_pMapObjList_GameLogic).end())
			m_listObj_ProgressRateZone = (*iter).second;

		iter = (*m_pMapObjList_GameLogic).find(L"BoosterZone");
		if (iter != (*m_pMapObjList_GameLogic).end())
			m_listObj_BoosterZone = (*iter).second;

		iter = (*m_pMapObjList_GameLogic).find(L"Item");
		if (iter != (*m_pMapObjList_GameLogic).end())
			m_listObj_Item = (*iter).second;

		iter = (*m_pMapObjList_GameLogic).find(L"Enemy");
		if (iter != (*m_pMapObjList_GameLogic).end())
			m_listObj_Enemy = (*iter).second;


		MakeListDiviSpace();
		break;

	default:
		break;
	}
}

void CColMgr::MakeListDiviSpace()
{
	DiviSpace*	pDiviSpace = NULL;
	D3DXVECTOR3 vStartPos = DIVITIONSPACE_SIZE_HALF;

	ListObj listBackGroundTmp = ((*m_pMapObjList_GameLogic)[L"UnmovableZone"]);

	if (listBackGroundTmp.size() <= 0)
		return;

	ListObj::iterator iter_UnMovableZone;
	ListObj::iterator iter_end_UnMovableZone;
	ListObj::iterator iter_Item;
	ListObj::iterator iter_end_Item;

	float fMinX, fMaxX;
	float fMinY, fMaxY;
	float fMinZ, fMaxZ;

	listBackGroundTmp.sort(CColMgr::SortObjColPosX);
	iter_UnMovableZone = listBackGroundTmp.begin();
	fMinX = ((CObjCol*)((*iter_UnMovableZone)->GetComponent(L"ObjCol")))->GetPos().x;
	iter_UnMovableZone = --(listBackGroundTmp.end());
	fMaxX = ((CObjCol*)((*iter_UnMovableZone)->GetComponent(L"ObjCol")))->GetPos().x;

	listBackGroundTmp.sort(CColMgr::SortObjColPosY);
	iter_UnMovableZone = listBackGroundTmp.begin();
	fMinY = ((CObjCol*)((*iter_UnMovableZone)->GetComponent(L"ObjCol")))->GetPos().y;
	iter_UnMovableZone = --(listBackGroundTmp.end());
	fMaxY = ((CObjCol*)((*iter_UnMovableZone)->GetComponent(L"ObjCol")))->GetPos().y;

	listBackGroundTmp.sort(CColMgr::SortObjColPosZ);
	iter_UnMovableZone = listBackGroundTmp.begin();
	fMinZ = ((CObjCol*)((*iter_UnMovableZone)->GetComponent(L"ObjCol")))->GetPos().z;
	iter_UnMovableZone = --(listBackGroundTmp.end());
	fMaxZ = ((CObjCol*)((*iter_UnMovableZone)->GetComponent(L"ObjCol")))->GetPos().z;


	float fDistanceX, fDistanceY, fDistanceZ;
	D3DXVECTOR3 vIterPos, vIterScale;

	int x_End = int((fMaxX / DIVITIONSPACE_GAB.x) + 1);
	int y_End = int((fMaxY / DIVITIONSPACE_GAB.y) + 1);
	int z_End = int((fMaxZ / DIVITIONSPACE_GAB.z) + 1);


	for (int x = int((fMinX / DIVITIONSPACE_GAB.x) - 1); x < x_End; ++x)
	{
		for (int y = int((fMinY / DIVITIONSPACE_GAB.y) - 1); y < y_End; ++y)
		{
			for (int z = int((fMinZ / DIVITIONSPACE_GAB.z) - 1); z < z_End; ++z)
			{
				pDiviSpace = new DiviSpace;
				pDiviSpace->vPos = D3DXVECTOR3(
					vStartPos.x + DIVITIONSPACE_GAB.x * x,
					vStartPos.y + DIVITIONSPACE_GAB.y * y,
					vStartPos.z + DIVITIONSPACE_GAB.z * z);


				iter_UnMovableZone = listBackGroundTmp.begin();
				iter_end_UnMovableZone = listBackGroundTmp.end();

				for (; iter_UnMovableZone != iter_end_UnMovableZone; ++iter_UnMovableZone)
				{
					vIterPos = ((CObjCol*)((*iter_UnMovableZone)->GetComponent(L"ObjCol")))->GetPos();
					vIterScale = ((CObjCol*)((*iter_UnMovableZone)->GetComponent(L"ObjCol")))->GetScale();

					fDistanceX = abs(pDiviSpace->vPos.x - vIterPos.x);
					fDistanceY = abs(pDiviSpace->vPos.y - vIterPos.y);
					fDistanceZ = abs(pDiviSpace->vPos.z - vIterPos.z);

					if ((fDistanceX < DIVITIONSPACE_SIZE_HALF.x + vIterScale.x)
						&& (fDistanceY < DIVITIONSPACE_SIZE_HALF.y + vIterScale.y)
						&& (fDistanceZ < DIVITIONSPACE_SIZE_HALF.z + vIterScale.z))
					{
						pDiviSpace->listUnMovableZoneObjCol.push_back(
							((CObjCol*)((*iter_UnMovableZone)->GetComponent(L"ObjCol"))));
					}
				}


				iter_Item		= m_listObj_Item.begin();
				iter_end_Item	= m_listObj_Item.end();

				for (; iter_Item != iter_end_Item; ++iter_Item)
				{
					vIterPos = ((CObjCol*)((*iter_Item)->GetComponent(L"ObjCol")))->GetPos();
					vIterScale = ((CObjCol*)((*iter_Item)->GetComponent(L"ObjCol")))->GetScale();

					fDistanceX = abs(pDiviSpace->vPos.x - vIterPos.x);
					fDistanceY = abs(pDiviSpace->vPos.y - vIterPos.y);
					fDistanceZ = abs(pDiviSpace->vPos.z - vIterPos.z);

					if ((fDistanceX < DIVITIONSPACE_SIZE_HALF.x + vIterScale.x)
						&& (fDistanceY < DIVITIONSPACE_SIZE_HALF.y + vIterScale.y)
						&& (fDistanceZ < DIVITIONSPACE_SIZE_HALF.z + vIterScale.z))
					{
						pDiviSpace->listItemObj.push_back((*iter_Item));
					}
				}

				m_listDiviSpace.push_back(pDiviSpace);
			}
		}
	}
}

bool CColMgr::CheckColObjToUnmovable(const CObjCol* _pMyObjCol,
									D3DXVECTOR3** _ppDirOut /* = null */)
{
	list<DiviSpace*>::iterator iter_DiviSpace = m_listDiviSpace.begin();
	list<DiviSpace*>::iterator iter_DiviSpace_End = m_listDiviSpace.end();

	float fDistanceX, fDistanceY, fDistanceZ;
	D3DXVECTOR3 vMyPos, vMyScalse;
	D3DXVECTOR3 vIterPos;


	vMyPos = _pMyObjCol->GetPos();
	vMyScalse = _pMyObjCol->GetScale();


	// Check Col Obj To Divition Space
	for (; iter_DiviSpace != iter_DiviSpace_End; ++iter_DiviSpace)
	{
		vIterPos = (*iter_DiviSpace)->vPos;

		//현재위치와 충돌박스와의 거리. 양수로 구하기 위해 절댓값사용.
		fDistanceX = abs(vMyPos.x - vIterPos.x);
		fDistanceY = abs(vMyPos.y - vIterPos.y);
		fDistanceZ = abs(vMyPos.z - vIterPos.z);

		//충돌거리를 첵크한다 (구충돌)
		if ((fDistanceX < vMyScalse.x + DIVITIONSPACE_SIZE_HALF.x)
			&& (fDistanceY < vMyScalse.y + DIVITIONSPACE_SIZE_HALF.y)
			&& (fDistanceZ < vMyScalse.z + DIVITIONSPACE_SIZE_HALF.z))
		{
			//트루일때 한번 더 체크하기위해 넘겨줌
			return CheckColObjToUnmovableInDiviSpace((*iter_DiviSpace), _pMyObjCol, _ppDirOut);	
		}
	}

	return false;
}

bool CColMgr::CheckColObjToUnmovableInDiviSpace(DiviSpace* _pDiviSpace,
												const CObjCol* _pMyObjCol,
												D3DXVECTOR3** _ppDirOut /* = null */)
{
	list<CObjCol*>::iterator iter = _pDiviSpace->listUnMovableZoneObjCol.begin();
	list<CObjCol*>::iterator iter_End = _pDiviSpace->listUnMovableZoneObjCol.end();

	D3DXVECTOR3 vDistance;
	float fMyRadius, fIterRadius;
	D3DXVECTOR3 vMyPos, vIterPos;
	D3DXMATRIX matMyWorld, matIterWorld;

	vMyPos = _pMyObjCol->GetPos();
	fMyRadius = _pMyObjCol->GetRadius();
	matMyWorld = _pMyObjCol->GetWorldMatrix();


	for (; iter != iter_End; ++iter)
	{
		vIterPos = (*iter)->GetPos();
		fIterRadius = (*iter)->GetRadius();
		matIterWorld = (*iter)->GetWorldMatrix();

		//넘어오면 여기서 obb 충돌체크를 한 번 더 함.
		if(!CheckColSphere(vMyPos, fMyRadius,vIterPos, fIterRadius)
			|| !CheckColRect_OBB(matMyWorld, matIterWorld, &vDistance))
			continue;


		if (_ppDirOut != NULL)
 			*_ppDirOut = &((*iter)->GetDir());

		//obb충돌까지 체크되면 true를 넘겨준다.구충돌이 되어있어도
		//obb충돌이 체크되지 않으면 false.
		return true;
	}

	_ppDirOut = NULL;
	return false;
}

bool CColMgr::CheckColObjToItem(const CObjCol* _pMyObjCol,
								CObj** _ppTargetOut /*= NULL*/)
{
	list<DiviSpace*>::iterator iter_DiviSpace = m_listDiviSpace.begin();
	list<DiviSpace*>::iterator iter_DiviSpace_End = m_listDiviSpace.end();

	float fDistanceX, fDistanceY, fDistanceZ;
	D3DXVECTOR3 vMyPos, vMyScalse;
	D3DXVECTOR3 vIterPos;


	vMyPos = _pMyObjCol->GetPos();
	vMyScalse = _pMyObjCol->GetScale();


	// Check Col Obj To Divition Space
	for (; iter_DiviSpace != iter_DiviSpace_End; ++iter_DiviSpace)
	{
		vIterPos = (*iter_DiviSpace)->vPos;

		fDistanceX = abs(vMyPos.x - vIterPos.x);
		fDistanceY = abs(vMyPos.y - vIterPos.y);
		fDistanceZ = abs(vMyPos.z - vIterPos.z);


		if ((fDistanceX < vMyScalse.x + DIVITIONSPACE_SIZE_HALF.x)
			&& (fDistanceY < vMyScalse.y + DIVITIONSPACE_SIZE_HALF.y)
			&& (fDistanceZ < vMyScalse.z + DIVITIONSPACE_SIZE_HALF.z))
		{
			return CheckColObjToItemInDiviSpace((*iter_DiviSpace), _pMyObjCol, _ppTargetOut);
		}
	}

	return false;
}

bool CColMgr::CheckColObjToItemInDiviSpace(DiviSpace* _pDiviSpace,
											const CObjCol* _pMyObjCol,
											CObj** _ppTargetOut /*= NULL*/)
{
	ListObj::iterator iter = _pDiviSpace->listItemObj.begin();
	ListObj::iterator iter_End = _pDiviSpace->listItemObj.end();

	float fMyRadius, fIterRadius;
	D3DXVECTOR3 vMyPos, vIterPos;
	D3DXMATRIX matMyWorld, matIterWorld;
	CObjCol* pIterObjCol = NULL;

	vMyPos = _pMyObjCol->GetPos();
	fMyRadius = _pMyObjCol->GetRadius();
	matMyWorld = _pMyObjCol->GetWorldMatrix();


	for (; iter != iter_End; ++iter)
	{
		if ((*iter)->GetObjState() != CObj::OBJ_STATE_ALIVE)
			continue;

		pIterObjCol = (CObjCol*)((*iter)->GetComponent(L"ObjCol"));
		vIterPos = pIterObjCol->GetPos();
		fIterRadius = pIterObjCol->GetRadius();
		matIterWorld = pIterObjCol->GetWorldMatrix();

		if (!CheckColSphere(vMyPos, fMyRadius, vIterPos, fIterRadius)
			|| !CheckColRect_OBB(matMyWorld, matIterWorld))
			continue;


		if (_ppTargetOut != NULL)
			*_ppTargetOut = (*iter);
		return true;
	}

	_ppTargetOut = NULL;
	return false;
}

bool CColMgr::CheckColObjToObj(const CObjCol* _pMyObjCol,
								const wstring& _wtrTargetObjKey,
								CObj** _ppTargetOut /*= NULL*/)
{
	ListObj::iterator iter = ((*m_pMapObjList_GameLogic)[_wtrTargetObjKey]).begin();
	ListObj::iterator iter_End = ((*m_pMapObjList_GameLogic)[_wtrTargetObjKey]).end();

	float fMyRadius, fIterRadius;
	D3DXVECTOR3 vMyPos, vIterPos;
	D3DXMATRIX matMyWorld, matIterWorld;
	CObjCol* pIterObjCol = NULL;

	vMyPos = _pMyObjCol->GetPos();
	fMyRadius = _pMyObjCol->GetRadius();
	matMyWorld = _pMyObjCol->GetWorldMatrix();


	for (; iter != iter_End; ++iter)
	{
		if ((*iter)->GetObjState() != CObj::OBJ_STATE_ALIVE)
			continue;

		pIterObjCol = (CObjCol*)((*iter)->GetComponent(L"ObjCol"));
		vIterPos = pIterObjCol->GetPos();
		fIterRadius = pIterObjCol->GetRadius();
		matIterWorld = pIterObjCol->GetWorldMatrix();

		if (!CheckColSphere(vMyPos, fMyRadius, vIterPos, fIterRadius)
			|| !CheckColRect_OBB(matMyWorld, matIterWorld))
			continue;


		if (_ppTargetOut != NULL)
			*_ppTargetOut = (*iter);
		return true;
	}

	_ppTargetOut = NULL;
	return false;
}

bool CColMgr::CheckColObjToObj(const ColTarget& _eColTarget,
								const CObjCol* _pMyObjCol,
								CObj** _ppTargetOut /*= NULL*/)
{
	ListObj::iterator iter;
	ListObj::iterator iter_End;


	switch (_eColTarget)
	{
	case COL_TARGET_PROGRESSRATEZONE :
		iter = m_listObj_ProgressRateZone.begin();
		iter_End = m_listObj_ProgressRateZone.end();
		break;

	case COL_TARGET_BOOSTERZONE :
		iter = m_listObj_BoosterZone.begin();
		iter_End = m_listObj_BoosterZone.end();
		break;

	case COL_TARGET_ENEMY :
		iter = m_listObj_Enemy.begin();
		iter_End = m_listObj_Enemy.end();
		break;

	default:
		break;
	}

	

	float fMyRadius, fIterRadius;
	D3DXVECTOR3 vMyPos, vIterPos;
	D3DXMATRIX matMyWorld, matIterWorld;
	CObjCol* pIterObjCol = NULL;

	vMyPos = _pMyObjCol->GetPos();
	fMyRadius = _pMyObjCol->GetRadius();
	matMyWorld = _pMyObjCol->GetWorldMatrix();


	for (; iter != iter_End; ++iter)
	{
		if ((*iter)->GetObjState() != CObj::OBJ_STATE_ALIVE)
			continue;

		pIterObjCol = (CObjCol*)((*iter)->GetComponent(L"ObjCol"));
		vIterPos = pIterObjCol->GetPos();
		fIterRadius = pIterObjCol->GetRadius();
		matIterWorld = pIterObjCol->GetWorldMatrix();

		if (!CheckColSphere(vMyPos, fMyRadius, vIterPos, fIterRadius)
			|| !CheckColRect_OBB(matMyWorld, matIterWorld))
			continue;
		

		if (_ppTargetOut != NULL)
			*_ppTargetOut = (*iter);
		return true;
	}

	_ppTargetOut = NULL;
	return false;
}

bool CColMgr::CheckColSphere(const D3DXVECTOR3& _vDestPos,
							const float& _vDestRadius,
							const D3DXVECTOR3& _vSourPos,
							const float& _vSourRadius)
{
	float fDistance = D3DXVec3Length(&(_vDestPos - _vSourPos));

	if (fDistance < _vDestRadius + _vSourRadius)
		return true;

	return false;
}

bool CColMgr::CheckColRect_AABB(const D3DXVECTOR3& _vDestScale,
								const D3DXVECTOR3& _vSourScale,
								const float& _fDistanceX,
								const float& _fDistanceY, 
								const float& _fDistanceZ)
{
	if ((_fDistanceX < _vDestScale.x + _vSourScale.x)
		&& (_fDistanceY < _vDestScale.y + _vSourScale.y)
		&& (_fDistanceZ < _vDestScale.z + _vSourScale.z))
	{
		return true;
	}

	return false;
}

bool CColMgr::CheckColRect_AABB(const D3DXVECTOR3& _vDestPos,
								const D3DXVECTOR3& _vDestScale,
								const D3DXVECTOR3& _vSourPos,
								const D3DXVECTOR3& _vSourScale)
{
	float fMin = 0.f;
	float fMax = 0.f;

	fMin = max(_vDestPos.x - _vDestScale.x, _vSourPos.x - _vSourScale.x);
	fMax = min(_vDestPos.x + _vDestScale.x, _vSourPos.x + _vSourScale.x);

	if (fMin > fMax)
		return false;


	fMin = max(_vDestPos.y - _vDestScale.y, _vSourPos.y - _vSourScale.y);
	fMax = min(_vDestPos.y + _vDestScale.y, _vSourPos.y + _vSourScale.y);

	if (fMin > fMax)
		return false;


	fMin = max(_vDestPos.z - _vDestScale.z, _vSourPos.z - _vSourScale.z);
	fMax = min(_vDestPos.z + _vDestScale.z, _vSourPos.z + _vSourScale.z);

	if (fMin > fMax)
		return false;

	return true;
}

//얘가 obb충돌 체크하는곳
bool CColMgr::CheckColRect_OBB(	const D3DXMATRIX& _matDestWorld,
								const D3DXMATRIX& _matSourWorld,
								D3DXVECTOR3* _vDistanceOut /*= NULL*/)
{
	ZeroMemory(m_tOBB, sizeof(OBB)* PAIR);

	if (_vDistanceOut != NULL)
		*_vDistanceOut = D3DXVECTOR3(0.f, 0.f, 0.f);

	Init_OBB_Point(&m_tOBB[0]);
	Init_OBB_Point(&m_tOBB[1]);

	for (int i = 0; i < HEXAHEDRON_POINT_NUM; ++i)
	{
		D3DXVec3TransformCoord(
			&m_tOBB[0].m_vPoint[i], &m_tOBB[0].m_vPoint[i], &_matDestWorld);
		D3DXVec3TransformCoord(
			&m_tOBB[1].m_vPoint[i], &m_tOBB[1].m_vPoint[i], &_matSourWorld);
	}
	D3DXVec3TransformCoord(
		&m_tOBB[0].m_vCenterPos, &m_tOBB[0].m_vCenterPos, &_matDestWorld);
	D3DXVec3TransformCoord(
		&m_tOBB[1].m_vCenterPos, &m_tOBB[1].m_vCenterPos, &_matSourWorld);

	Init_OBB_Axis(&m_tOBB[0]);
	Init_OBB_Axis(&m_tOBB[1]);


	float	fDistance[HEXAHEDRON_AXIS_NUM];
	D3DXVECTOR3 vDistanceForOut(0.f, 0.f, 0.f);

	for (int i = 0; i < PAIR; ++i)
	{
		for (int j = 0; j < HEXAHEDRON_AXIS_NUM; ++j)
		{
			fDistance[0] = fabs(D3DXVec3Dot(&m_tOBB[0].m_vProjAxis[0], &m_tOBB[i].m_vAxis[j]))
				+ fabs(D3DXVec3Dot(&m_tOBB[0].m_vProjAxis[1], &m_tOBB[i].m_vAxis[j]))
				+ fabs(D3DXVec3Dot(&m_tOBB[0].m_vProjAxis[2], &m_tOBB[i].m_vAxis[j]));

			fDistance[1] = fabs(D3DXVec3Dot(&m_tOBB[1].m_vProjAxis[0], &m_tOBB[i].m_vAxis[j]))
				+ fabs(D3DXVec3Dot(&m_tOBB[1].m_vProjAxis[1], &m_tOBB[i].m_vAxis[j]))
				+ fabs(D3DXVec3Dot(&m_tOBB[1].m_vProjAxis[2], &m_tOBB[i].m_vAxis[j]));

			D3DXVECTOR3		vTemp = m_tOBB[1].m_vCenterPos - m_tOBB[0].m_vCenterPos;
			fDistance[2] = fabs(D3DXVec3Dot(&vTemp, &m_tOBB[i].m_vAxis[j]));

			if (fDistance[2] > fDistance[1] + fDistance[0])
			{
				if (_vDistanceOut != NULL)
					_vDistanceOut = NULL;
				return false;
			}
		}
	}

	if (_vDistanceOut == NULL)
		_vDistanceOut = NULL;

	return true;
}

void CColMgr::Init_OBB_Point(OBB* _pOBB)
{
	_pOBB->m_vPoint[0] = D3DXVECTOR3(-1.f, 1.f, -1.f);
	_pOBB->m_vPoint[1] = D3DXVECTOR3(1.f, 1.f, -1.f);
	_pOBB->m_vPoint[2] = D3DXVECTOR3(1.f, -1.f, -1.f);
	_pOBB->m_vPoint[3] = D3DXVECTOR3(-1.f, -1.f, -1.f);
		   
	_pOBB->m_vPoint[4] = D3DXVECTOR3(-1.f, 1.f, 1.f);
	_pOBB->m_vPoint[5] = D3DXVECTOR3(1.f, 1.f, 1.f);
	_pOBB->m_vPoint[6] = D3DXVECTOR3(1.f, -1.f, 1.f);
	_pOBB->m_vPoint[7] = D3DXVECTOR3(-1.f, -1.f, 1.f);

	_pOBB->m_vCenterPos = D3DXVECTOR3(0.f, 0.f, 0.f);;
}

void CColMgr::Init_OBB_Axis(OBB* _pOBB)
{
	// x
	_pOBB->m_vProjAxis[0]
		= (_pOBB->m_vPoint[1] + _pOBB->m_vPoint[2] + _pOBB->m_vPoint[5] + _pOBB->m_vPoint[6])
		* 0.25f - _pOBB->m_vCenterPos;
	_pOBB->m_vAxis[0] = _pOBB->m_vPoint[1] - _pOBB->m_vPoint[0];

	//y
	_pOBB->m_vProjAxis[1]
		= (_pOBB->m_vPoint[0] + _pOBB->m_vPoint[1] + _pOBB->m_vPoint[4] + _pOBB->m_vPoint[5])
		* 0.25f - _pOBB->m_vCenterPos;
	_pOBB->m_vAxis[1] = _pOBB->m_vPoint[0] - _pOBB->m_vPoint[3];

	//z
	_pOBB->m_vProjAxis[2]
		= (_pOBB->m_vPoint[4] + _pOBB->m_vPoint[5] + _pOBB->m_vPoint[6] + _pOBB->m_vPoint[7])
		* 0.25f - _pOBB->m_vCenterPos;
	_pOBB->m_vAxis[2] = _pOBB->m_vPoint[7] - _pOBB->m_vPoint[3];


	for (int i = 0; i < HEXAHEDRON_AXIS_NUM; ++i)
		D3DXVec3Normalize(&_pOBB->m_vAxis[i], &_pOBB->m_vAxis[i]);
}

void CColMgr::Release()
{
	list<DiviSpace*>::iterator iter = m_listDiviSpace.begin();
	list<DiviSpace*>::iterator iter_end = m_listDiviSpace.end();

	for (; iter != iter_end; ++iter)
		::Safe_Delete(*iter);

	m_listDiviSpace.clear();
}