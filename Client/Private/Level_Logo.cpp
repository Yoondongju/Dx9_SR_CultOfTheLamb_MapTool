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

//  �ð������� ������Ʈ�� üũ���ִ� ����� �߰��ؾ�
//  �������� ������� �������մ�  (����Ƽ��  FixedUpdate) �ٵ� �̰� ��Ƽ�������� �������ʿ�..
//  �����ϱ� �� �������� ������ ã�Ƽ� ����ȭ����.


HRESULT CLevel_Logo::Render()
{
	SetWindowText(g_hWnd, TEXT("�ΰ����Դϴ�."));


		

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
