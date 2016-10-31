/*!
 * \file Item.h
 * \date 2016/03/13 0:51
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

#ifndef Item_h__
#define Item_h__

#include "Trigger.h"

class CPhysics;
class CTexture;
class CBuffer;
class CItem : public CTrigger
{
private :
	unsigned int		m_iID;


public :
	static enum ItemType {	ITEM_TYPE_FULL, ITEM_TYPE_THIRD };

protected:
	ItemType		m_eItemType;
	int				m_iGageValue;

	CTexture*		m_pTexture;
	CBuffer*		m_pBuffer;


public:
	static CObj*	Create(CDevice* _pDevice, ItemType _eItemType);

private:
	void	AddComponent(ItemType _eItemType);

public :
	void	SetWorldMatrix(const D3DXVECTOR3& _vPos = D3DXVECTOR3(0.f, 0.f, 0.f));


public:
	virtual HRESULT	Init();
	virtual int		Update();
	virtual void	Render();
	virtual void	Release();


public :
	unsigned int	GetID()							{ return m_iID; }
	int				GetRecosterGaugePlusValue();
	void			SendDataToServer();

private :
	void	Animating();


protected:
	explicit CItem(CDevice* _pDevice, ItemType _eItemType);
public:
	virtual ~CItem();
};

#endif // Item_h__
