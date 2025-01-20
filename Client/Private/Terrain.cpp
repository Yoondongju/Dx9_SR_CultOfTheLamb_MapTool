#include "stdafx.h"
#include "..\Public\Terrain.h"

#include "GameInstance.h"
#include "../Public/Level_MapTool.h"


CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CTerrain::CTerrain(const CTerrain& Prototype)
	: CGameObject{ Prototype }
{
}


HRESULT CTerrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrain::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	//CLevel_MapTool::TERRAIN_DESC* pTerrainDesc = static_cast<CLevel_MapTool::TERRAIN_DESC*>(pArg);
	//pTerrainDesc->pTerrain = this;
	

	return S_OK;
}

void CTerrain::Priority_Update(_float fTimeDelta)
{
}

void CTerrain::Update(_float fTimeDelta)
{
	
}

void CTerrain::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CTerrain::Render()	// 이런애들은 렌더러가 호출해줌  
{
	if (FAILED(m_pTransformCom->Bind_Transform()))
		return E_FAIL;

	CLevel* pLevel = m_pGameInstance->GetCurLevel();
	CLevel_MapTool* pLevel_MapTool = dynamic_cast<CLevel_MapTool*>(pLevel);

	int iTileNum(0);
	if (pLevel_MapTool != nullptr)
		iTileNum = pLevel_MapTool->GetTileNum();


	if (FAILED(m_pTextureCom[0]->Bind_Texture(iTileNum)))			// 기본 맵 타일
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	
	_float2* pFiltercoord = m_pVIBufferCom->GetFilterVerticesCoord();


	if (FAILED(m_pTextureCom[1]->Bind_SecondTexture(0)))			// 맵 필터 
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))							// 기본 맵 타일 렌더링
		return E_FAIL;


	return S_OK;
}

HRESULT CTerrain::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Terrain"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[0]), nullptr)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Terrain_Filter"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[1]), nullptr)))
		return E_FAIL;

	

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_MAPTOOL, TEXT("Prototype_Component_VIBuffer_Terrain"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
		return E_FAIL;


	

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};

	TransformDesc.fSpeedPerSec = 20.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::SetUp_RenderState()
{
	return E_NOTIMPL;
}

HRESULT CTerrain::Reset_RenderState()
{
	return E_NOTIMPL;
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrain* pInstance = new CTerrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

/* 원형객체를 찾아서 원형객체의 Clone함수를 호출한다.  */
CGameObject* CTerrain::Clone(void* pArg)
{
	CTerrain* pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom[0]);
	Safe_Release(m_pTextureCom[1]);

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
}
