#include "Item.h"

//CItem *CItem::m_GetInstance = NULL;
map<int, ITEM> CItem::item;

CItem::CItem(){

}
CItem::~CItem(){

}

void CItem::SetItem(){
	for (int i = 0; i < ITEM_NUM; ++i){
		item[i].id = i;
		item[i].in_use = true;
	}
}

SC_Item CItem::AvoidItem(int id){
	SC_Item itempacket;

	itempacket.itemid = id;
	itempacket.isExist = false;
	itempacket.size = sizeof(SC_Item);
	itempacket.type = SC_ITEMEXIST;

	item[id].in_use = false;
	return itempacket;
}
