#include "Player.h"

// 이동키값
#define KEY_UP					1
#define KEY_DOWN				2
#define KEY_RIGHT				3
#define KEY_LEFT				4
//케
#define STATE_NORMAL			1
#define STATE_DIZZY				2
#define STATE_BOOSTER			3

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
	put_PlayerPacket.x = 0;
	put_PlayerPacket.y = 0;
	put_PlayerPacket.z = 0;

	return put_PlayerPacket;
}

SC_Player CPlayer::PlayerPos(char packet, SC_Player Pl){
	if (packet == KEY_UP){
		Pl.y++;
	}
	else if (packet == KEY_DOWN){
		Pl.y--;
	}
	else if (packet == KEY_RIGHT){
		Pl.x++;
	}
	else if (packet == KEY_LEFT){
		Pl.x++;
	} 
	return Pl;
}
