#pragma once

#include "Base.h"
/* 모든 게임내에 사용되는 게임 오브젝트들의 부모 클래스다. */
BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject(const CGameObject& Prototype);

	virtual ~CGameObject() = default;


public:
	_bool	Get_Dead() { return m_isDead; }
	void	Set_Dead(_bool isDead) { m_isDead = isDead; }


	int			Get_BindTexNum() { return m_iBindTexNum; }
	_wstring	Get_PrototypeName() { return m_strName; }


	void		Set_LayerName(_wstring strLayerName) { m_strLayerName = strLayerName; }
	_wstring    Get_LayerName() { return m_strLayerName; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();


public:
	class CComponent* Find_Component(const _wstring & strComponentTag);


protected:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = { nullptr };
	class CGameInstance*	m_pGameInstance = { nullptr };

	int						m_iBindTexNum = -1;

	_wstring				m_strName = L"";		// 맵툴에서 저장불러오기할때 도저히 판단기준을 잡을수없어서 만듬
	_wstring				m_strLayerName = L"";


	_bool					m_isDead = { false };



protected:
	map<const _wstring, class CComponent*>	m_Components;
	class CTransform* m_pTransformCom = nullptr;

	HRESULT	Add_Component(_uint iLevelIndex, const _wstring strPrototypeTag, const _wstring & strComponentTag, CComponent** ppOut, void* pArg);


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END

