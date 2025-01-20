#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{

private:
	CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& Prototype);
	virtual ~CVIBuffer_Terrain() = default;


public:
	_float2*		 GetFilterVerticesCoord() { return m_pFilterVerticesCoord; }


public:
	virtual HRESULT Initialize_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ);
	virtual HRESULT Initialize(void* pArg) override;


	void Bind_FilterCoord();


public:
	_float Compute_Height(const _float3 & vLocalPos);

	
private:
	_float2*		m_pFilterVerticesCoord = { nullptr };
	

public:
	static CVIBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END
