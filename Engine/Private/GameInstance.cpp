#include "../Public/GameInstance.h"
#include "../Public/Graphic_Device.h"

#include "../Public/Level_Manager.h"
#include "../Public/Object_Manager.h"
#include "../Public/Timer_Manager.h"

/* 우리가 추가한 기능*/
#include "../Public/Key_Manager.h"
#include "../Public/CEvent_Manager.h"
#include "../Public/UI_Manager.h"
#include "../Public/PickingManager.h"



IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

POINT CGameInstance::Get_MosePos()
{
	POINT		ptMouse = {};
	GetCursorPos(&ptMouse);
	ScreenToClient(m_pGraphic_Device->Get_Handle(), &ptMouse);

	return ptMouse;
}

HRESULT CGameInstance::Initialize_Engine(_uint iNumLevels, const ENGINE_DESC& EngineDesc , LPDIRECT3DDEVICE9* ppDevice)
{
	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc, ppDevice);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/* 사운드 카드를 초기화하낟. */

	/* 입력장치를 초기화하낟. */

	/* 여러가지 매니져를 초기화하낟. */


	m_pRenderer = CRenderer::Create(*ppDevice);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pObject_Manager = CObject_Manager::Create(iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pComponent_Manager = CComponent_Manager::Create(iNumLevels);
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;
	


	m_pEvent_Manager = CEvent_Manager::Create(m_pLevel_Manager, m_pObject_Manager);
	if (nullptr == m_pEvent_Manager)
		return E_FAIL;


	m_pKey_Manager = CKey_Manager::Create(m_pGraphic_Device->Get_Handle());
	if (nullptr == m_pKey_Manager)
		return E_FAIL;

	m_pUI_Manager = CUI_Manager::Create();
	if (nullptr == m_pUI_Manager)
		return E_FAIL;

	m_pPicking_Manager = CPickingManager::Create(*ppDevice, m_pGraphic_Device->Get_Handle());
	if (nullptr == m_pPicking_Manager)
		return E_FAIL;


	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
	//키 값은 항상 먼저 update
	m_pKey_Manager->Update();

	m_pPicking_Manager->Update();

	m_pObject_Manager->Priority_Update(fTimeDelta);
	m_pObject_Manager->Update(fTimeDelta);
	m_pObject_Manager->Late_Update(fTimeDelta);


	m_pLevel_Manager->Update(fTimeDelta);

	//UIManager는 항상 Render 되기 전에 호출
	m_pUI_Manager->Update();
}

void CGameInstance::FinalUpdate_Engine()
{
	m_pEvent_Manager->update();
}

HRESULT CGameInstance::Draw_Engine()
{
	m_pRenderer->Draw();				// GameObj 렌더링하는부분

	return m_pLevel_Manager->Render();  // GameObj는 Renderer가 렌더링한다.. 그럼 LEVEL에서의 렌더는 뭘 렌더링하지?
}

HRESULT CGameInstance::Clear(_uint iLevelIndex)
{
	m_pObject_Manager->Clear(iLevelIndex);

	return S_OK;
}

void CGameInstance::Render_Begin()
{
	m_pGraphic_Device->Render_Begin();
}

void CGameInstance::Render_End(HWND hWnd)
{
	m_pGraphic_Device->Render_End(hWnd);
}


HRESULT CGameInstance::Change_Level(_uint iLevelIndx , CLevel* pNextLevel)
{
	CEvent_Manager::EVENT eEvent{};

	eEvent.eEven = CEvent_Manager::EVENT_TYPE::CHANGE_LEVEL;
	eEvent.lParam = (DWORD_PTR)iLevelIndx;
	eEvent.wParam = (DWORD_PTR)pNextLevel;

	m_pEvent_Manager->AddEvent(eEvent);

	return S_OK;
	//return m_pLevel_Manager->Change_Level(iLevelIndx,pNextLevel);
}

CLevel* CGameInstance::GetCurLevel()
{
	return m_pLevel_Manager->GetCurLevel();
}

HRESULT CGameInstance::Add_Prototype(const _wstring& strPrototypeTag, CGameObject* pPrototype)
{
	return m_pObject_Manager->Add_Prototype(strPrototypeTag, pPrototype);
}


HRESULT CGameInstance::Add_CloneObject_ToLayer(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strPrototypeTag, void* pArg)
{
	//CEvent_Manager::EVENT eEvent{};
	//eEvent.eEven = CEvent_Manager::EVENT_TYPE::CREATE_OBJECT;
	//
	//
	//CEvent_Manager::CLONE_DESC eCloneDesc{};
	//eCloneDesc.iLevelIndex = iLevelIndex;
	//eCloneDesc.strLayerTag = strLayerTag;
	//eCloneDesc.strPrototypeTag = strPrototypeTag;
	//eCloneDesc.pArg = pArg;
	//
	//eEvent.CloneDesc = eCloneDesc;
	//
	//m_pEvent_Manager->AddEvent(eEvent);   // << push_back 이기에 원본을 복사해서 넣기에 eEvent가 지역변수여도 상관없다
	//
	//return S_OK;
  	return m_pObject_Manager->Add_CloneObject_ToLayer(iLevelIndex, strLayerTag, strPrototypeTag, pArg);
}

CComponent* CGameInstance::Find_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex)
{
	return m_pObject_Manager->Find_Component(iLevelIndex, strLayerTag, strComponentTag, iIndex);
}

CGameObject* CGameInstance::Find_Player()
{
	return m_pObject_Manager->Find_Player();
}



