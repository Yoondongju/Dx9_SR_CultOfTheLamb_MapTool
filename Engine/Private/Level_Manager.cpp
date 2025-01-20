#include "../Public/Level_Manager.h"
#include "../Public/Level.h"
#include "../Public/GameInstance.h"

CLevel_Manager::CLevel_Manager()
	:m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CLevel_Manager::Initialize()
{

	return S_OK;
}

void CLevel_Manager::Update(_float fTimeDelta)
{
	if (nullptr != m_pCurrentLevel)
		m_pCurrentLevel->Update(fTimeDelta);
}

HRESULT CLevel_Manager::Render()
{
	if (nullptr != m_pCurrentLevel)
		m_pCurrentLevel->Render();

	return S_OK;
}

HRESULT CLevel_Manager::Change_Level(_uint iLevelIndex , CLevel* pNextLevel)
{
	m_pGameInstance->Clear(m_iLevelIndex);  // ������ �Ѿ�� ������������ ����ϴ� ��ü�� �������� �ʿ䰡 �ִ�.
	 
	Safe_Release(m_pCurrentLevel);          // �� �ٲ� �������� �� �����Ѵ�.. �³�?


    m_pCurrentLevel = pNextLevel;
	m_iLevelIndex = iLevelIndex;

	return S_OK;
}

CLevel_Manager* CLevel_Manager::Create()
{
	CLevel_Manager* pInstance = new CLevel_Manager;
	
	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pCurrentLevel);
}
