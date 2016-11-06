#include "stdafx.h"
#include "Player.h"

#include "Timer.h"
#include "KeyMgr.h"
#include "ColMgr.h"

#include "ObjMgr.h"
#include "Bullet.h"

#include "Trigger.h"

#include "UIObserver.h"
#include "Item.h"
#include "UnMovableZone.h"
#include "BoosterZone.h"
#include "ProgressRateZone.h"

#include <conio.h>

extern HWND		g_hWnd;


CPlayer::CPlayer(CDevice* _pDevice)
	: CRider(_pDevice)
	, m_pKeyMgr(CKeyMgr::GetInstance())
	, m_pTimer(CTimer::GetInstance())
	, m_pObjMgr(CObjMgr::GetInstance())
	, m_pServer(CServer::GetInstance())

, m_pInfoSubject(CInfoSubject::GetInstance())
, m_pUIObserver(NULL)

, m_eBulletState(BULLET_STATE_NONE)

, posstarttime(0)
, posendtime(0)
, rotatestarttime(0)
, rotateendtime(0)
{
	sound.SoundInit();
	ZeroMemory(&m_tKey, sizeof(CS_Key));
}

CPlayer::~CPlayer()
{
	Release();
}

CObj* CPlayer::Create(CDevice* _pDevice)
{
	CObj* pObj = new CPlayer(_pDevice);
	if (FAILED(pObj->Init()))
		::Safe_Delete(pObj);

	return pObj;
}

HRESULT CPlayer::Init()
{
	m_eObjState = CObj::OBJ_STATE_ALIVE;

	m_pObjCol->m_pOwnerPhysics = m_pPhysics;
	m_pObjCol->m_vPlusPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_pObjCol->SetScale(D3DXVECTOR3(1.5f, 1.5f, 2.4f));
	m_pObjCol->SetPlusPos(D3DXVECTOR3(0.f, 2.5f, 0.f));

	m_pPhysics->m_vDir = g_vLook;
	m_pPhysics->m_fSpeed = 150.f;
	m_pPhysics->m_vScale = D3DXVECTOR3(0.03f, 0.03f, 0.03f);

	m_pPlayingInfo->m_eAniState = CPlayingInfo::ANI_STATE_RUN;

	m_pInfoSubject->AddData(CInfoSubject::DATA_TYPE_PLAYER_KEY, &m_tKey);
	m_pInfoSubject->AddData(CInfoSubject::DATA_TYPE_PLAYER_ROTATE, &m_tRotate);
	m_pInfoSubject->AddData(CInfoSubject::DATA_TYPE_PLAYER_SHOT, &m_tShoot);

	m_pInfoSubject->AddData(CInfoSubject::DATA_TYPE_PLAYER_BULLET, &m_pPlayingInfo->m_iBulletNum);
	m_pInfoSubject->AddData(CInfoSubject::DATA_TYPE_PLAYER_HP, &m_pPlayingInfo->m_iHp);
	m_pInfoSubject->AddData(CInfoSubject::DATA_TYPE_PLAYER_RECOSTER, &m_pPlayingInfo->m_iRecoter);

	m_pInfoSubject->AddData(CInfoSubject::DATA_TYPE_PLAYER_CUR_MAPLENGHT, &m_fProgressRate);

	m_pUIObserver = CUIObserver::Create();
	CInfoSubject::GetInstance()->Subscribe(m_pUIObserver);

	m_iprogressnum = 0;
	m_fProgressRate = 0;

	sound.PlayMusic(0);
	
	return S_OK;
}

