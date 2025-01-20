#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include "GameInstance.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CCamera final : public CGameObject
{
public:
	typedef struct tagCamera : public CTransform::TRANSFORM_DESC
	{
		_float3			vEye;
		_float3			vAt;
		_float			fFovy;     // << 라디안 각도임지금
		_float			fNear;
		_float			fFar;
	}CAMERA_DESC;


private:
	CCamera(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCamera(const CCamera& Prototype);
	virtual ~CCamera() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	CTransform*		m_pTransformCom = { nullptr };

	_float			m_fFovy = {};
	_float			m_fNear = {};
	_float			m_fFar = {};

	POINT					m_ptOldMousePos = {};

private:
	HRESULT Ready_Components(void* pArg);

private:
	void Cam_ZoomIn();
	void Cam_ZoomOut();

public:
	static CCamera* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

