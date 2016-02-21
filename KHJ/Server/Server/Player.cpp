#include "Player.h"



CPlayer::CPlayer(){

}
CPlayer::~CPlayer(){}


SC_Player CPlayer::PlayerAccept(int id, SC_Player *packet){
	SC_Player put_PlayerPacket;

	//packet = put_PlayerPacket;

	put_PlayerPacket.ID = id;
	put_PlayerPacket.type = SC_PLAYER;
	put_PlayerPacket.size = sizeof(put_PlayerPacket);
	put_PlayerPacket.rotate_x = 0;
	put_PlayerPacket.rotate_y = 0;
	put_PlayerPacket.state = 0;
	put_PlayerPacket.x = 40;
	put_PlayerPacket.y = 40;
	put_PlayerPacket.z = 0;

	return put_PlayerPacket;
}

SC_Player CPlayer::PlayerPos(char packet, SC_Player Pl){
	if (packet == KEY_UP){
		Pl.y  += 2;
	}
	else if (packet == KEY_DOWN){
		Pl.y -= 2;
	}
	else if (packet == KEY_RIGHT){
		Pl.x += 2;
	}
	else if (packet == KEY_LEFT){
		Pl.x -= 2;
	} 
	return Pl;
}