int CPlayer::Update()
{
	switch (m_eObjState)
	{
	case CObj::OBJ_STATE_NONE:
		break;

	case CObj::OBJ_STATE_IDLE:
		break;

	case CObj::OBJ_STATE_ALIVE:
		
		switch (m_pPlayingInfo->m_ePlayingState)
		{
		case CPlayingInfo::PLAYING_STATE_NONE :
			break;

		case CPlayingInfo::PLAYING_STATE_STANDBY:
			CRider::SetInitPlayer(m_pServerMgr->GetInitPlayer());
			m_pPlayingInfo->m_ePlayingState = CPlayingInfo::PLAYING_STATE_READY;
			break;

		case CPlayingInfo::PLAYING_STATE_READY :
			break;

		case CPlayingInfo::PLAYING_STATE_PLAYING :
			
			CheckKey();
			CheckCol();

			//if (m_pPlayingInfo->m_eAniState != CPlayingInfo::ANI_STATE_FALL)
				UpdateServerData_Physics();

			RotateToServer();
			UpdateCurMapLength();
			break;

		case CPlayingInfo::PLAYING_STATE_GOAL :
			break;

		default:
			break;
		}

		//FixMouse();
		CheckTestKey();
		CObj::Update_Component();
		break;

	case CObj::OBJ_STATE_DIE:
		break;
	}

	return m_eObjState;
}

void CPlayer::Render()
{
	m_pPhysics->Render();
	m_pTexture->Render();

#ifdef _DEBUG
	if (!(g_dwClientDebugMode & CLIENT_DEBUG_MODE_NO_ANIMATION))
#endif
		
	PlayAnimation();

	if (m_bRenderObjCol_Other)
		m_pObjCol->Render();
}

void CPlayer::CheckKey()
{
	//if (m_pPlayingInfo->m_eAniState == CPlayingInfo::ANI_STATE_FALL)
	//{
	//	m_pPhysics->m_vPos.y -= CTimer::GetInstance()->GetTime() * m_pPhysics->m_fSpeed;
	//	return;
	//}

	float fTime = m_pTimer->GetTime();

	CheckKey_Move(fTime);
	CheckKey_Rotate(fTime);
	CheckKey_Bullet(fTime);
	CheckKey_UseRecoster();


	m_tKey.type = CS_KEY;
	m_tKey.size = sizeof(CS_Key);

	m_pInfoSubject->Notify(CInfoSubject::DATA_TYPE_PLAYER_KEY);
}

void CPlayer::CheckTestKey()
{
	// 명지 ------------------------------

	if (m_pKeyMgr->CheckKeyboardPressed(DIK_1))
		m_bRenderObjCol_Other = !m_bRenderObjCol_Other;
	else if (m_pKeyMgr->CheckKeyboardPressed(DIK_2))
		m_bRenderObjCol_UnMovableZone = !m_bRenderObjCol_UnMovableZone;
	else if (m_pKeyMgr->CheckKeyboardPressed(DIK_3))
		m_bRenderObjCol_ProgressRateZone = !m_bRenderObjCol_ProgressRateZone;
	else if (m_pKeyMgr->CheckKeyboardPressed(DIK_4) && m_eObjState == CPlayingInfo::PLAYING_STATE_PLAYING)
	{
		m_pPlayingInfo->m_iHp = 0;
		m_pInfoSubject->Notify(CInfoSubject::DATA_TYPE_PLAYER_HP);
		SetAnimation(CPlayingInfo::ANI_STATE_FALL);

		CS_Die tDie;
		tDie.type = CS_DIE;
		tDie.size = sizeof(tDie);

		CServer::GetInstance()->SendServerData(reinterpret_cast<char*>(&tDie));

		//m_pPhysics->m_vDir.y *= -1;
		//m_pPhysics->m_fSpeed = 100.f;
	}
		

	//else if (m_pKeyMgr->CheckKeyboardPressed(DIK_NUMPAD2))
	//{
	//	m_pPlayingInfo->m_iHp -= 1;
	//	m_pInfoSubject->Notify(CInfoSubject::DATA_TYPE_PLAYER_HP);
	//}
	//else if (m_pKeyMgr->CheckKeyboardPressed(DIK_NUMPAD3))
	//{
	//	m_pPlayingInfo->m_iHp += 1;
	//	m_pInfoSubject->Notify(CInfoSubject::DATA_TYPE_PLAYER_HP);
	//}

	//else if (m_pKeyMgr->CheckKeyboardPressed(DIK_9))
	//{
	//	m_pPlayingInfo->m_ePlayingState = CPlayingInfo::PLAYING_STATE_PLAYING;
	//	CInMapMgr::GetInstance()->SetInMapState(CInMapMgr::INMAP_STATE_PLAYING);
	//}
}

