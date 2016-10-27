#include "D3DX10math.h"
#ifndef __COLLISIONDETECTION_HPP
#define __COLLISIONDETECTION_HPP

typedef	float real;

class CCollisionBox
{
public:
	real center[3];
	real axis[3][3];		//transformation matrix
	real extent[3];			//half full size-eqivalent to radius not diameter
	real translation[3];	// translation vector
	void GetTransform(real t[16])
	{
		t[0] = axis[0][0];
		t[1] = axis[0][1];
		t[2] = axis[0][2];
		t[3] = ((real)0.0);
		t[4] = axis[1][0];
		t[5] = axis[1][1];
		t[6] = axis[1][2];
		t[7] = ((real)0.0);
		t[8] = axis[2][0];
		t[9] = axis[2][1];
		t[10] = axis[2][2];
		t[11] = ((real)0.0);
		t[12] = translation[0];
		t[13] = translation[1];
		t[14] = translation[2];
		t[15] = ((real)1.0);
	}
};


inline real DotProduct(const real v0[3], const real v1[3])
{//=v0.v1
	return v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2];
}
#define fc_fabs fabsf

#endif//of #ifndef __COLLISIONDETECTION_HPP
