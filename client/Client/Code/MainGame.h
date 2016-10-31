#pragma once

#include "Include.h"

class CDevice;
class CSceneMgr;
class CTimer;
class CKeyMgr;
class CServer;
class CMainGame
{
private :
	CDevice*		m_pDevice;
	CSceneMgr*		m_pSceneMgr;
	CTimer*			m_pTimer;
	CKeyMgr*		m_pKeyMgr;
	CServer*		m_pServerMgr;

public :
	HRESULT		Init();
	void		Update();
	void		Render();
	void		Release();

public:
	CMainGame();
	~CMainGame();
};

