/*!
 * \file Notify.h
 * \date 2016/03/28 6:14
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

#ifndef Notify_h__
#define Notify_h__

#include "InMapUI.h"


class CFont;
class CNotify : public CInMapUI
{
private:
	static const float	POS_X, POS_Y;
	static const WORD	MAX_START_COUNT_NUM = 3;


private:
	Sprite		m_tSprite_StartCountDown[MAX_START_COUNT_NUM];
	Sprite		m_tSprite_Start;
	Sprite		m_tSprite_Final;
	Sprite		m_tSprite_Lap;

	CFont*		m_pFont_LastCountDown;


public:
	static CObj*	Create(CDevice*);

public:
	virtual HRESULT	Init();
	virtual int		Update();
	virtual void	Render();
	virtual void	Release();


public:
	static enum AnimationType
	{
		ANIMATION_TYPE_NONE,

		ANIMATION_TYPE_CNT_3,
		ANIMATION_TYPE_CNT_2,
		ANIMATION_TYPE_CNT_1,

		ANIMATION_TYPE_START,
		ANIMATION_TYPE_FINALLAP
	};

private :
	static const float	TIME_UI_ANIMATING_COUNTDOWN;
	static const float	TIME_UI_ANIMATING_OTHER;

	static const float	DISTANCE_UI_ANIMATING;

	static const float	SPEED_UI_ANIMATING_COUNTDOWN;
	static const float	SPEED_UI_ANIMATING_START;
	static const float	SPEED_UI_ANIMATING_FINAL;
	static const float	SPEED_UI_ANIMATING_LAP;

	AnimationType		m_eCurAnimatingType;
	float				m_fAniTimer;

public :
	void	SetCurAnimatingType(const AnimationType& _eAnimationType);
	void	SetCurAnimatingType(const int& _iCountDownNum);

private :
	void	InitUIPos();
	void	AnimationgUI();
	void	RenderUI();


protected:
	explicit CNotify(CDevice*);
public:
	virtual ~CNotify();
};


#endif // Notify_h__
