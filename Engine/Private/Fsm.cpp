#include "../Public/Fsm.h"
#include "../Public/State.h"
#include "../Public/GameObject.h"

CFsm::CFsm(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent{ pGraphic_Device }	
{
	m_eComponentType = FSM;
}

CFsm::CFsm(const CFsm& Prototype)
	:CComponent{ Prototype }
{
	m_eComponentType = FSM;
}

HRESULT CFsm::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFsm::Initialize(void* pArg)
{
	m_pOwner = static_cast<CGameObject*>(pArg);
	//Safe_AddRef(m_pOwner);

	return S_OK;
}

void CFsm::Update(_float fTimeDelta)
{
	m_pNowState->Update(fTimeDelta);
}

void CFsm::Release_States()
{
	for (auto& iter : m_States)
		Safe_Release(iter);

	Safe_Release(m_pNowState);
	Safe_Release(m_pPreState);

	m_States.clear();
}

void CFsm::Add_State(CState* pState)
{
	pState->SetFSM(this);
	m_States.push_back(pState);
}

void CFsm::Change_State(_uint iState, void* pArg)
{
	Safe_Release(m_pPreState);
	Safe_Release(m_pNowState);

	m_pPreState = m_pNowState;
	Safe_AddRef(m_pPreState);

	m_pNowState->End_State();

	for (int i = 0; i < m_States.size(); ++i)
	{
		if (iState == m_States[i]->Get_State())
		{
			m_pNowState = m_States[i];
			Safe_AddRef(m_pNowState);

			m_pNowState->Start_State();
			return;
		}
	}
}

void CFsm::Set_State(_uint iState, void* pArg)
{
	for (int i = 0; i < m_States.size(); ++i)
	{
		if (iState == m_States[i]->Get_State())
		{
			m_pNowState = m_States[i];
			m_pPreState = m_States[i];

			Safe_AddRef(m_pNowState);
			Safe_AddRef(m_pPreState);

			m_pNowState->Start_State();
			return;
		}
	}
}

CFsm* CFsm::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFsm* pInstance = new CFsm(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CFsm"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

/* ������ü�� ã�Ƽ� ������ü�� Clone�Լ��� ȣ���Ѵ�.  */
CComponent* CFsm::Clone(void* pArg)
{
	CFsm* pInstance = new CFsm(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CFsm"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFsm::Free()
{
	__super::Free();
}
