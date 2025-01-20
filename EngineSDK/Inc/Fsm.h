#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CFsm :
    public CComponent
{

private:
	CFsm(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFsm(const CFsm& Prototype);
	virtual ~CFsm() = default;

public:

	class CGameObject*	Get_Owner() { return m_pOwner; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta);
	virtual void Release_States();

public:
	void				Add_State(class CState* pState);
	void				Change_State(_uint eState, void* pArg = nullptr);
	void				Set_State(_uint eState, void* pArg = nullptr);

private:
	vector<class CState*> m_States;	//모든 상태를 갖고 있는 벡터 - 인덱스 접근용

	class CState* m_pNowState = { nullptr };			//지금 상태
	class CState* m_pPreState = { nullptr };			//이전 상태

	class CGameObject* m_pOwner = { nullptr };

public:
	static CFsm* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END
