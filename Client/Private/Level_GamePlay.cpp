#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"
#include "../Public/Camera.h"


CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(Ready_Layer_Camera()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround()))
		return E_FAIL;


	

	return S_OK;

	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	
}

HRESULT CLevel_GamePlay::Render()
{

	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera()
{
	static CCamera::CAMERA_DESC	Desc{};

	Desc.vEye = _float3(0.f, 10.f, -10.f);
	Desc.vAt = _float3(0.f, 0.f, 0.f);
	Desc.fFovy = D3DXToRadian(55.0f);	// 라디안이네
	Desc.fNear = 0.1f;
	Desc.fFar = 1000.f;
	Desc.fSpeedPerSec = 10.f;
	Desc.fRotationPerSec = D3DXToRadian(90.0f);


	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), TEXT("Prototype_GameObject_Camera"), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround()
{
	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_GamePlay*		pInstance = new CLevel_GamePlay(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();

}
