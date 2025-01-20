#pragma once

#include "Renderer.h"
#include "Component_Manager.h"

/* 1. ������ Ŭ���̾�Ʈ�� ������ ���� Ŭ������. */
/* 2. �������� Ŭ���̾�Ʈ�� �����ְ� ���� �Լ����� ��Ƴ��´�. */
/* 3. �Լ��� -> Ŭ������ ����Լ�. ��ü�� �ʿ��ϴ�! �׷��� ������� �������ֱ����� Ŭ���� ��ü�� �����Ѵ�. */
/* 4. ��ü���� �����ϱ����� Initialize_Engine() �Լ� �ȿ��� ����� �������ֱ����� Ŭ�������� ��üȭ�Ѵ�. */

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	POINT Get_MosePos();

public:
	HRESULT Initialize_Engine(_uint iNumLevels , const ENGINE_DESC& EngineDesc, _Inout_ LPDIRECT3DDEVICE9* ppDevice);
	void Update_Engine(_float fTimeDelta);

	void FinalUpdate_Engine();

	HRESULT Draw_Engine();
	HRESULT Clear(_uint iLevelIndex);


#pragma region GRAPHIC_DEVICE
public:
	void Render_Begin();  					  // Get�Լ� �Ⱦ��� Graphic_Device�� �Լ� ����ϱ����� ���ο���
	void Render_End(HWND hWnd = nullptr);	  // Get�Լ� �Ⱦ��� Graphic_Device�� �Լ� ����ϱ����� ���ο���
#pragma endregion

#pragma region LEVEL_MANAGER
public:
	HRESULT Change_Level(_uint iLevelIndx , class CLevel* pNextLevel);
	class CLevel* GetCurLevel();
#pragma endregion

#pragma region OBJECT_MANAGER
public:
	HRESULT Add_Prototype(const _wstring & strPrototypeTag, class CGameObject* pPrototype); // ���� �ε����� ���°� OBJ
	HRESULT Add_CloneObject_ToLayer(_uint iLevelIndex, const _wstring & strLayerTag, const _wstring & strPrototypeTag, void* pArg = nullptr);
	class CComponent* Find_Component(_uint iLevelIndex, const _wstring & strLayerTag, const _wstring & strComponentTag, _uint iIndex = 0);

	
	//bool IsObjectAtPosition(_uint iLevelIndex, const _wstring & strLayerTag, _float3 vPosition);
	class CGameObject* ObjectAtPosition(_uint iLevelIndex, const _wstring & strLayerTag, _float3 vPosition);


	class CGameObject* Find_Player();


	class CGameObject* IsObjectAtPosition(_uint iLevelIndex, const _wstring & strLayerTag, _float3 vPosition);
	CGameObject* IsObjectAtPosition_GoodVerdict(_uint iLevelIndex, const _wstring & strLayerTag, _float3 vPosition);

	list<class CGameObject*>& Get_GameObjects(_uint iLevelIndx, const _wstring & strLayerTag);
#pragma endregion


#pragma region COMPONENT_MANAGER
	HRESULT Add_Prototype(_uint iLevelIndex, const _wstring & strPrototypeTag, class CComponent* pPrototype); // ���� �ε����� �ִ°� ������Ʈ
	class CComponent* Clone_Component(_uint iLevelIndex, const _wstring & strPrototypeTag, void* pArg = nullptr);
	class CComponent* Get_Prototype(_uint iNumLevelIndex, const _wstring & strPrototypeTag);	
#pragma endregion


#pragma region TIMER_MANAGER
	HRESULT Add_Timer(const _wstring & strTimerTag);
	_float Compute_TimeDelta(const _wstring & strTimerTag);
	void Update_Timer(const _wstring & strTimerTag);
#pragma endregion

#pragma region RENDERER
	HRESULT Add_RenderObject(CRenderer::RENDERGROUP eRenderGroupID, class CGameObject* pRenderObject);
	list<class CGameObject*>& Get_RenderList(CRenderer::RENDERGROUP eGroup);
#pragma endregion


#pragma region KEY_MANAGER
	KEY_STATE	Get_KeyState(KEY _eKey);
#pragma endregion

#pragma region EVENT_MANAGER
	HRESULT Event_CreateObject(_uint iLevelIndex, const _wstring & strLayerTag, const _wstring & strPrototypeTag, _uint iSize = 0, void* pArg = nullptr);
	HRESULT Event_DestoryObject(class CGameObject* pGameObject);
#pragma endregion


#pragma region EVENT_MANAGER
	//HRESULT Event_CreateObject(_uint iLevelIndex, const _wstring & strLayerTag, const _wstring & strPrototypeTag, _uint iSize = 0, void* pArg = nullptr);
	//HRESULT Event_DestoryObject(class CGameObject* pGameObject);
#pragma endregion


#pragma region PICKING_MANAGER
	_float3 Get_PickingObject_CenterPos(_uint iLevelIndex , const _wstring & strLayerTag);
	_float3 Get_PickingObject_JustPos(_uint iLevelIndex, const _wstring& strLayerTag);
	_float3 Get_PickingBackGround_JustPos();


	void Transform_MouseRay_ToLocalSpace(const _float4x4 & WorldMatrix);
	_bool isPicked_InLocalSpace(const _float3 & vPointA, const _float3 & vPointB, const _float3 & vPointC, _float3 * pOut);
#pragma endregion


	void Delete(class CGameObject* pObj);


private:
	class CGraphic_Device*		m_pGraphic_Device = { nullptr };
	class CLevel_Manager*		m_pLevel_Manager = { nullptr };
	class CObject_Manager*		m_pObject_Manager = { nullptr };
	class CComponent_Manager*	m_pComponent_Manager = { nullptr };
	class CTimer_Manager*		m_pTimer_Manager = { nullptr };
	class CRenderer*			m_pRenderer = { nullptr };


	/* ���⼭���� �츮�� �߰��� ��ɵ�*/
	class CKey_Manager*		m_pKey_Manager = { nullptr };
	class CEvent_Manager*	m_pEvent_Manager = { nullptr };
	class CUI_Manager*		m_pUI_Manager = { nullptr };
	class CPickingManager*  m_pPicking_Manager = { nullptr };


public:
	void Release_Engine();
	virtual void Free() override;

};

END

