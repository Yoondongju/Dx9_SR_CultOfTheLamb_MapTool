#pragma once

#include "Client_Defines.h"
#include "BlendObject.h"

BEGIN(Engine)
class CTransform;
class CVIBuffer_Terrain;
END

BEGIN(Client)

class CLandObject abstract : public CBlendObject
{
public:
	typedef struct
	{

		CVIBuffer_Terrain*		pTerrainVIBuffer = { nullptr };
		CTransform*				pTerrainTransform = { nullptr };

		_uint					iInitBindTexNum = {};
		_float3					vInitPos = {};
		_float3					vInitScale = {};

		_float					fRotationX = { 0.f };
		_float					fRotationY = { 0.f };
		_float					fRotationZ = { 0.f };

		_wstring				strLayerName = L"";

		_uint					iPreModelType = {};
		_uint					iPreAdornType = {};
		_uint					iPreLandMode = {};

	}LANDOBJECT_DESC;

protected:
	CLandObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLandObject(const CLandObject& Prototype);
	virtual ~CLandObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT SetUp_OnTerrain(CTransform* pTransform, _float fOffsetY = 0.f);




protected:
	CVIBuffer_Terrain* m_pTerrainVIBuffer = { nullptr };
	CTransform*		   m_pTerrainTransform = { nullptr };


public:
	_uint				m_iPreModelType = {};
	_uint				m_iPreLandType = {};
	_uint				m_iPreAdornType = {};


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END