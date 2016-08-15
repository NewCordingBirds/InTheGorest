#include "Gun.h"

CGun::CGun(){}

CGun::~CGun(){}

D3DXVECTOR3 CGun::ShootAMMO(int id, D3DXVECTOR3 direction){
	
	m_pPhysics->m_vPos += m_pPhysics->m_fSpeed * m_pPhysics->m_vDir * CTimer::FramePerSec();

	return pos;
}
void CGun::RemoveAMMO(){
	
}
