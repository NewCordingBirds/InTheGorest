#pragma once
#include "Scene.h"


class CDevice;
class CTitle : public CScene
{
public:
	static CTitle*	Create(CDevice*);

public:
	virtual HRESULT		Init();
	virtual void		Update();
	virtual void		Render();
	virtual void		Release();

private:
	explicit CTitle(CDevice*);
public:
	virtual ~CTitle();
};

