#include "Player.h"



CPlayer::CPlayer(){

}
CPlayer::~CPlayer(){}


SC_Player CPlayer::PlayerAccept(int id, SC_Player packet){

	//packet = put_PlayerPacket;

	packet.ID = id;
	packet.type = SC_PLAYER;
	packet.size = sizeof(packet);
	packet.rotate_x = 0;
	packet.rotate_y = 0;
	packet.state = 0;
	packet.x = 40 + (id* 30);
	packet.y = 40;
	packet.z = 0;

	return packet;
}

SC_Player CPlayer::PlayerPos(char packet, SC_Player Pl){
	if (packet == KEY_UP){
		Pl.y  -= 2;
	}
	else if (packet == KEY_DOWN){
		Pl.y += 2;
	}
	else if (packet == KEY_RIGHT){
		Pl.x += 2;
	}
	else if (packet == KEY_LEFT){
		Pl.x -= 2;
	} 
	return Pl;
}


