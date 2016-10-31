#pragma once
#include "Scene.h"


class CDevice;
class CLobby : public CScene
{
public:
	static CLobby*	Create(CDevice*);

public:
	virtual HRESULT		Init();
	virtual void		Update();
	virtual void		Render();
	virtual void		Release();

private:
	explicit CLobby(CDevice*);
public:
	virtual ~CLobby();
};

