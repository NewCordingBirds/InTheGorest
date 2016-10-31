#pragma once
#include "Scene.h"

class CDevice;
class CRoom : public CScene
{
public:
	static CRoom*	Create(CDevice*);

public:
	virtual HRESULT		Init();
	virtual void		Update();
	virtual void		Render();
	virtual void		Release();

private:
	explicit CRoom(CDevice*);
public:
	virtual ~CRoom();
};

