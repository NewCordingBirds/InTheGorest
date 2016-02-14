
#include "Protocol.h"



class CPlayer{

public:	
	
	CPlayer();
	~CPlayer();

	PLAYER players[8];					//������ ��

	enum state{
		normal	= 1,
		dizzy	= 2,
		booster = 3,
	};
	float x;
	float y;
	float z;

	int HP;
	int state;

	int m_room;									// �� ��ȣ

	SC_Player PlayerAccept(int id, SC_Player *packet);
//	int InitPlayer ();					// ���� ���� �� �÷��̾�
	SC_Player PlayerPos(char packet, SC_Player Pl);				// �÷��̾� ��ġ
	int ChangePlayerState (int state);						// ����

	float RotateX (float rotate);			// �¿�ȸ��
	float RotateY (float rotate);			// ����ȸ��

	void UseItem ();						// ������ ���
	void UetItem ();						// ������ ȹ��

	void Shoot (int ID);					// �� ���
	int PlayerHP (int HP);					// HP����
};