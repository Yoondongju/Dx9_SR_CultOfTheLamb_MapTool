#pragma once
#include "Base.h"


BEGIN (Engine)
class CPickingManager final : public CBase
{
private:
	CPickingManager(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CPickingManager() = default;

public:
	HRESULT	Initialize(HWND hWnd);


public:
	_float3 Get_PickingObject_CenterPos(class CLayer* pLayer);
	_float3 Get_PickingObject_JustPos(class CLayer* pLayer);
	_float3 Get_PickingBackGround_JustPos();




public:
	void Update(); /* ��������ǥ��(����Ʈ��)�� ���콺 ��ǥ�� ���ϰ� �� ���콺�� ��ǥ�� ���� �����̽�(���� �����̽�)���� ���� �� ���� ��ŭ ������. */
	void Transform_ToLocalSpace(const _float4x4& WorldMatrix);
	_bool isPicked_InWorldSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut);
	_bool isPicked_InLocalSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut);


private:
	LPDIRECT3DDEVICE9				m_pGraphic_Device = { nullptr };

	_float3							m_vOriginal;
	_float3							m_vDirection;
	HWND							m_hWnd = {};
	

private:
	_float3						m_vRayPos_InLocalSpace = {};
	_float3						m_vRayDir_InLocalSpace = {};

public:
	static CPickingManager* Create(LPDIRECT3DDEVICE9 pGraphic_Device , HWND hWnd);
	virtual void Free() override;
};
END

