#include "Protocol.h"

class CGun{
private:
	CGun();
	~CGun();

public:

	int m_ID;
	float m_speed;

	vector <pair<int, BULLET>> bullet;


	D3DXVECTOR3 ShootAMMO(int, D3DXVECTOR3);
	void RemoveAMMO();							//���� �Ÿ� �̻� ���� ����� 
	static CGun& getInstance() {
		static CGun uniqueInstance;
		return uniqueInstance;
	}
};