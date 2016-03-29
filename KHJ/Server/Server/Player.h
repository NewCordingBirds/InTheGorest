
#include "Protocol.h"



class CPlayer{


public:	

	CPlayer();
	~CPlayer();

	static PLAYER players[8];					//������ ��

	enum state{
		normal	= 1,
		dizzy	= 2,
		booster = 3,
	};
	static float x;
	static float y;
	static float z;

	int HP;
	int state;
	float speed;
	
	int m_room;									// �� ��ȣ

	static SC_Player PlayerAccept(int id, SC_Player packet);
//	int InitPlayer ();					// ���� ���� �� �÷��̾�
	static SC_Player PlayerPos(char packet, SC_Player Pl);				// �÷��̾� ��ġ
	SC_Player PlayerAvoid(SC_Player packet, int id);		// �÷��̾� ���� ���� �� ����

	float RotateX (float rotate);			// �¿�ȸ��
	float RotateY (float rotate);			// ����ȸ��

	void UseItem ();						// ������ ���
	void UetItem ();						// ������ ȹ��

	void Shoot (int ID);					// �� ���
	int PlayerHP (int HP);					// HP����
};