CGameObject* CGameInstance::IsObjectAtPosition(_uint iLevelIndex, const _wstring& strLayerTag, _float3 vPosition)
{
	return m_pObject_Manager->IsObjectAtPosition(iLevelIndex,strLayerTag,vPosition);
}

CGameObject* CGameInstance::IsObjectAtPosition_GoodVerdict(_uint iLevelIndex, const _wstring& strLayerTag, _float3 vPosition)
{
	return m_pObject_Manager->IsObjectAtPosition_GoodVerdict(iLevelIndex, strLayerTag, vPosition);
}

CGameObject* CGameInstance::ObjectAtPosition(_uint iLevelIndex, const _wstring& strLayerTag, _float3 vPosition)
{
	return m_pObject_Manager->ObjectAtPosition(iLevelIndex,strLayerTag,vPosition);
}


list<class CGameObject*>& CGameInstance::Get_GameObjects(_uint iLevelIndx, const _wstring& strLayerTag)
{
	return m_pObject_Manager->Get_GameObjects(iLevelIndx,strLayerTag);
}



HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, CComponent* pPrototype)
{
	return m_pComponent_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);
}

CComponent* CGameInstance::Clone_Component(_uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	return m_pComponent_Manager->Clone_Component(iLevelIndex, strPrototypeTag, pArg);
}

CComponent* CGameInstance::Get_Prototype(_uint iNumLevelIndex, const _wstring& strPrototypeTag)
{
	return m_pComponent_Manager->Get_Prototype(iNumLevelIndex,strPrototypeTag);
}

HRESULT CGameInstance::Add_Timer(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Add_Timer(strTimerTag);
}

_float CGameInstance::Compute_TimeDelta(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Compute_TimeDelta(strTimerTag);
}

void CGameInstance::Update_Timer(const _wstring& strTimerTag)
{
	m_pTimer_Manager->Update(strTimerTag);
}

HRESULT CGameInstance::Add_RenderObject(CRenderer::RENDERGROUP eRenderGroupID, CGameObject* pRenderObject)
{
	if (FAILED(m_pRenderer->Add_RenderObject(eRenderGroupID, pRenderObject)))
		return E_FAIL;

	return S_OK;
}

list<class CGameObject*>& CGameInstance::Get_RenderList(CRenderer::RENDERGROUP eGroup)
{
	return m_pRenderer->Get_RenderList(eGroup);
}


KEY_STATE CGameInstance::Get_KeyState(KEY _eKey)
{
	return m_pKey_Manager->Get_KeyState(_eKey);
}

HRESULT CGameInstance::Event_CreateObject(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strPrototypeTag, _uint iSize, void* pArg)
{
	return E_NOTIMPL;
}

HRESULT CGameInstance::Event_DestoryObject(CGameObject* pGameObject)
{
	CEvent_Manager::EVENT tEvent = {};
	tEvent.eEven = CEvent_Manager::EVENT_TYPE::DELETE_OBJECT;


	CEvent_Manager::CLONE_DESC* pCloneDesc = new CEvent_Manager::CLONE_DESC;
	tEvent.lParam = (DWORD_PTR)pGameObject;

	tEvent.CloneDesc = pCloneDesc;

	m_pEvent_Manager->AddEvent(tEvent);
	return E_NOTIMPL;
}

_float3 CGameInstance::Get_PickingObject_CenterPos(_uint iLevelIndex , const _wstring& strLayerTag)
{
	// 클릭 했을때만 업데이트 돌자
	CLayer* pLayer = m_pObject_Manager->Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
	{
		return _float3(0.f,0.f,0.f);
	}
	
	 // 현재 레벨의 Picking 체크할 레이어의 모든 오브젝트를 순회해야한다.
	
	return m_pPicking_Manager->Get_PickingObject_CenterPos(pLayer);
}

_float3 CGameInstance::Get_PickingObject_JustPos(_uint iLevelIndex, const _wstring& strLayerTag)
{
	CLayer* pLayer = m_pObject_Manager->Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
	{
		return _float3(0.f, 0.f, 0.f);
	}

	// 현재 레벨의 Picking 체크할 레이어의 모든 오브젝트를 순회해야한다.

	return m_pPicking_Manager->Get_PickingObject_JustPos(pLayer);
}

_float3 CGameInstance::Get_PickingBackGround_JustPos()
{
	return m_pPicking_Manager->Get_PickingBackGround_JustPos();
}

void CGameInstance::Transform_MouseRay_ToLocalSpace(const _float4x4& WorldMatrix)
{
	return m_pPicking_Manager->Transform_ToLocalSpace(WorldMatrix);
}

_bool CGameInstance::isPicked_InLocalSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut)
{
	return m_pPicking_Manager->isPicked_InLocalSpace(vPointA, vPointB, vPointC, pOut);
}

void CGameInstance::Delete(class CGameObject* pObj)
{
	CEvent_Manager::EVENT eEvent{};
	eEvent.eEven = CEvent_Manager::EVENT_TYPE::DELETE_OBJECT;
	eEvent.lParam = (DWORD_PTR)pObj;

	m_pEvent_Manager->AddEvent(eEvent);
}


void CGameInstance::Release_Engine()
{
	Safe_Release(m_pPicking_Manager);
	Safe_Release(m_pUI_Manager);
	Safe_Release(m_pKey_Manager);
	Safe_Release(m_pEvent_Manager);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pGraphic_Device);


	CGameInstance::Get_Instance()->Destroy_Instance();
}

void CGameInstance::Free()
{
	__super::Free();
	
}
