#include <math.h>
#include "Collision.h"

Collision::Collision(){}
Collision::~Collision(){}

bool Collision::BoxBoxIntersection(const CCollisionBox& box0, const CCollisionBox& box1)
{
	// compute difference of box centers,D=C1-C0
	real D[3] =
	{
		box1.center[0] - box0.center[0],
		box1.center[1] - box0.center[1],
		box1.center[2] - box0.center[2]
	};

	real C[3][3];    //matrix C=A^T B,c_{ij}=Dot(A_i,B_j)
	real absC[3][3]; //|c_{ij}|
	real AD[3];      //Dot(A_i,D)
	real R0, R1, R;    //interval radii and distance between centers
	real R01;        //=R0+R1

	//A0
	C[0][0] = DotProduct(box0.axis[0], box1.axis[0]);
	C[0][1] = DotProduct(box0.axis[0], box1.axis[1]);
	C[0][2] = DotProduct(box0.axis[0], box1.axis[2]);
	AD[0] = DotProduct(box0.axis[0], D);
	absC[0][0] = (real)fc_fabs(C[0][0]);
	absC[0][1] = (real)fc_fabs(C[0][1]);
	absC[0][2] = (real)fc_fabs(C[0][2]);
	R = (real)fc_fabs(AD[0]);
	R1 = box1.extent[0] * absC[0][0] + box1.extent[1] * absC[0][1] + box1.extent[2] * absC[0][2];
	R01 = box0.extent[0] + R1;
	if (R > R01)return 0;

	//A1
	C[1][0] = DotProduct(box0.axis[1], box1.axis[0]);
	C[1][1] = DotProduct(box0.axis[1], box1.axis[1]);
	C[1][2] = DotProduct(box0.axis[1], box1.axis[2]);
	AD[1] = DotProduct(box0.axis[1], D);
	absC[1][0] = (real)fc_fabs(C[1][0]);
	absC[1][1] = (real)fc_fabs(C[1][1]);
	absC[1][2] = (real)fc_fabs(C[1][2]);
	R = (real)fc_fabs(AD[1]);
	R1 = box1.extent[0] * absC[1][0] + box1.extent[1] * absC[1][1] + box1.extent[2] * absC[1][2];
	R01 = box0.extent[1] + R1;
	if (R > R01)return 0;

	//A2
	C[2][0] = DotProduct(box0.axis[2], box1.axis[0]);
	C[2][1] = DotProduct(box0.axis[2], box1.axis[1]);
	C[2][2] = DotProduct(box0.axis[2], box1.axis[2]);
	AD[2] = DotProduct(box0.axis[2], D);
	absC[2][0] = (real)fc_fabs(C[2][0]);
	absC[2][1] = (real)fc_fabs(C[2][1]);
	absC[2][2] = (real)fc_fabs(C[2][2]);
	R = (real)fc_fabs(AD[2]);
	R1 = box1.extent[0] * absC[2][0] + box1.extent[1] * absC[2][1] + box1.extent[2] * absC[2][2];
	R01 = box0.extent[2] + R1;
	if (R > R01)return 0;

	//B0
	R = (real)fc_fabs(DotProduct(box1.axis[0], D));
	R0 = box0.extent[0] * absC[0][0] + box0.extent[1] * absC[1][0] + box0.extent[2] * absC[2][0];
	R01 = R0 + box1.extent[0];
	if (R > R01)return 0;

	//B1
	R = (real)fc_fabs(DotProduct(box1.axis[1], D));
	R0 = box0.extent[0] * absC[0][1] + box0.extent[1] * absC[1][1] + box0.extent[2] * absC[2][1];
	R01 = R0 + box1.extent[1];
	if (R > R01)return 0;

	//B2
	R = (real)fc_fabs(DotProduct(box1.axis[2], D));
	R0 = box0.extent[0] * absC[0][2] + box0.extent[1] * absC[1][2] + box0.extent[2] * absC[2][2];
	R01 = R0 + box1.extent[2];
	if (R > R01)return 0;

	//A0xB0
	R = (real)fc_fabs(AD[2] * C[1][0] - AD[1] * C[2][0]);
	R0 = box0.extent[1] * absC[2][0] + box0.extent[2] * absC[1][0];
	R1 = box1.extent[1] * absC[0][2] + box1.extent[2] * absC[0][1];
	R01 = R0 + R1;
	if (R > R01)return 0;

	//A0xB1
	R = (real)fc_fabs(AD[2] * C[1][1] - AD[1] * C[2][1]);
	R0 = box0.extent[1] * absC[2][1] + box0.extent[2] * absC[1][1];
	R1 = box1.extent[0] * absC[0][2] + box1.extent[2] * absC[0][0];
	R01 = R0 + R1;
	if (R > R01)return 0;

	//A0xB2
	R = (real)fc_fabs(AD[2] * C[1][2] - AD[1] * C[2][2]);
	R0 = box0.extent[1] * absC[2][2] + box0.extent[2] * absC[1][2];
	R1 = box1.extent[0] * absC[0][1] + box1.extent[1] * absC[0][0];
	R01 = R0 + R1;
	if (R > R01)return 0;

	//A1xB0
	R = (real)fc_fabs(AD[0] * C[2][0] - AD[2] * C[0][0]);
	R0 = box0.extent[0] * absC[2][0] + box0.extent[2] * absC[0][0];
	R1 = box1.extent[1] * absC[1][2] + box1.extent[2] * absC[1][1];
	R01 = R0 + R1;
	if (R > R01)return 0;

	//A1xB1
	R = (real)fc_fabs(AD[0] * C[2][1] - AD[2] * C[0][1]);
	R0 = box0.extent[0] * absC[2][1] + box0.extent[2] * absC[0][1];
	R1 = box1.extent[0] * absC[1][2] + box1.extent[2] * absC[1][0];
	R01 = R0 + R1;
	if (R > R01)return 0;

	//A1xB2
	R = (real)fc_fabs(AD[0] * C[2][2] - AD[2] * C[0][2]);
	R0 = box0.extent[0] * absC[2][2] + box0.extent[2] * absC[0][2];
	R1 = box1.extent[0] * absC[1][1] + box1.extent[1] * absC[1][0];
	R01 = R0 + R1;
	if (R > R01)return 0;

	//A2xB0
	R = (real)fc_fabs(AD[1] * C[0][0] - AD[0] * C[1][0]);
	R0 = box0.extent[0] * absC[1][0] + box0.extent[1] * absC[0][0];
	R1 = box1.extent[1] * absC[2][2] + box1.extent[2] * absC[2][1];
	R01 = R0 + R1;
	if (R > R01)return 0;

	//A2xB1
	R = (real)fc_fabs(AD[1] * C[0][1] - AD[0] * C[1][1]);
	R0 = box0.extent[0] * absC[1][1] + box0.extent[1] * absC[0][1];
	R1 = box1.extent[0] * absC[2][2] + box1.extent[2] * absC[2][0];
	R01 = R0 + R1;
	if (R > R01)return 0;

	//A2xB2
	R = (real)fc_fabs(AD[1] * C[0][2] - AD[0] * C[1][2]);
	R0 = box0.extent[0] * absC[1][2] + box0.extent[1] * absC[0][2];
	R1 = box1.extent[0] * absC[2][1] + box1.extent[1] * absC[2][0];
	R01 = R0 + R1;
	if (R > R01)return 0;

	return 1;
}
D3DXMATRIX* Collision::GetBoxTransform(D3DXMATRIX *pMat, CCollisionBox* pBox)
{
	int i, j;
	real fMat[16];
	pBox->GetTransform(fMat);
	for (i = 0; i < 4; ++i)
		for (j = 0; j < 4; ++j)
			(*pMat)(i, j) = fMat[i * 4 + j];
	return pMat;
}

