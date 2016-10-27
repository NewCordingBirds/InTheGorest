#include "Gun.h"

CGun::CGun():
m_speed(500.f)
{}

CGun::~CGun(){}

D3DXVECTOR3 CGun::ShootAMMO(int id, D3DXVECTOR3 direction){
	
	bullet[id].second.pos += m_speed * direction * CTimer::FramePerSec();

	return bullet[id].second.pos;
}
void CGun::RemoveAMMO(){
	
}
