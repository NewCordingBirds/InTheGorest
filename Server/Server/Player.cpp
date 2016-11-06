#include "Player.h"

CPlayer::CPlayer():
x(4.9f),
y(60.5f),
z(275.0f)
{
	for (int i = 0; i < ROOM_MAXUSER; ++i){
		InitializeCriticalSection(&client[i].cs);
	}
}
CPlayer::~CPlayer(){
	for (int i = 0; i < ROOM_MAXUSER; ++i)
		DeleteCriticalSection(&client[i].cs);
}

SC_InitPlayer CPlayer::PlayerAccept(int id){
	SC_InitPlayer packet;

	packet.ID = id;
	packet.type = SC_INITPLAYER;
	packet.size = sizeof(SC_PlayerPos);
	packet.playerstate = m_state;
	packet.direction = { 0, 0, 1 };
	packet.move = { x + (id * 10), y, z };
	packet.speed = 0;
	packet.accel = 100.0f;
	
	usernum++;
	
	return packet;
}
D3DXVECTOR3 CPlayer::PlayerPos(DWORD packet, int id){


	//EnterCriticalSection(&client[id].cs);
	client[id].frametime = timer[id].FramePerSec();
	//client[id].frametime = 0.01f;	
	//LeaveCriticalSection(&client[id].cs);
	
	if (client[id].isDie){
		D3DXVECTOR3 diedir(0, 1, 0);
		client[id].speed = 100.f;
		client[id].position -= diedir * client[id].speed * client[id].frametime;
		Pl = client[id].position;
		return Pl;
	}
	if (noneTrigger == client[id].trigger || ForwordTrigger == client[id].trigger ||
		BackwordTrigger == client[id].trigger || FDecelTrigger == client[id].trigger ||
		BDecelTrigger == client[id].trigger){
		//포워드--------------------------------------------
		if (KEY_UP & packet){
			client[id].trigger = ForwordTrigger;
			if (BDecelTrigger == client[id].trigger){
				client[id].speed = 0;							// 수정 꼭
				client[id].presstime = 0;						// 수정 꼭
			}
			EnterCriticalSection(&client[id].cs);
			BasicAccel(KEY_UP, client[id].frametime, id);			// 수정
			LeaveCriticalSection(&client[id].cs);
		}
		// 위 방향키 눌렀다 떼기
		if (UPKEY_UP & packet){
			client[id].trigger = FDecelTrigger;
		}
		if (client[id].trigger == FDecelTrigger){
			if (client[id].speed <= 0){
				client[id].trigger = noneTrigger;
			}
			EnterCriticalSection(&client[id].cs);
			BasicDecel(UPKEY_UP, client[id].frametime, id);
			LeaveCriticalSection(&client[id].cs);
		}
		//백워드--------------------------------------------

		if (KEY_DOWN & packet){
			client[id].trigger = BackwordTrigger;
			if (FDecelTrigger == client[id].trigger)
			{
				client[id].speed = 0;							// 수정 꼭
				client[id].presstime = 0;						// 수정 꼭
			}
			EnterCriticalSection(&client[id].cs);
			BasicAccel(KEY_DOWN, client[id].frametime, id);		// 수정
			LeaveCriticalSection(&client[id].cs);
		}
		//아래 방향키 눌렀다 뗌
		if (UPKEY_DOWN & packet){
			//client[id].presstime = 1.2;
			client[id].trigger = BDecelTrigger;
		}
		if (BDecelTrigger == client[id].trigger){
			if (client[id].speed <= 0){
				client[id].trigger = noneTrigger;
			}
			EnterCriticalSection(&client[id].cs);
			BasicDecel(UPKEY_DOWN, client[id].frametime, id);
			LeaveCriticalSection(&client[id].cs);
		}
	}
	
	SC_AvoidPlayer avoidplayer;
	//Avoid------------------------------------------------
	if (AVOIDKEY_LEFT & packet){
		client[id].avoidspeed = 200.0f;
		client[id].accel = 150.0f;
		client[id].trigger = LAvoidTrigger;
		//printf("im in avoidtrig");
	}

	if (AVOIDKEY_RIGHT & packet){
		client[id].avoidspeed = 200.0f;
		client[id].accel = 150.0f;
		client[id].trigger = RAvoidTrigger;
		//printf("im in avoidtrig");
	}
	if (client[id].trigger == LAvoidTrigger){
		if (client[id].avoidspeed < 0){
			client[id].accel = 50.0f;
			client[id].presstime = 0;
			client[id].trigger = noneTrigger;
			avoid = true;
		}

		D3DXVECTOR3 vRight;
		D3DXVec3Cross(&vRight, &D3DXVECTOR3(0.f, 1.f, 0.f), &client[id].direction);
		
		//EnterCriticalSection(&client[id].cs);
		if (client[id].avoidspeed > 0.0f){
			client[id].avoidspeed -= client[id].accel * client[id].frametime * client[id].presstime;
			client[id].presstime += 0.11f;
		//LeaveCriticalSection(&client[id].cs);
		}
		//EnterCriticalSection(&client[id].cs);
		client[id].position -= vRight * client[id].avoidspeed * client[id].frametime;
		//LeaveCriticalSection(&client[id].cs);
	}

	if (client[id].trigger == RAvoidTrigger){
		if (client[id].avoidspeed < 0){
			client[id].accel = 50.0f;
			client[id].presstime = 0;
			client[id].trigger = noneTrigger;

			avoid = true;
		}

		D3DXVECTOR3 vRight;
		D3DXVec3Cross(&vRight, &D3DXVECTOR3(0.f, 1.f, 0.f), &client[id].direction);
		EnterCriticalSection(&client[id].cs);
		if (client[id].avoidspeed > 0.0f){
			client[id].avoidspeed -= client[id].accel * client[id].frametime * client[id].presstime;
			client[id].presstime += .11f;
		LeaveCriticalSection(&client[id].cs);
		}else
			LeaveCriticalSection(&client[id].cs);

		//EnterCriticalSection(&client[id].cs);
		client[id].position += vRight * client[id].avoidspeed * client[id].frametime;
		//LeaveCriticalSection(&client[id].cs);
		//printf("player X pos = %f, speed = %f", client[id].position.x, client[id].speed);
	}

	if (client[id].trigger == FcolTrigger || client[id].trigger == BcolTrigger)
		CollWall(id);

	//m_time = m_frametime;
	Pl= client[id].position;
	//printf("%d : %.2f, %.2f, %.2f \n",
	//	id, client[id].frametime, client[id].speed, client[id].presstime);
	return Pl;
}
void CPlayer::BasicAccel(int keyTrigger, float _fTime, int id){	// 수정 
	if (client[id].speed < 150.0f){
		client[id].speed = client[id].presstime * client[id].accel;
		client[id].presstime += 0.01f;
	}if (KEY_UP == keyTrigger){
		client[id].position += client[id].direction * client[id].speed * _fTime;
	}
	if (KEY_DOWN == keyTrigger){
		client[id].position -= client[id].direction * client[id].speed * _fTime;
	}

}
void CPlayer::BasicDecel(int keyTrigger, float _fTime, int id){

	if (client[id].speed > 0.0f){
		client[id].speed = client[id].presstime * client[id].accel;
		client[id].presstime -= 0.01f;
	}if (UPKEY_UP == keyTrigger){
		client[id].position += client[id].direction * client[id].speed * _fTime;
	}
	if (UPKEY_DOWN == keyTrigger){
		client[id].position -= client[id].direction * client[id].speed * _fTime;
	}
}

