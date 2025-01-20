#include "stdafx.h"
#include "..\public\CMainApp.h"
#include "../Public/Level_Loading.h"
#include "../Public/Level_MapTool.h"



// ../../Engine/ 이런식으로 경로를 안타고 바로 이름.h로 접근할수있는이유는 
// EngineSDK에 우리가 lib랑 dll을 모아두고 거기를 추가 포함디렉토리로 참조하기때문에 가능한것이다.
  
#include "GameInstance.h"


CMainApp::CMainApp()
	: m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	ENGINE_DESC			EngineDesc{};
	EngineDesc.hWnd = g_hWnd;				// 핸들 받아오고
	EngineDesc.iWinSizeX = g_iWinSizeX;     // window 사이즈
	EngineDesc.iWinSizeY = g_iWinSizeY;		// window 사이즈
	EngineDesc.isWindowsed = true;			// 창모드할건지 아닌지 

	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END,EngineDesc, &m_pGraphic_Device)))
		return E_FAIL;
	
	if (FAILED(Ready_Default_Setting()))
		return E_FAIL;

	if(FAILED(Open_Level(LEVEL_LOADING)))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component_Static()))
		return E_FAIL;

	
	return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	m_pGameInstance->Update_Engine(fTimeDelta);

}

void CMainApp::Final_Update()
{
	m_pGameInstance->FinalUpdate_Engine();
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;


	m_pGameInstance->Render_Begin();

	m_pGameInstance->Draw_Engine();

	

	m_pGameInstance->Render_End();

	return S_OK;
}





HRESULT CMainApp::Ready_Default_Setting()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);



	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component_Static()
{
	/* For.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_Animation */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Animation"),
		CAnimator::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For. Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Fsm"),
		CFsm::Create(m_pGraphic_Device))))
		return E_FAIL;



	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVELID eStartLevelID)
{
	/* 어떤 레벨을 선택하던 로딩 레벨로 선 진입한다.  */
	/* 로딩레벨에서 내가 선택한 레벨에 필요한 자원을 준비한다. */
	/* 로딩 레벨은 다음레벨(내가 선택한 레벨)이 누구인지를 인지해야한다. */

	//if(FAILED(m_pGameInstance->Change_Level(LEVEL_LOADING,CLevel_Loading::Create(m_pGraphic_Device, eStartLevelID))))
	//	return E_FAIL;

	if (FAILED(m_pGameInstance->Change_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device,LEVEL_MAPTOOL))))
		return E_FAIL;

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pMainApp = new CMainApp;

	if (FAILED(pMainApp->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CMainApp"));
		Safe_Release(pMainApp);
	}

	return pMainApp;
}

void CMainApp::Free()
{

	__super::Free();

	Safe_Release(m_pGraphic_Device);

	m_pGameInstance->Release_Engine();

	Safe_Release(m_pGameInstance);

}


