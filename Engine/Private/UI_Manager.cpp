#include "../Public/UI_Manager.h"
#include "../Public/GameInstance.h"

#include "../Public/UI.h"

CUI_Manager::CUI_Manager()
	: m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CUI_Manager::Initialize()
{
	return S_OK;
}

void CUI_Manager::Update()
{
	// ��Ŀ�� �� UI�� ã�´�

	m_pFocusUI = GetFocusUI();

	if (m_pFocusUI)
	{
		//Dead ó���� �������
		/*if (m_pFocusUI->Get_Dead())
		{
			m_pFocusUI = nullptr;
			return;
		}			*/
	}
	else
		return;

	
	
	CUI* pTargetUI = GetTargetUI(m_pFocusUI);

	bool bLbtnTap = m_pGameInstance->Get_KeyState(KEY::LBUTTON) == KEY_STATE::TAP;
	bool bLbtnAway = m_pGameInstance->Get_KeyState(KEY::LBUTTON) == KEY_STATE::AWAY;   // �� �� ����

	if (pTargetUI)
	{
		if (bLbtnTap)
		{
			pTargetUI->MouseLBtnDown();
			pTargetUI->m_bLbtnDown = true;
		}
		else if (bLbtnAway)
		{
			pTargetUI->MouseLBtnUp();
			
			if (pTargetUI->m_bLbtnDown)
				pTargetUI->MouseClick();

			pTargetUI->m_bLbtnDown = false;
		}
	}



}


void CUI_Manager::SetFocusUI(CUI* _pUI)
{
	m_pFocusUI = _pUI;

}



CUI* CUI_Manager::GetFocusUI()
{
	//CScene* pCureScene = CSceneMgr::CreateSingleTonInst()->GetCurScene();

	list<CGameObject*>& playerUIList = m_pGameInstance->Get_RenderList(CRenderer::RG_UI);

	bool bLbtnTap = m_pGameInstance->Get_KeyState(KEY::LBUTTON) == KEY_STATE::TAP;
	bool bLbtnAway = m_pGameInstance->Get_KeyState(KEY::LBUTTON) == KEY_STATE::AWAY;

	bool bLbtnHold = m_pGameInstance->Get_KeyState(KEY::LBUTTON) == KEY_STATE::HOLD;


	if (0 == playerUIList.size())
	{
		m_pFocusUI = nullptr;
	}

	CUI* pFocusUI = m_pFocusUI;

	list<CUI*> queue;
	CUI* pUI = nullptr;
	CUI* pTargetUI = nullptr;

	list<CGameObject*>::iterator targetiter = playerUIList.end();
	list<CGameObject*>::iterator iter = playerUIList.begin();

	if (!bLbtnTap)  // ���� �������� ���ٸ� ��Ŀ���� ���ֵ�, MouseOn�� ȣ���������..
	{
		for (; iter != playerUIList.end(); ++iter)
		{
			pUI = ((CUI*)*iter);  // �ֻ��� �θ�
			queue.push_back(pUI); // �� �θ� ť�� �ְ�

			while (!queue.empty())
			{
				pUI = queue.front();
				queue.pop_front();

				if (pUI->IsMouseOn())
				{
					pTargetUI = pUI;

					if (nullptr != pTargetUI)
						pTargetUI->MouseOn();
				}

				else if (!pUI->IsMouseOn())
				{
					pTargetUI = pUI;

					if (nullptr != pTargetUI)
						pTargetUI->MouseOut();
				}

				// �� �θ� pUI�� �ڽ�UI���͸� vecChild���� ��
				const list<CUI*>& PlayerUIList = pUI->GetChildUI();

				// queue�� �ڽ� UI�߰�
				for (auto& iter : PlayerUIList)
				{
					queue.push_back(iter);
				}
			}
		}

		return pFocusUI;
	}

	if (bLbtnTap || bLbtnHold)
	{
		for (iter; iter != playerUIList.end(); ++iter)
		{			
			pUI = ((CUI*)(*iter));
			bool m = pUI->IsMouseOn();
			if (pUI->IsMouseOn() ) // && pUI->GetActivate()
			{
				targetiter = iter;
			}
		}
	}

	if (targetiter == playerUIList.end())
	{
		return pFocusUI;
	}
		

	pFocusUI = (CUI*)(*targetiter);


	playerUIList.erase(targetiter);
	playerUIList.push_back(pFocusUI);
	
	return pFocusUI;
}

CUI* CUI_Manager::GetTargetUI(CUI* _pFocusUI)
{

	CUI* pTargetUI = nullptr;

	bool bLbtnAway = m_pGameInstance->Get_KeyState(KEY::LBUTTON) == KEY_STATE::AWAY;   // �� �� ��

	static list<CUI*> queue;
	static vector<CUI*> vecNoneTarget;
	
	
	queue.push_back(_pFocusUI);


	while (!queue.empty())			// �ȿ� ����ִ� �ڽĵ��� ���������� ����
	{
		CUI* pUI = queue.front();
		queue.pop_front();

		if (pUI->IsMouseOn())
		{
			if (nullptr != pTargetUI)
			{
				vecNoneTarget.push_back(pTargetUI);
			}
			pTargetUI = pUI;
		}
		else
		{
			vecNoneTarget.push_back(pUI);
		}


		const list<CUI*>& PlayerUIList = pUI->GetChildUI();

		for (auto& iter : PlayerUIList)
		{
			queue.push_back(iter);
		}
	}


	//if (bLbtnAway)
	//{
	//	for (auto& iter : vecNoneTarget)
	//	{
	//		iter->m_bLbtnDown = false;
	//	}
	//}

	vecNoneTarget.clear();

	return pTargetUI;
}

CUI_Manager* CUI_Manager::Create()
{
	CUI_Manager* pInstance = new CUI_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
}