void Collision::SetBoxTransform(const D3DXMATRIX* pMat, CCollisionBox* pBox)
{
	int i, j;
	for (i = 0; i < 3; ++i) {
		for (j = 0; j < 3; ++j)
			pBox->axis[i][j] = (*pMat)(i, j);
		pBox->translation[i] = (*pMat)(3, i);
	}
}

void Collision::initBox(CCollisionBox *pBox, const D3DXVECTOR3& vecMin, const D3DXVECTOR3& vecMax)
{
	pBox->center[0] = (vecMin.x + vecMax.x) / 2.0F;
	pBox->center[1] = (vecMin.y + vecMax.y) / 2.0F;
	pBox->center[2] = (vecMin.z + vecMax.z) / 2.0F;

	pBox->extent[0] = vecMax.x - pBox->center[0];
	pBox->extent[1] = vecMax.y - pBox->center[1];
	pBox->extent[2] = vecMax.z - pBox->center[2];
	// identity world coordinate axis
	pBox->axis[0][0] = 1.0F; pBox->axis[0][1] = 0.0F; pBox->axis[0][2] = 0.0F;
	pBox->axis[1][0] = 0.0F; pBox->axis[1][1] = 1.0F; pBox->axis[1][2] = 0.0F;
	pBox->axis[2][0] = 0.0F; pBox->axis[2][1] = 0.0F; pBox->axis[2][2] = 1.0F;
	pBox->translation[0] = 0.0F; pBox->translation[1] = 0.0F; pBox->translation[2] = 0.0F;
}

void Collision::moveBox(CCollisionBox *pBox, const D3DXMATRIX& mat)
{
	D3DXMATRIX matBox;
	// 박스의 transform을 가져온다.
	GetBoxTransform(&matBox, pBox);
	// 박스의 transform을 바꾼다.
	matBox *= mat;
	SetBoxTransform(&matBox, pBox);
	// 박스의 center 좌표도 바꾼다.
	D3DXVECTOR3 vecCenter(pBox->center[0], pBox->center[1], pBox->center[2]);
	D3DXVec3TransformCoord(&vecCenter, &vecCenter, &mat);
	pBox->center[0] = vecCenter.x; pBox->center[1] = vecCenter.y; pBox->center[2] = vecCenter.z;
}
