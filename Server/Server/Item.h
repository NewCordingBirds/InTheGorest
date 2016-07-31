
#include "Protocol.h"

class CItem{

public:
	static map<int, ITEM> item;

	static void SetItem();
	static SC_Item AvoidItem(int);
public:
	CItem();
	~CItem();

	//static CItem *m_GetInstance;

	//static CItem& getInstance()
	//{
	//	static CItem instance;
	//	return instance;
	//}

};