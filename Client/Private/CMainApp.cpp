#include "stdafx.h"
#include "..\public\CMainApp.h"
#include "../Public/Level_Loading.h"
#include "../Public/Level_MapTool.h"



// ../../Engine/ �̷������� ��θ� ��Ÿ�� �ٷ� �̸�.h�� �����Ҽ��ִ������� 
// EngineSDK�� �츮�� lib�� dll�� ��Ƶΰ� �ű⸦ �߰� ���Ե��丮�� �����ϱ⶧���� �����Ѱ��̴�.
  
#include "GameInstance.h"


CMainApp::CMainApp()
	: m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	ENGINE_DESC			EngineDesc{};
	EngineDesc.hWnd = g_hWnd;				// �ڵ� �޾ƿ���
	EngineDesc.iWinSizeX = g_iWinSizeX;     // window ������
	EngineDesc.iWinSizeY = g_iWinSizeY;		// window ������
	EngineDesc.isWindowsed = true;			// â����Ұ��� �ƴ��� 

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
	/* � ������ �����ϴ� �ε� ������ �� �����Ѵ�.  */
	/* �ε��������� ���� ������ ������ �ʿ��� �ڿ��� �غ��Ѵ�. */
	/* �ε� ������ ��������(���� ������ ����)�� ���������� �����ؾ��Ѵ�. */

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


