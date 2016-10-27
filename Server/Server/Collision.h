#include "CollisionBox.h"


class Collision{
public:
	Collision();
	~Collision();

public:

	bool BoxBoxIntersection(const CCollisionBox& box0, const CCollisionBox& box1);

	D3DXMATRIX* GetBoxTransform(D3DXMATRIX *pMat, CCollisionBox* pBox);
	void SetBoxTransform(const D3DXMATRIX *pMat, CCollisionBox* pBox);
	void initBox(CCollisionBox *pBox, const D3DXVECTOR3& vecMin, const D3DXVECTOR3& vecMax);
	void moveBox(CCollisionBox *pBox, const D3DXMATRIX& mat);


	static Collision& getInstance() {
		static Collision uniqueInstance;
		return uniqueInstance;
	}
};