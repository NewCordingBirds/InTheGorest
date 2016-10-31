/*!
 * \file InfoSubject.h
 * \date 2016/01/11 6:10
 *
 * \author ±è¸íÁö
 * Contact: bluevill04@gmail.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#pragma once

#ifndef InfoSubject_h__
#define InfoSubject_h__

#include "Subject.h"
#include "Define.h"

class CInfoSubject : public CSubject
{
	DECLARE_SINGLETON(CInfoSubject)

public:
	static enum DataType
	{
		DATA_TYPE_CAMERA_VIEW,
		DATA_TYPE_CAMERA_PROJECTION,
		DATA_TYPE_CAMERA_EYE,
		DATA_TYPE_CAMERA_ZOOM,

		//DATA_TYPE_PLAYER_INITPLAYER,
		//DATA_TYPE_PLAYER_PLAYERPOS,
		DATA_TYPE_PLAYER_KEY,
		DATA_TYPE_PLAYER_ROTATE,

		DATA_TYPE_PLAYER_SHOT,

		//DATA_TYPE_ENEMY_INITPLAYER,
		//DATA_TYPE_ENEMY_PLAYERPOS,
		//DATA_TYPE_ENEMY_PLAYERROTATE,

		DATA_TYPE_PLAYER_BULLET,
		DATA_TYPE_PLAYER_HP,
		DATA_TYPE_PLAYER_RECOSTER,
		DATA_TYPE_PLAYER_CUR_MAPLENGHT,

		DATA_TYPE_UI_STATIE,
		DATA_TYPE_UI_DIR_TO_TARGET,
	};

private :
	map<int, vector<void*>>		m_mapData;


public:
	vector<void*>* GetDatalist(int _imessage);


public:
	void AddData(int _iMessage, void* _pData);
	void RemoveData(int _iMessage, void* _pData);

private:
	void Release(void);


private :
	CInfoSubject();
	virtual ~CInfoSubject();
};

#endif // InfoSubject_h__
