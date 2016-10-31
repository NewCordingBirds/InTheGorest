#include "stdafx.h"
#include "CameraObserver.h"


CCameraObserver::CCameraObserver()
: m_vEye(0.f, 0.f, 0.f)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

CCameraObserver::~CCameraObserver()
{

}

CCameraObserver* CCameraObserver::Create()
{
	CCameraObserver* pObserver = new CCameraObserver;
// 	pObserver->Update(CInfoSubject::DATA_TYPE_CAMERA_VIEW);
// 	pObserver->Update(CInfoSubject::DATA_TYPE_CAMERA_PROJECTION);
// 	pObserver->Update(CInfoSubject::DATA_TYPE_CAMERA_EYE);
// 	pObserver->Update(CInfoSubject::DATA_TYPE_CAMERA_ZOOM);

	return pObserver;
}

void CCameraObserver::Update(int _iMessage,
							UINT _nIdx /*= 0*/)
{
	vector<void*>* pData = m_pInfoSubject->GetDatalist(_iMessage);
	//NULL_CHECK(pData);
	if (pData == NULL)
		return;

	switch (_iMessage)
	{
	case CInfoSubject::DATA_TYPE_CAMERA_VIEW :
		m_matView = *((D3DXMATRIX*)((*pData)[_nIdx]));
		break;

	case CInfoSubject::DATA_TYPE_CAMERA_PROJECTION:
		m_matProj = *((D3DXMATRIX*)((*pData)[_nIdx]));
		break;

	case CInfoSubject::DATA_TYPE_CAMERA_EYE:
		m_vEye = *((D3DXVECTOR3*)((*pData)[_nIdx]));
		break;

	case CInfoSubject::DATA_TYPE_CAMERA_ZOOM :
		m_fZoom = *((float*)((*pData)[_nIdx]));

	default:
		break;
	}
}

