/*!
 * \file ColMgr.h
 * \date 2016/02/16 4:26
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

#ifndef ColMgr_h__
#define ColMgr_h__


#include "Obj.h"
#include "ObjCol.h"

#include "Layer.h"

const D3DXVECTOR3	DIVITIONSPACE_SIZE_HALF		= D3DXVECTOR3(500.f, 500.f, 500.f);
const D3DXVECTOR3	DIVITIONSPACE_GAB			= D3DXVECTOR3(370.f, 370.f, 370.f);
const float			DIVITIONSPACE_RADIUS		= MakeDiagonalLine(DIVITIONSPACE_SIZE_HALF.x, DIVITIONSPACE_SIZE_HALF.y);


struct DiviSpace
{
	D3DXVECTOR3		vPos;
	list<CObjCol*>	listUnMovableZoneObjCol;
	ListObj			listItemObj;

	DiviSpace() {};
	~DiviSpace() { listUnMovableZoneObjCol.clear(); }
};

struct OBB
{
	D3DXVECTOR3		m_vPoint[HEXAHEDRON_POINT_NUM];
	D3DXVECTOR3		m_vCenterPos;
	D3DXVECTOR3		m_vAxis[HEXAHEDRON_AXIS_NUM];
	D3DXVECTOR3		m_vProjAxis[HEXAHEDRON_AXIS_NUM];
};

class CColMgr
{
	DECLARE_SINGLETON(CColMgr)

public :
	static enum ColTarget
	{
		COL_TARGET_PROGRESSRATEZONE,
		COL_TARGET_BOOSTERZONE,
		COL_TARGET_ENEMY
	};

private :
	map<wstring, ListObj>*		m_pMapObjList_GameLogic;
	list<DiviSpace*>			m_listDiviSpace;

	ListObj			m_listObj_ProgressRateZone;
	ListObj			m_listObj_BoosterZone;
	ListObj			m_listObj_Item;
	ListObj			m_listObj_Enemy;

public :
	void SetMapObjList(CLayer::LayerType _eLayerType, map<wstring, ListObj>* _pMapObjList);

private:
	void		MakeListDiviSpace();

public:
	static bool SortObjColPosX(CObj* _pDest, CObj* _pSour)
	{
		return ((CObjCol*)(_pDest->GetComponent(L"ObjCol")))->GetPos().x
			< ((CObjCol*)(_pSour->GetComponent(L"ObjCol")))->GetPos().x;
	}
	static bool SortObjColPosY(CObj* _pDest, CObj* _pSour)
	{
		return ((CObjCol*)(_pDest->GetComponent(L"ObjCol")))->GetPos().y
			< ((CObjCol*)(_pSour->GetComponent(L"ObjCol")))->GetPos().y;
	}
	static bool SortObjColPosZ(CObj* _pDest, CObj* _pSour)
	{
		return ((CObjCol*)(_pDest->GetComponent(L"ObjCol")))->GetPos().z
			< ((CObjCol*)(_pSour->GetComponent(L"ObjCol")))->GetPos().z;
	}


public :
	bool CheckColObjToUnmovable(const CObjCol* _pMyObjCol, D3DXVECTOR3** _ppDirOut = NULL);
	bool CheckColObjToItem(const CObjCol* _pMyObjCol, CObj** _ppTargetOut = NULL);

	bool CheckColObjToObj(const CObjCol* _pMyObjCol, const wstring& _wtrTargetObjKey, CObj** _ppTargetOut = NULL);
	bool CheckColObjToObj(const ColTarget& _eColTarget, const CObjCol* _pMyObjCol, CObj** _ppTargetOut = NULL);


private :
	bool CheckColObjToUnmovableInDiviSpace(DiviSpace* _pDiviSpace,
											const CObjCol* _pMyObjCol,
											D3DXVECTOR3** _ppDirOut = NULL);
	bool CheckColObjToItemInDiviSpace(DiviSpace* _pDiviSpace,
										const CObjCol* _pMyObjCol,
										CObj** _ppTargetOut = NULL);

private :
	bool CheckColSphere(const D3DXVECTOR3& _vDestPos, const float& _vDestRadius,
						const D3DXVECTOR3& _vSourPos, const float& _vSourRadius );

	bool CheckColRect_AABB(const D3DXVECTOR3& _vSourPos, const D3DXVECTOR3& _vSourScale,
							const float& _fDistanceX, const float& _fDistanceY, const float& _fDistanceZ);
	bool CheckColRect_AABB(const D3DXVECTOR3& _vDestPos, const D3DXVECTOR3& _vDestScale,
							const D3DXVECTOR3& _vSourPos, const D3DXVECTOR3& _vSourScale);

	bool CheckColRect_OBB(const D3DXMATRIX& _matDestWorld, const D3DXMATRIX& _matSourWorld,
							D3DXVECTOR3* _vDistanceOut = NULL);


private :
	OBB		m_tOBB[PAIR];

private :
	void Init_OBB_Point(OBB* _pOBB);
	void Init_OBB_Axis(OBB* _pOBB);


public :
	void Release();

private: 
	CColMgr();
	~CColMgr();
};

#endif // ColMgr_h__
