#include "Protocol.h"
#include "Map.h"
#include "Item.h"
#include "Gun.h"
#include "Player.h"

class CColision{
public :
	CColision();
	~CColision();

public:
	CMap*			m_map;
	CItem*			m_item;
	CGun*			m_gun;
	CPlayer*		m_player;

public :
	void CheckColl(PLAYER, COLLOBJ);
};