#pragma once
#include "Base.h"


BEGIN(Engine)

class CRenderer final : public CBase
{
public:
	enum RENDERGROUP  { RG_PRIORITY, RG_NONBLEND, RG_BLEND, RG_UI, RG_END };

private:
	CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CRenderer() = default;


public:
	list<class CGameObject*>& Get_RenderList(RENDERGROUP eGroup) { return m_RenderObjects[RG_UI]; }

public:
	HRESULT Initialize();
	HRESULT Add_RenderObject(RENDERGROUP eRenderGroupID, class CGameObject* pRenderObject);
	HRESULT Draw();


private:
	LPDIRECT3DDEVICE9				m_pGraphic_Device = { nullptr };
	list<class CGameObject*>		m_RenderObjects[RG_END];

private:
	HRESULT Render_Priority();
	HRESULT Render_NonBlend();
	HRESULT Render_Blend();
	HRESULT Render_UI();

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END