void CPlayer::CollWall(int id){
	//client[id].frametime = CTimer::FramePerSec();
	
	//client[id].trigger = colTrigger;

	if (client[id].presstime <= 0){
		client[id].trigger = noneTrigger;
		client[id].presstime = 0;
		client[id].boostertimming = false;
		client[id].isDie = false;
	}
	if (client[id].trigger == FcolTrigger){
		if (client[id].speed > 0.0f){
			client[id].speed = client[id].presstime * client[id].accel;
			client[id].presstime -= 0.01f;
		}
		client[id].position -= client[id].direction * client[id].speed * client[id].frametime;
	}

	if (client[id].trigger == BcolTrigger){
		if (client[id].speed > 0.0f){
			client[id].speed = client[id].presstime * client[id].accel;
			client[id].presstime -= 0.01f;
		}
		client[id].position += client[id].direction * client[id].speed * client[id].frametime;
	}

	//return client[id].position;
}

D3DXVECTOR3 CPlayer::UseBooster(int id){
	//D3DXVECTOR3 pos;
	client[id].frametime = CTimer::FramePerSec();
	if (client[id].trigger != boosterTrigger){
		client[id].presstime = 0;
		client[id].accel = 70.0f;
		client[id].trigger = boosterTrigger;
	}

	if (client[id].trigger == boosterTrigger){
		if (client[id].boostertime > 1.7f){
			client[id].accel = 50.f;
			client[id].speed = 150.0f;
			client[id].presstime = 0.0f;
			client[id].boostertime = 0.0f;
			client[id].boostertimming = false;
			client[id].trigger = noneTrigger;
			//printf("Endbooster");
		}

		if (client[id].boostertime < 1.0f){
			if (client[id].speed < 200.f){
				client[id].speed = client[id].accel * client[id].presstime;
				client[id].presstime += 0.01f;
			}
		}
		else{
			client[id].speed = client[id].accel * client[id].presstime;
			if (client[id].presstime > 0)
				client[id].presstime -= 0.01f;
		}

		client[id].position += client[id].direction * client[id].speed * client[id].frametime;
		client[id].boostertime += client[id].frametime;
		//printf("Speed = %f\n", client[id].boostertime);
	}
	return client[id].position;
}
D3DXVECTOR3 CPlayer::BoosterColl(int id, D3DXVECTOR3 direction){

	client[id].frametime = CTimer::FramePerSec();
	if (client[id].trigger != boosterTrigger){
		client[id].presstime = 0;
		client[id].accel = 1.0f;
		client[id].trigger = boosterTrigger;
	}

	if (client[id].trigger == boosterTrigger){
		if (client[id].boostertime > 0.17f){
			client[id].accel = 50.f;
			client[id].speed = 150.0f;
			client[id].presstime = 0.0f;
			client[id].boostertime = 0.0f;
			client[id].boostertimming = false;
			client[id].trigger = noneTrigger;
		}

		if (client[id].boostertime < 0.10f){
			if (client[id].speed < 200.f){
				client[id].speed = client[id].accel * client[id].presstime;
				client[id].presstime += 0.01f;
			}
		}
		else{
			client[id].speed = client[id].accel * client[id].presstime;
			if (client[id].presstime > 0)
				client[id].presstime -= 0.01f;
		}

		client[id].position += direction * client[id].speed * client[id].frametime;
		client[id].boostertime += client[id].frametime;
		//printf("Speed = %f\n", client[id].boostertime);
	}
	return client[id].position;
}