void CPlayer::CheckKey_Move(float _fTime)
{
	/*static DWORD nStandardTime = GetTickCount();
	static DWORD nTimer = GetTickCount();
	nTimer = GetTickCount();

	if (nTimer - nStandardTime >= 20)
	{
	nStandardTime = nTimer;
	}
	else
	return;*/
	D3DXVECTOR3 vOriginPos = m_pPhysics->m_vPos;
	m_tKey.movetype = 0;
	if (m_pKeyMgr->CheckKeyboardPress(DIK_W))
	{
		SetAnimation(CPlayingInfo::ANI_STATE_RUN);
		m_tKey.movetype = KEY_UP;
		//_cprintf("%f %f %f", m_pPhysics->m_vPos.x, m_pPhysics->m_vPos.y, m_pPhysics->m_vPos.z);
	}
	if (m_pKeyMgr->CheckKeyboardPressed(DIK_W))
	{
		SetAnimation(CPlayingInfo::ANI_STATE_RUN);
		m_tKey.movetype = UPKEY_UP;
	}
	if (m_pKeyMgr->CheckKeyboardPress(DIK_S))
	{
		SetAnimation(CPlayingInfo::ANI_STATE_BREAK);
		m_tKey.movetype = KEY_DOWN;
	}
	if (m_pKeyMgr->CheckKeyboardPressed(DIK_S)){
		SetAnimation(CPlayingInfo::ANI_STATE_RUN);
		m_tKey.movetype = UPKEY_DOWN;
	}
/*	if (m_pKeyMgr->CheckKeyboardPress(DIK_A))
	{
		m_tKey.movetype |= KEY_LEFT;
	}
	if (m_pKeyMgr->CheckKeyboardPress(DIK_D))
	{
		m_tKey.movetype |= KEY_RIGHT;
	}	*/
	if (m_AvoidState == keyuptrig && m_pKeyMgr->CheckKeyboardPress(DIK_SPACE)){
		if (m_pKeyMgr->CheckKeyboardPress(DIK_A)){
				// 좌로굴러
				m_tKey.movetype |= AVOIDKEY_LEFT; //여기서 키 보내줍니다
				m_pPhysics->m_fSpeed = 33;
				m_pPhysics->m_fAcctime = 0;
				m_AvoidState = avoidtrig;
		}
		if (m_pKeyMgr->CheckKeyboardPress(DIK_D)){
			// 우로굴러
				m_tKey.movetype |= AVOIDKEY_RIGHT; //여기서 키 보내줍니다
				m_pPhysics->m_fSpeed = 33;
				m_pPhysics->m_fAcctime = 0;
				m_AvoidState = avoidtrig;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	if (m_AvoidState == avoidtrig || m_AvoidState == earlyup){
		m_pPhysics->AvoidPhysics(_fTime);
		SC_AvoidPlayer tempAvoid = m_pServerMgr->GetPlayerAvoid();
		m_bAvoidTrig = tempAvoid.endtriger;

		if (m_bAvoidTrig){
			m_pPhysics->m_vRadian.z = 0;
			if (m_AvoidState == earlyup)
				m_AvoidState = keyuptrig;
			else
				m_AvoidState = keydowntrig;
		}
		//m_pPhysics->m_vPos = m_pServerMgr->GetPlayerAvoid().pos;
	}

	if (m_pKeyMgr->CheckKeyboardPressed(DIK_A)){
		if (m_AvoidState != avoidtrig){
			m_AvoidState = keyuptrig;
		}
		else{
			m_AvoidState = earlyup;
		}
	}
}

void CPlayer::CheckKey_Rotate(float _fTime)
{
	int	iDistance = 0;
	//static DWORD nStandardTime = GetTickCount();
	//static DWORD nTimer = GetTickCount();
	//nTimer = GetTickCount();

	//if (nTimer - nStandardTime >= 20)
	//{
	//	nStandardTime = nTimer;

	//}
	//else{
	//	return;
	//}
	if (iDistance = m_pKeyMgr->GetDIMouseMove(CKeyMgr::MOUSE_MOVE_X))
	{
		m_pPhysics->m_vRadian.y += iDistance * _fTime;
		D3DXVec3TransformNormal(
			&m_pPhysics->m_vDir, &g_vLook, &m_pPhysics->m_matWorld);
		D3DXVec3Normalize(&m_pPhysics->m_vDir, &m_pPhysics->m_vDir);
		
	}

	if (iDistance = m_pKeyMgr->GetDIMouseMove(CKeyMgr::MOUSE_MOVE_Y))
	{
		const static float MAX_DIR_X	= 1.0f;
		const static float MIN_DIR_X	= -1.2f;


		m_pPhysics->m_vRadian.x += iDistance * _fTime;

		if (m_pPhysics->m_vRadian.x > MAX_DIR_X)
			m_pPhysics->m_vRadian.x = MAX_DIR_X;
		else if (m_pPhysics->m_vRadian.x < MIN_DIR_X)
			m_pPhysics->m_vRadian.x = MIN_DIR_X;

		D3DXVec3TransformNormal(
			&m_pPhysics->m_vDir, &g_vLook, &m_pPhysics->m_matWorld);
		D3DXVec3Normalize(&m_pPhysics->m_vDir, &m_pPhysics->m_vDir);
	}
}

void CPlayer::CheckKey_Bullet(float _fTime)
{
	static const float	TIME_CREATE_BULLET		= 0.1f;
	static const float	TIME_RELOAD_BULLET		= 1.5f;

	static float	fBulletTimer = 0.f;


	if (BULLET_STATE_RELOAD == m_eBulletState)
	{
		if (MAX_BULLET_NUM > m_pPlayingInfo->m_iBulletNum)
		{
			fBulletTimer += _fTime;

			if (fBulletTimer >= TIME_RELOAD_BULLET)
			{
				m_pPlayingInfo->m_iBulletNum = MAX_BULLET_NUM;
				m_pInfoSubject->Notify(CInfoSubject::DATA_TYPE_PLAYER_BULLET);
				m_eBulletState = BULLET_STATE_NONE;

				fBulletTimer = 0.f;
			}
		}
		return;
	}

	if (m_pKeyMgr->CheckMousePress(CKeyMgr::MOUSE_TYPE_R))
	{
		m_eBulletState = BULLET_STATE_RELOAD;
		fBulletTimer = 0.f;
	}
	else if (m_pKeyMgr->CheckMousePress(CKeyMgr::MOUSE_TYPE_L))
	{
		if (CPlayingInfo::ANI_STATE_FALL == m_pPlayingInfo->m_eAniState
			|| CPlayingInfo::ANI_STATE_DEAD == m_pPlayingInfo->m_eAniState
			|| (m_AvoidState == avoidtrig)
			|| (m_AvoidState == earlyup))
			return;

		if (0 < m_pPlayingInfo->m_iBulletNum)
		{
			fBulletTimer += _fTime;

			if (fBulletTimer >= TIME_CREATE_BULLET)
			{

				CreateBullet();

				m_pPlayingInfo->m_iBulletNum -= 1;
				m_pInfoSubject->Notify(CInfoSubject::DATA_TYPE_PLAYER_BULLET);

				//m_pServerMgr->SendServerData(reinterpret_cast<char*>(&tShoot));

				fBulletTimer = 0.f;
			}
		}/*
		CS_Shoot tShoot;
		tShoot.type = CS_SHOT;
		tShoot.size = sizeof(CS_Shoot);
		tShoot.ammonum = m_pPlayingInfo->m_iBulletNum;

		m_pServerMgr->SendServerData(reinterpret_cast<char*>(&tShoot));*/
	}
}

void CPlayer::CheckKey_UseRecoster()
{
	if (m_pKeyMgr->CheckKeyboardDown(DIK_LSHIFT))
	{
		if (m_pPlayingInfo->PlusRecoster(-RECOSTER_GAUGE_FULL))
		{
			m_tKey.movetype |= KEY_BOOSTER;
			m_pInfoSubject->Notify(CInfoSubject::DATA_TYPE_PLAYER_RECOSTER);
			
			CS_UseBooster tUseBooster;
			tUseBooster.type = CS_BOOSTERITEM;
			tUseBooster.size = sizeof(CS_UseBooster);

			SetAnimation(CPlayingInfo::ANI_STATE_BOOSTER);

			m_pServerMgr->SendServerData(reinterpret_cast<char*>(&tUseBooster));
		}
	}

	if (m_pKeyMgr->CheckKeyboardDown(DIK_LCONTROL))
	{
		if (m_pPlayingInfo->PlusRecoster(-RECOSTER_GAUGE_FULL))
		{
			m_pPlayingInfo->PlusHp(RECOSTER_HEAL_HP);
			m_pInfoSubject->Notify(CInfoSubject::DATA_TYPE_PLAYER_RECOSTER);
			m_pInfoSubject->Notify(CInfoSubject::DATA_TYPE_PLAYER_HP);
		}
	}
}

void CPlayer::CreateBullet()
{
	D3DXVECTOR3 vDir(0.f, 0.f, 0.f);

	if (1 == m_pUIObserver->GetTargetingState())
		vDir = m_pUIObserver->GetDirToTarget();
	else vDir = m_pPhysics->m_vDir;


	CObj*	pObj = CBullet::Create(
		m_pDevice,
		m_pPhysics->m_vPos + (m_pObjCol->m_fRadius * m_pPhysics->m_vDir),
		vDir);

	((CBullet*)pObj)->SendDataToServer(vDir);

	m_pObjMgr->AddObj(CLayer::LAYERTYPE_GAMELOGIC, L"Bullet_Player", pObj);
}

void CPlayer::ReloadingBullet()
{

}

void CPlayer::FixMouse()
{
	POINT	ptMouse = { ::CLIENT_WINCX >> 1, ::CLIENT_WINCY >> 1 };
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CPlayer::UpdateServerData_Physics()
{
	SC_PlayerPos tPlayerPos = m_pServerMgr->GetPlayerPos();
	m_pPhysics->m_vPos = tPlayerPos.move;
}

void CPlayer::CheckCol()
{
	static bool bColInItem = false;
	static bool bColInBoosterZone = false;
	static bool bColInProgressRateZone = false;
	static bool bColInUnMovableZone = false;
	static bool bColInBullet = false;


	CColMgr* pColMgr = CColMgr::GetInstance();


	CObj* pObj = NULL;
	if (pColMgr->CheckColObjToItem(m_pObjCol, &pObj))
	{
		if (!bColInItem)
		{
			CItem* pItem = ((CItem*)pObj);
			pItem->SendDataToServer();
			m_pPlayingInfo->PlusRecoster(pItem->GetRecosterGaugePlusValue());
			m_pInfoSubject->Notify(CInfoSubject::DATA_TYPE_PLAYER_RECOSTER);
		}

		bColInItem = true;
	}
	else bColInItem = false;

	if (pColMgr->CheckColObjToUnmovable(m_pObjCol))
	{
		CS_CollWall tCollWall;
		tCollWall.type = CS_COLLWALL;
		tCollWall.size = sizeof(CS_CollWall);

		m_pServerMgr->SendServerData(reinterpret_cast<char*>(&tCollWall));

		if (m_pPlayingInfo->m_eAniState == CPlayingInfo::ANI_STATE_FALL)
		{
			m_pPhysics->m_fSpeed = 0.f;
			SetAnimation(CPlayingInfo::ANI_STATE_RUN);

			m_pPlayingInfo->PlusHp(MAX_HP/2);
			m_pInfoSubject->Notify(CInfoSubject::DATA_TYPE_PLAYER_HP);
		}
	}


	if (pColMgr->CheckColObjToObj(CColMgr::COL_TARGET_PROGRESSRATEZONE, m_pObjCol, &pObj))
	{

		if (!bColInProgressRateZone)
		{
			//AllocConsole();
			//_cprintf("ID : %d\n", ((CProgressRateZone*)pObj)->GetID());
			//_cprintf("Num : %d\n", m_iprogressnum);

			if (m_iprogressnum == (PROGRESS_RATE_ZONE_NUM * 2) - 1)
			{
				m_pPlayingInfo->m_ePlayingState = CPlayingInfo::PLAYING_STATE_GOAL;
				CInMapMgr::GetInstance()->SetInMapState(CInMapMgr::INMAP_STATE_END);
			}
			else if (PROGRESS_RATE_ZONE_NUM - 1 <= m_iprogressnum)
			{
				if (((CProgressRateZone*)pObj)->GetID() == m_iprogressnum - (PROGRESS_RATE_ZONE_NUM - 1))
					++m_iprogressnum;
				else --m_iprogressnum;
			}
			else
			{
				if (((CProgressRateZone*)pObj)->GetID() == m_iprogressnum + 1)
					++m_iprogressnum;
				else --m_iprogressnum;
			}
		}
		bColInProgressRateZone = true;
	}
	else
		bColInProgressRateZone = false;


	if (pColMgr->CheckColObjToObj(CColMgr::COL_TARGET_BOOSTERZONE, m_pObjCol, &pObj))
	{
		if (!bColInBullet)
		{
			((CBoosterZone*)pObj)->SendDataToServer();
			SetAnimation(CPlayingInfo::ANI_STATE_BOOSTER);
		}

		bColInBoosterZone = true;
	}
	else bColInBoosterZone = false;


	if (pColMgr->CheckColObjToObj(m_pObjCol, L"Bullet_Enemy", &pObj)
		&& m_pPlayingInfo->m_eAniState != CPlayingInfo::ANI_STATE_FALL)
	{
		if (!bColInBullet)
		{
			((CBullet *)pObj)->SetObjState(OBJ_STATE_DIE);
			SetAnimation(CPlayingInfo::ANI_STATE_DAMAGE);

			m_pPlayingInfo->PlusHp(-10);				// 총알을 맞으면 hp -10 감소
			m_pInfoSubject->Notify(CInfoSubject::DATA_TYPE_PLAYER_HP);

			if (m_pPlayingInfo->m_iHp <= 0)
			{
				//m_pPhysics->m_vDir.y *= -1;
				//m_pPhysics->m_fSpeed = 80.f;
				SetAnimation(CPlayingInfo::ANI_STATE_FALL);

				CS_Die tDie;
				tDie.type = CS_DIE;
				tDie.size = sizeof(tDie);

				CServer::GetInstance()->SendServerData(reinterpret_cast<char*>(&tDie));
			}
		}

		bColInBullet = true;
	}
	else bColInBullet = false;
}

void CPlayer::SetAnimation(CPlayingInfo::AniState _eAniState)
{
	CRider::SetAnimation(_eAniState);

	CS_ClientAni tClientAni;
	tClientAni.type = CS_CLIENTANI;
	tClientAni.size = sizeof(CS_ClientAni);
	tClientAni.aninum = _eAniState;

	m_pServerMgr->SendServerData(reinterpret_cast<char*>(&tClientAni));
}

void CPlayer::Release()
{
	CInfoSubject::GetInstance()->Unsubscribe(m_pUIObserver);
	::Safe_Delete(m_pUIObserver);
}

void CPlayer::RotateToServer()
{
	//static DWORD nStandardTime = GetTickCount();
	//static DWORD nTimer = GetTickCount();
	//nTimer = GetTickCount();

	//if (nTimer - nStandardTime >= 20)
	//{
	//	nStandardTime = nTimer;

	//}
	//else{
	//	return;
	//}
	m_tRotate.rad = m_pPhysics->m_vRadian;
	m_tRotate.dir = m_pPhysics->m_vDir;

	m_tRotate.type = CS_ROTATE;
	m_tRotate.size = sizeof(CS_Rotate);

	m_pInfoSubject->Notify(CInfoSubject::DATA_TYPE_PLAYER_ROTATE);
}

void CPlayer::UpdateCurMapLength()
{
	m_pInfoSubject->Notify(CInfoSubject::DATA_TYPE_PLAYER_CUR_MAPLENGHT);
}
