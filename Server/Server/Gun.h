#include "Protocol.h"

class CGun{
private:
	CGun();
	~CGun();

public:

	int m_ID;

	map <int, 

	D3DXVECTOR3 ShootAMMO(int, D3DXVECTOR3);
	void RemoveAMMO();							//일정 거리 이상 가면 사라짐 
	static CGun& getInstance() {
		static CGun uniqueInstance;
		return uniqueInstance;
	}
};