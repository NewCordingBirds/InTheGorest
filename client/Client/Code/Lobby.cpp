#include "stdafx.h"
#include "Lobby.h"


CLobby::CLobby(CDevice* _pDevice)
: CScene(_pDevice)
{
}


CLobby::~CLobby()
{
}

CLobby* CLobby::Create(CDevice* _pDevice)
{
	CLobby* pScene = new CLobby(_pDevice);

	if (FAILED(pScene->Init()))
		::Safe_Delete(pScene);

	return pScene;
}

HRESULT CLobby::Init()
{
	return S_OK;
}

void CLobby::Update()
{

}

void CLobby::Render()
{

}

void CLobby::Release()
{

}
