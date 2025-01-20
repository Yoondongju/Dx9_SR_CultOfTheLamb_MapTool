#pragma once
#include "Client_Defines.h"
#include "LandObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

class CInteractObject final : public CLandObject
{
public:
	enum INTERACT_TYPE { ROCK, SHRINE, TREE, WEED , BUILD, INTERACTTYPE_END};


private:
	CInteractObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CInteractObject(const CInteractObject& Prototype);
	virtual ~CInteractObject() = default;

public:
	void			Set_MyType(INTERACT_TYPE _e) { m_eMyType = _e; }
	INTERACT_TYPE	Get_MyType() {return m_eMyType;}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };



	



	_uint				m_iAlpha = { 255 };
	
	INTERACT_TYPE		m_eMyType = { INTERACTTYPE_END };

private:
	HRESULT Ready_Components();
	HRESULT SetUp_RenderState();
	HRESULT Reset_RenderState();


	HRESULT Change_ComponentModel(const wstring& strComponentTag);

public:
	static CInteractObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

