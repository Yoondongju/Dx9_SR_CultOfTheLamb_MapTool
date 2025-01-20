#include "stdafx.h"
#include "..\Public\Level_Logo.h"
#include "../Public/Level_Loading.h"
#include "GameInstance.h"



#define CUSTOM_FVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

CLevel_Logo::CLevel_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
{
}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(Ready_Layer_BackGround()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Logo::Update(_float fTimeDelta)
{
	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		if (FAILED(m_pGameInstance->Change_Level(LEVEL_LOADING,CLevel_Loading::Create(m_pGraphic_Device, LEVEL_GAMEPLAY))))
			return;
	}
}

//  시간단위로 업데이트를 체크해주는 기능을 추가해야
//  프레임이 떨어졋을때 막을수잇다  (유니티의  FixedUpdate) 근데 이건 멀티쓰레드의 개념이필요..
//  어려우니까 걍 느려지는 이유를 찾아서 최적화하자.


HRESULT CLevel_Logo::Render()
{
	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));


		

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround()
{
	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_LOGO, TEXT("Layer_BackGround"),
		TEXT("Prototype_GameObject_BackGround"))))
		return E_FAIL;

	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Logo*		pInstance = new CLevel_Logo(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Logo"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();

}
