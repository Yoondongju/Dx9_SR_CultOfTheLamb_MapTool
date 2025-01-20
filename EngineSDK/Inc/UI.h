#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTransform;

class ENGINE_DLL CUI abstract : public CGameObject
{
protected:
	CUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUI(const CGameObject& Prototype);
	virtual ~CUI() = default;

public:
	void SetMovement(bool _b) { m_bMovement = _b; }
	bool GetMovement() { return m_bMovement; }

	bool IsMouseOn() { return m_bMouseOn; }
	bool IsLbtnDown() { return m_bLbtnDown; }

	CUI* GetParent() { return m_pParentUI; }
	list<CUI*>& GetChildUI() { 	return m_childUI_List; }


	UI_TYPE	 GetUI_Type() { return m_eUItype; }

public:
	void SetActivate(bool _b)
	{
		m_bActivate = _b;

		//필요하면 맞게 수정하기
		//if (!m_bActivate)
		//{
		//	CUIMgr::CreateSingleTonInst()->SetFocusUI(nullptr);
		//	m_bMovement = false;
		//}

	}
	bool GetActivate() { return m_bActivate; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	CTransform*				m_pTransformCom = { nullptr };

	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4				m_ViewMatrix, m_ProjMatrix;

	list<CUI*>				m_childUI_List;

	CUI*					m_pParentUI = { nullptr };	 // 부모 UI가 있니

	bool					m_bMovement = {};     // UI가 움직일수 있는지?
	bool					m_bMouseOn = {};	 // UI 위에 마우스가 있는지
	bool					m_bLbtnDown = {};	 // UI에 왼쪽버튼이 눌러져 있는지

	bool					m_bActivate = {};	 // 활성화
	UI_TYPE					m_eUItype = { UI_TYPE::UI_END };

protected:
	virtual void MouseOnCheck();				// 모든 자식 UI들은 부모꺼를 쓸거임

	virtual void MouseLBtnDown();
	virtual void MouseLBtnUp();
	virtual void MouseClick();


	virtual void MouseOn();
	virtual void MouseOut();

	void	SetParentUI(CUI* _p) { m_pParentUI = _p; }

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;



	friend class CUI_Manager;
};

END
